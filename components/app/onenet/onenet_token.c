#include "onenet_token.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "mbedtls/base64.h"
#include "mbedtls/md.h"

#define TOKEN_VERSION "2018-10-31"
#define TOKEN_KEY_MAX_SIZE 64
#define TOKEN_DIGEST_MAX_SIZE 32
#define TOKEN_BASE64_MAX_SIZE 64
#define TOKEN_SIGN_TEXT_MAX_SIZE 256

/* 追加普通字符串，并在缓冲区不足时返回错误 */
static int append_text(char *output, size_t output_size, size_t *offset,
                       const char *text)
{
    size_t length = strlen(text);

    if (*offset >= output_size || length >= output_size - *offset)
    {
        return -1;
    }

    memcpy(output + *offset, text, length);
    *offset += length;
    output[*offset] = '\0';
    return 0;
}

/* 按 RFC 3986 对 Token 参数进行 URL 编码 */
static int append_url_encoded(char *output, size_t output_size, size_t *offset,
                              const char *text)
{
    static const char hex[] = "0123456789ABCDEF";

    for (const unsigned char *p = (const unsigned char *)text; *p; ++p)
    {
        bool plain = (*p >= 'A' && *p <= 'Z') ||
                     (*p >= 'a' && *p <= 'z') ||
                     (*p >= '0' && *p <= '9') ||
                     *p == '-' || *p == '.' || *p == '_' || *p == '~';

        if (plain)
        {
            if (*offset + 1 >= output_size)
            {
                return -1;
            }
            output[(*offset)++] = (char)*p;
        }
        else
        {
            if (*offset + 3 >= output_size)
            {
                return -1;
            }
            output[(*offset)++] = '%';
            output[(*offset)++] = hex[*p >> 4];
            output[(*offset)++] = hex[*p & 0x0F];
        }
    }

    output[*offset] = '\0';
    return 0;
}

/* 获取签名方法名称 */
static const char *signature_name(enum sig_method_e method)
{
    switch (method)
    {
    case SIG_METHOD_MD5:
        return "md5";
    case SIG_METHOD_SHA1:
        return "sha1";
    case SIG_METHOD_SHA256:
        return "sha256";
    default:
        return NULL;
    }
}

/* 获取 mbedTLS 摘要类型 */
static mbedtls_md_type_t signature_type(enum sig_method_e method)
{
    switch (method)
    {
    case SIG_METHOD_MD5:
        return MBEDTLS_MD_MD5;
    case SIG_METHOD_SHA1:
        return MBEDTLS_MD_SHA1;
    case SIG_METHOD_SHA256:
        return MBEDTLS_MD_SHA256;
    default:
        return MBEDTLS_MD_NONE;
    }
}

int32_t dev_token_generate_ex(char *token, size_t token_size,
                              enum sig_method_e method, uint32_t exp_time,
                              const char *product_id, const char *dev_name,
                              const char *access_key)
{
    unsigned char key[TOKEN_KEY_MAX_SIZE] = {0};
    unsigned char digest[TOKEN_DIGEST_MAX_SIZE] = {0};
    unsigned char signature[TOKEN_BASE64_MAX_SIZE] = {0};
    char sign_text[TOKEN_SIGN_TEXT_MAX_SIZE] = {0};
    char expiration[16] = {0};
    char result_text[ONENET_TOKEN_MAX_SIZE] = {0};
    size_t key_len = 0;
    size_t signature_len = 0;
    size_t offset = 0;
    const char *method_name = signature_name(method);
    const mbedtls_md_info_t *md_info =
        mbedtls_md_info_from_type(signature_type(method));
    int result;

    if (token == NULL || token_size == 0 || product_id == NULL ||
        product_id[0] == '\0' || access_key == NULL || access_key[0] == '\0' ||
        method_name == NULL || md_info == NULL)
    {
        return -1;
    }
    token[0] = '\0';

    result = mbedtls_base64_decode(key, sizeof(key), &key_len,
                                   (const unsigned char *)access_key,
                                   strlen(access_key));
    if (result != 0 || key_len == 0)
    {
        return -1;
    }

    bool has_device = dev_name != NULL && dev_name[0] != '\0';
    result = has_device
                 ? snprintf(sign_text, sizeof(sign_text),
                            "%lu\n%s\nproducts/%s/devices/%s\n%s",
                            (unsigned long)exp_time, method_name, product_id,
                            dev_name, TOKEN_VERSION)
                 : snprintf(sign_text, sizeof(sign_text),
                            "%lu\n%s\nproducts/%s\n%s",
                            (unsigned long)exp_time, method_name, product_id,
                            TOKEN_VERSION);
    if (result < 0 || (size_t)result >= sizeof(sign_text))
    {
        return -1;
    }

    result = mbedtls_md_hmac(md_info, key, key_len,
                             (const unsigned char *)sign_text,
                             strlen(sign_text), digest);
    if (result != 0)
    {
        return -1;
    }

    result = mbedtls_base64_encode(signature, sizeof(signature), &signature_len,
                                   digest, mbedtls_md_get_size(md_info));
    if (result != 0 || signature_len >= sizeof(signature))
    {
        return -1;
    }
    signature[signature_len] = '\0';

    result = snprintf(expiration, sizeof(expiration), "%lu",
                      (unsigned long)exp_time);
    if (result < 0 || (size_t)result >= sizeof(expiration))
    {
        return -1;
    }

    if (append_text(result_text, sizeof(result_text), &offset,
                    "version=" TOKEN_VERSION "&res=products%2F") != 0 ||
        append_url_encoded(result_text, sizeof(result_text), &offset,
                           product_id) != 0)
    {
        return -1;
    }

    if (has_device)
    {
        if (append_text(result_text, sizeof(result_text), &offset,
                        "%2Fdevices%2F") != 0 ||
            append_url_encoded(result_text, sizeof(result_text), &offset,
                               dev_name) != 0)
        {
            return -1;
        }
    }

    if (append_text(result_text, sizeof(result_text), &offset, "&et=") != 0 ||
        append_text(result_text, sizeof(result_text), &offset, expiration) != 0 ||
        append_text(result_text, sizeof(result_text), &offset, "&method=") != 0 ||
        append_text(result_text, sizeof(result_text), &offset, method_name) != 0 ||
        append_text(result_text, sizeof(result_text), &offset, "&sign=") != 0 ||
        append_url_encoded(result_text, sizeof(result_text), &offset,
                           (const char *)signature) != 0 ||
        offset >= token_size)
    {
        return -1;
    }

    memcpy(token, result_text, offset + 1);
    return 0;
}
