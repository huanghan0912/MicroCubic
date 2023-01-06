# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)



set( SRCS_CORE
    Sd/Sd.cpp
    Wifi/Wifi.cpp
    HttpClient/HttpClient.cpp
    HttpServer/HttpServer.cpp
    Spiffs/Spiffs.cpp
 )




set(INCLUDE_CORE
    Sd
    Wifi
    HttpClient
    HttpServer
    Spiffs
)


 set(COMPONENT_REQUIRES
    esp_http_server
    esp_http_client
    fatfs
    nvs_flash 
    esp_event 
    esp_wifi
    driver
    spiffs
    )



idf_component_register(SRCS   "${SRCS_CORE}"
                    INCLUDE_DIRS "${INCLUDE_CORE}"
                    EMBED_FILES  "HttpServer/wifi.html"
                    REQUIRES "${COMPONENT_REQUIRES}" )

