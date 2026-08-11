#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include <stdbool.h>

/* 初始化应用任务 */
void app_task_init(void);

/* 开始音乐任务 */
void music_task_start(void);

/* 停止音乐任务 */
void music_task_stop(void);

#endif
