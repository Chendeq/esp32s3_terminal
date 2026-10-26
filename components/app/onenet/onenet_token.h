#ifndef ONENET_TOKEN_H
#define ONENET_TOKEN_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* OneNET 设备 Token 缓冲区大小 */
#define ONENET_TOKEN_MAX_SIZE 256

enum sig_method_e {
    SIG_METHOD_MD5,
    SIG_METHOD_SHA1,
    SIG_METHOD_SHA256,
};

/**
 * @brief 生成 OneNET 设备访问 Token
 *
 * 使用 OneNET 2018-10-31 签名算法，包含资源、过期时间、
 * 签名方法和 HMAC 签名。
 *
 * @param token       输出缓冲区，成功时以 '\0' 结尾
 * @param token_size  输出缓冲区大小
 * @param method      HMAC 签名方法
 * @param exp_time    Token 过期时间（Unix 时间戳）
 * @param product_id  OneNET 产品 ID
 * @param dev_name    设备名称；传 NULL 表示产品级 Token
 * @param access_key  Base64 编码的 OneNET 产品密钥
 *
 * @return 成功返回 0，失败返回负值
 */
int32_t dev_token_generate_ex(char *token, size_t token_size,
                              enum sig_method_e method, uint32_t exp_time,
                              const char *product_id, const char *dev_name,
                              const char *access_key);

#ifdef __cplusplus
}
#endif

#endif /* ONENET_TOKEN_H */
