#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "lvgl.h"
#include "Screen.h"
#include "Sd.h"
#include "Wifi.h"
#include "nvs_flash.h"


/***********************************全局变量***************************************/
//使用的Espidf_Study全局变量全都定义在此
Screen screen;
Sd sdCard;
Wifi wifi;

extern "C" void app_main(void)
{

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    wifi.Init();
    sdCard.Init();
    wifi.BeginSTA("304","22358122");
    screen.Init();

   
}