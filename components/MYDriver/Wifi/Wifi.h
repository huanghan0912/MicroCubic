/**
 * @file Wifi.h
 * @author huanghan0912 (huanghan0912@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-16
 * 
 * 
 */
#ifndef _WIFI_H_
#define _WIFI_H_


#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"


extern bool wifi_flag;
extern char wifi_ssid[30];
extern char wifi_password[30];

#define EXAMPLE_ESP_WIFI_SSID      "MicroCubic"
#define EXAMPLE_ESP_WIFI_PASS      "12345678"
#define EXAMPLE_ESP_WIFI_CHANNEL   4
#define EXAMPLE_MAX_STA_CONN       3


void wifi_init_softap(void);  

void wifiAP_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);

static void wifiSTA_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);

void wifi_init_sta( char * ssid, char * password);

void WifiSTADel();

void wifiAPDel();
void startwifi();
#endif
