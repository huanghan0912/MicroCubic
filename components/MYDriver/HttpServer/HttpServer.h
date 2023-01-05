#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include "esp_http_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t ap_sem;
extern char wifi_name[30];
extern char wifi_password[30];

static esp_err_t index_get_handler(httpd_req_t *req);

static esp_err_t echo_post_handler(httpd_req_t *req);

httpd_handle_t start_webserver(void);

static void stop_webserver(httpd_handle_t server);



#endif