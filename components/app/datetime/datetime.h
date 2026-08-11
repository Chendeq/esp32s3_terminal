#ifndef __DATETIME_H__
#define __DATETIME_H__

#include <stdbool.h>
#include <stddef.h>

/* 等待SNTP时间同步 */
void datetime_wait_sntp_sync(void);

/* 获取当前时间字符串 */
void datetime_get_current_time_str(char *time_buf, size_t time_len, char *date_buf, size_t date_len, char *week_buf, size_t week_len);

/* 检查时间是否同步 */
bool datetime_is_time_synced(void);

/* 更新calendar显示 */
void datetime_get_current_date(int *year, int *month, int *day);

#endif
