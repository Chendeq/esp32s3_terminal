#ifndef __BSP_SDCARD_H__
#define __BSP_SDCARD_H__

#include "esp_err.h"
#include <stdbool.h>

esp_err_t sdcard_init(void);
void sdcard_unmount(void);
bool sdcard_is_mounted(void);
void sdcard_get_info(void);


#endif
