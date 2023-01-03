#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "lvgl.h"
#include "sntp.h"
#include "Sd.h"
#include "Wifi.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "AppController.h"
#include "WeatherApp.h"



//测试
#include "MPU.h"


Mpu mpu;
ImuAction *a;



 AppController appController;



extern "C" void app_main(void)
{

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);


    appController.Init();
    mpu.Init();
    appController.AppInstall(&WeatherApp);


    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "cn.ntp.org.cn");
    sntp_setservername(1, "ntp1.aliyun.com");

    a=mpu.GetAction();
    while(1){
        a=mpu.GetAction();
        appController.MainProcess(a);   
        vTaskDelay(200 / portTICK_PERIOD_MS);
        
    }
    

}
