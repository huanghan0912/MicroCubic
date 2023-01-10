#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "lvgl.h"
#include "sntp.h"
#include "Sd.h"
#include "HttpServer.h"
#include "Screen.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "AppController.h"
#include "WeatherApp.h"
#include "Game2048App.h"
#include "WifiApp.h"
#include "MPU.h"
#include "Spiffs.h"
#include "UpgradeApp.h"


Mpu mpu;
ImuAction *a;



int count=0;

AppController appController;

extern "C" void app_main(void)
{

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    //测试
    //测试
    mpu.Init();
    appController.Init();
    // appController.AppInstall(&WeatherApp);
    appController.AppInstall(&WifiApp);
    // appController.AppInstall(&Game2048App);
    appController.AppInstall(&UpgradeApp);

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "ntp5.aliyun.com");
    sntp_setservername(1, "ntp1.aliyun.com");

    a=mpu.GetAction();

    while(1){
        LVGL_OPERATE_LOCK(lv_task_handler();)
        if(count == 2){
            a=mpu.GetAction();
            count =0;
        }
        appController.MainProcess(a);    
        count++;
        
    }
    

}
