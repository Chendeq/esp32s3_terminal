file(GLOB_RECURSE SRCS 
    "bsp_lcd/*.c"
    "bsp_touch/*.c"
    "bsp_sdcard/*.c"
)

set(INCLUDE_DIRS
    bsp_lcd
    bsp_touch
    bsp_sdcard
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp_lcd
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp_touch
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp_sdcard
)

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    REQUIRES 
        driver
        esp_timer
        lvgl
        esp_common
        freertos
        heap
        esp_lcd
        esp_lcd_st7789
        esp_lcd_touch
        esp_lcd_touch_ft5x06
        sdmmc
        fatfs
)
