# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)



set( SRCS
    WeatherApp/weatherApp.c
 )




set(INCLUDE
    WeatherApp
)




 set(COMPONENT_REQUIRES
    lvgl
    lvgl_St7789_driver
    )



idf_component_register(SRCS   "${SRCS}"
                    INCLUDE_DIRS "${INCLUDE}" 
                    REQUIRES "${COMPONENT_REQUIRES}" )

