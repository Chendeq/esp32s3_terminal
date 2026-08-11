#include "url_encode.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


// 判断字符是否为保留字符（无需编码）
static int is_reserved_char(char c) {
    return (isalnum((unsigned char)c) || 
            c == '-' || c == '_' || c == '.' || c == '~');
}

size_t url_encode_len(const char *src, int src_len) {
    if (src == NULL) return 0;
    
    size_t len = 0;
    size_t max_len = (src_len < 0) ? strlen(src) : (size_t)src_len;
    
    for (size_t i = 0; i < max_len && src[i] != '\0'; i++) {
        len += is_reserved_char(src[i]) ? 1 : 3;
    }
    
    return len;
}

size_t url_encode(const char *src, int src_len, char *dst, size_t dst_size) {
    if (src == NULL || dst == NULL || dst_size == 0) {
        return 0;
    }
    
    static const char hex[] = "0123456789ABCDEF";
    size_t j = 0;
    size_t max_len = (src_len < 0) ? strlen(src) : (size_t)src_len;
    
    for (size_t i = 0; i < max_len && src[i] != '\0'; i++) {
        // 预留 1 字节给 \0
        if (j >= dst_size - 1) {
            return 0;  // 缓冲区不足
        }
        
        unsigned char c = (unsigned char)src[i];
        
        if (is_reserved_char((char)c)) {
            dst[j++] = (char)c;
        } else {
            // 需要 3 字节：%XX
            if (j + 3 >= dst_size) {
                return 0;  // 缓冲区不足
            }
            dst[j++] = '%';
            dst[j++] = hex[c >> 4];
            dst[j++] = hex[c & 0x0F];
        }
    }
    
    dst[j] = '\0';
    return j;
}

size_t url_decode(const char *src, char *dst, size_t dst_size) {
    if (src == NULL || dst == NULL || dst_size == 0) {
        return 0;
    }
    
    size_t j = 0;
    size_t len = strlen(src);
    
    for (size_t i = 0; i < len && j < dst_size - 1; i++) {
        if (src[i] == '%' && i + 2 < len) {
            // 解析 %XX
            int high, low;
            char h = src[i + 1];
            char l = src[i + 2];
            
            if (isdigit((unsigned char)h)) {
                high = h - '0';
            } else {
                high = toupper((unsigned char)h) - 'A' + 10;
            }
            
            if (isdigit((unsigned char)l)) {
                low = l - '0';
            } else {
                low = toupper((unsigned char)l) - 'A' + 10;
            }
            
            if (high >= 0 && high <= 15 && low >= 0 && low <= 15) {
                dst[j++] = (char)((high << 4) | low);
                i += 2;
                continue;
            }
            // 格式错误，原样输出 %
        } else if (src[i] == '+') {
            // application/x-www-form-urlencoded 规范
            dst[j++] = ' ';
            continue;
        }
        
        dst[j++] = src[i];
    }
    
    dst[j] = '\0';
    return j;
}

char *url_encode_alloc(const char *src, int src_len) {
    if (src == NULL) return NULL;
    
    size_t need = url_encode_len(src, src_len);
    char *dst = (char *)malloc(need + 1);  // +1 for \0
    
    if (dst == NULL) {
        return NULL;
    }
    
    size_t written = url_encode(src, src_len, dst, need + 1);
    if (written == 0 && need > 0) {
        free(dst);
        return NULL;
    }
    
    return dst;
}