# 递归查找所有.c文件
file(GLOB_RECURSE SRCS 
    "*.c"
    "custom/*.c"
    "generated/*.c"
    "generated/guider_fonts/*.c"
    "generated/images/*.c"
)

# 设置头文件搜索路径
set(INCLUDE_DIRS 
    .
    "custom"
    "generated"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/custom
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
)

# 注册组件
idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    REQUIRES 
        driver
        lvgl
        esp_common
        freertos
        app
        bsp
)
