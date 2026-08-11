#include "bsp_sdcard.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "locale.h"
#include "dirent.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include <stdbool.h>

static const char *TAG = "bsp_sdcard";

#define MOUNT_POINT "/S"

#define SD_CS         GPIO_NUM_9
#define SD_SCK        GPIO_NUM_14
#define SD_MISO       GPIO_NUM_17
#define SD_MOSI       GPIO_NUM_10

static bool sdcard_mounted = false;
static sdmmc_card_t *sdcard_card = NULL;

void sdcard_unmount(void)
{
    if (sdcard_mounted) {
        esp_vfs_fat_sdcard_unmount(MOUNT_POINT, sdcard_card);
        spi_bus_free(SD_CS);
        sdcard_mounted = false;
        ESP_LOGI(TAG, "SD card unmounted");
    }
}

bool sdcard_is_mounted(void)
{
    return sdcard_mounted;
}

/**
 * @brief 打印SD卡的容量信息
 */
void sdcard_get_info(void)
{
    uint64_t card_total_bytes = 0;//SD 总大小
    uint64_t total_bytes;
    uint64_t free_bytes;
    uint64_t used_bytes;

    if (sdcard_is_mounted() == 0) // 如果SD卡没有挂载
        return;

    // 1. 获取SD卡硬件总容量（物理容量）
    card_total_bytes = (uint64_t)sdcard_card->csd.capacity * sdcard_card->csd.sector_size;

    // 获取TF卡信息
    esp_vfs_fat_info(TAG, &total_bytes, &free_bytes);
    used_bytes = total_bytes - free_bytes;

    // 打印信息 转换单位  Byte > GB
    ESP_LOGI(TAG, "SD卡物理总容量: %.2f GB", (double)card_total_bytes / (1024 * 1024 * 1024));
    ESP_LOGI(TAG, "FAT分区总空间: %.2f GB", (double)total_bytes / (1024 * 1024 * 1024));
    ESP_LOGI(TAG, "已使用空间: %.6f GB", (double)used_bytes / (1024 * 1024 * 1024));
    ESP_LOGI(TAG, "剩余空间: %.6f GB", (double)free_bytes / (1024 * 1024 * 1024));

}

esp_err_t sdcard_init(void)
{
    esp_err_t ret;

    if (sdcard_mounted) {
        ESP_LOGW(TAG, "SD card already mounted");
        return ESP_OK;
    }

    // 1. 配置 Locale 为中文（中国）UTF-8，覆盖所有类别
    setlocale(LC_ALL, "zh_CN.UTF-8");

    // 2. 配置 SDMMC 主机
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_1BIT;

    // 3. SDMMC接口配置
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT(); // 获取SDMMC槽默认配置
    slot_config.width = 1;                                         // 1线模式
    slot_config.clk = SD_SCK;                                      // CLK引脚
    slot_config.cmd = SD_MOSI;                                     // CMD引脚
    slot_config.d0 = SD_MISO;                                      // D0引脚
    slot_config.gpio_cd = GPIO_NUM_NC;                             // 不使用卡检测引脚
    slot_config.gpio_wp = GPIO_NUM_NC;                             // 不使用写保护引脚

    // 4. 挂载配置
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,  // 挂载失败时不格式化
        .max_files = 5,                   // 最大同时打开文件数
        .allocation_unit_size = 16 * 1024 // 分配单元大小
    };

    // 5. 挂载SD卡到文件系统
    ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &sdcard_card);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "mount success"); // 挂载成功，标记为已挂载
    }
    else
    {
        sdcard_card = NULL;
        ESP_LOGW(TAG, "mount failure %s", esp_err_to_name(ret));
    }

    sdcard_mounted = true;

    // 6. 打印SD卡信息
    sdcard_get_info();

    return ESP_OK;
}
