# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)



set( SRCS
    WeatherApp/WeatherApp.cpp
    WeatherApp/WeatherUI/WeatherUI.c
    WeatherApp/WeatherUI/ch_font_20.c
    WeatherApp/WeatherUI/lv_font_ibmplex_64.c
    WeatherApp/WeatherUI/lv_font_ibmplex_115.c
    WeatherApp/WeatherUI/WeatherIco.c
    Screen.cpp
 )



set(INCLUDE
    WeatherApp/WeatherUI/.
    WeatherApp/.
    .
)




 set(COMPONENT_REQUIRES
    lvgl
    lvgl_esp32_drivers
    MYDriver
    json
    mpu6050
    AppController
    )



idf_component_register(SRCS   "${SRCS}"
                    INCLUDE_DIRS "${INCLUDE}" 
                    REQUIRES "${COMPONENT_REQUIRES}" )

