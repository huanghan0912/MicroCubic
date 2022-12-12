#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "lvgl.h"
#include "Screem.h"
#include "Sd.h"
#include "Wifi.h"



Screem screem;
Sd sdCard;
Wifi wifi;

extern "C" void app_main(void)
{
    wifi.Init();
    sdCard.Init();
    wifi.BeginSTA("304","22358122");
    screem.Init();
}