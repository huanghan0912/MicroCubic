#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "lvgl.h"
#include "sntp.h"
#include "Sd.h"
#include "HttpServer.h"
// #include "Wifi.h"
#include "Screen.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "AppController.h"
#include "WeatherApp.h"
#include "MPU.h"




Mpu mpu;
ImuAction *a;



int count=0;

 AppController appController;

// lvgl 操作的锁
SemaphoreHandle_t lvgl_mutex;

// LVGL操作的安全宏（避免脏数据）
#define LVGL_OPERATE_LOCK(CODE)                          \
    if (pdTRUE == xSemaphoreTake(lvgl_mutex, portMAX_DELAY)) \
    {                                                        \
        CODE                                                 \
        xSemaphoreGive(lvgl_mutex);                          \
    }



extern "C" void app_main(void)
{

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    //测试

    // appController.Init();
    // mpu.Init();
    // appController.AppInstall(&WeatherApp);


    // sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // sntp_setservername(0, "ntp5.aliyun.com");
    // sntp_setservername(1, "ntp1.aliyun.com");

    // a=mpu.GetAction();

    // lvgl_mutex = xSemaphoreCreateMutex();
    // while(1){
    //     LVGL_OPERATE_LOCK(lv_task_handler();)
    //     if(count == 3){
    //         a=mpu.GetAction();
    //         count =0;
    //     }
    //     appController.MainProcess(a);    
    //     count++;
        
    // }
    

}
