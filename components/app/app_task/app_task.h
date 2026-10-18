#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include <stdbool.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* 初始化应用任务 */
void app_task_init(void);

/* 开始音乐任务 */
void music_task_start(void);

/* 停止音乐任务 */
void music_task_stop(void);

/* Called by mic_init(); a partial creation is cleaned up by the caller. */
esp_err_t mic_tasks_create(TaskHandle_t *writer, TaskHandle_t *capture);

#endif
