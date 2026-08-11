file(GLOB_RECURSE SRCS
    "lvgl_port/*.c"
    "url_encode/*.c"
)

set(INCLUDE_DIRS
    lvgl_port
    url_encode
    ${CMAKE_CURRENT_SOURCE_DIR}/lvgl_port
    ${CMAKE_CURRENT_SOURCE_DIR}/url_encode
)

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    REQUIRES
        bsp
        lvgl
        freertos
        heap
        esp_common
        esp_lvgl_port
        esp_timer
)