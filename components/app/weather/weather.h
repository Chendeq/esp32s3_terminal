#ifndef __WEATHER_H
#define __WEATHER_H

#include <stdbool.h>
#include "esp_err.h"

typedef struct {
    char city[64];                      // 城市名称
    char temp[8];                       // 温度
    char weather[16];                   // 天气状况描述
    char last_update[32];               // 上一次更新时间
    char forecast_date[3][16];          // 未来3天日期
    char forecast_weather[3][16];       // 未来3天天气状况描述
    char forecast_temp[3][32];          // 未来3天温度
    bool has_data;                      // 是否已获取过有效数据
} weather_cache_t;

/* 获取天气信息 */
esp_err_t get_weather(void);
/* 获取位置信息 */
esp_err_t get_location(void);
/* 获取缓存的天气数据 */
void weather_get_cached(weather_cache_t *out);

#endif
