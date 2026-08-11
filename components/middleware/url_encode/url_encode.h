#ifndef __URL_ENCODE_H
#define __URL_ENCODE_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 计算 URL 编码后所需缓冲区大小（不含 \0）
 * @param src 源字符串
 * @param src_len 源字符串长度（字节），传 -1 表示以 \0 结尾
 * @return 编码后所需字节数
 */
size_t url_encode_len(const char *src, int src_len);

/**
 * URL 编码（百分比编码）
 * @param src 源字符串
 * @param src_len 源字符串长度，传 -1 表示以 \0 结尾
 * @param dst 目标缓冲区
 * @param dst_size 目标缓冲区大小
 * @return 实际写入的字节数（不含 \0），失败返回 0
 */
size_t url_encode(const char *src, int src_len, char *dst, size_t dst_size);

/**
 * URL 解码
 * @param src 编码后的字符串
 * @param dst 目标缓冲区
 * @param dst_size 目标缓冲区大小
 * @return 实际写入的字节数（不含 \0），失败返回 0
 */
size_t url_decode(const char *src, char *dst, size_t dst_size);

/**
 * 动态分配内存进行 URL 编码（需调用者 free）
 * @param src 源字符串
 * @param src_len 源字符串长度，传 -1 表示以 \0 结尾
 * @return 编码后的字符串，失败返回 NULL
 */
char *url_encode_alloc(const char *src, int src_len);


#endif /* URL_ENCODE_H */