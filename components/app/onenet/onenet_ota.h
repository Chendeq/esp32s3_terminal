#ifndef ONENET_OTA_H
#define ONENET_OTA_H

#include "esp_err.h"

/* 新固件联网自检时间，超时后自动回滚 */
#define ONENET_OTA_SELF_TEST_TIMEOUT_MS (120 * 1000)

/**
 * @brief 初始化 OTA 工作线程和回滚保护
 *
 * 若当前固件处于待验证状态，会创建看门狗；固件必须在自检时间内
 * 成功联网并上报版本，否则通知引导程序回滚。
 */
esp_err_t onenet_ota_init(void);

/**
 * @brief 唤醒 OTA 工作线程检查升级任务
 *
 * 工作线程会上报当前版本、确认运行固件有效、查询升级任务，
 * 并在存在新版本时写入非活动 OTA 分区。
 */
esp_err_t onenet_ota_start(void);

#endif /* ONENET_OTA_H */
