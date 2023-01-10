#include "UpgradeApp.h"
#include "Wifi.h"
#include "lvgl.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "UpgradeAppUI.h"
#include "MPU.h"
#include "AppController.h"

static SemaphoreHandle_t ap_semUp;
static const char *TAG = "UpgradeApp";

#define UPGRADE_APP_NAME "Upgrade"
extern const lv_img_dsc_t UpgradeIco;

BaseType_t xReturned_lvtask = pdFALSE;
TaskHandle_t xHandle_lvtask = NULL;
static void lvtask(void *parameter)
{
    while (1)
    {
        // LVGL任务主函数
        vTaskDelay(5 / portTICK_PERIOD_MS);
        LVGL_OPERATE_LOCK(lv_task_handler();)
    }
    vTaskDelete(NULL);
}




static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            // Write out data
            //printf("%.*s", evt->data_len, (char*)evt->data);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}



esp_err_t Upgrade()
{
    esp_http_client_config_t config =
    {
        .url = "http://49.235.152.179:8080/esp32/MicroCubic.bin",
        .event_handler = _http_event_handler,
    };
    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {
        xSemaphoreGive(ap_semUp);//释放信号量
        esp_restart();
    } else {
        xSemaphoreGive(ap_semUp);//释放信号量
        ESP_LOGE(TAG, "Firmware Upgrades Failed:%d\n",ret);
        SetUpgradeTextSrc("Update failed","Please contact author\nemail :2235812260@qq.com");
    }
    return ret;

}

static int UpgradeInit(AppController *sys)
{
    UpgradeUIInit();
    UpgradeUIScrinit();

    xReturned_lvtask = xTaskCreate(
        lvtask,                      /*任务函数*/
        "lvtask",                    /*带任务名称的字符串*/
        8 * 1024,                     /*堆栈大小，单位为字节*/
        NULL,                         /*作为任务输入传递的参数*/
        1,                            /*任务的优先级*/
        &xHandle_lvtask); /*任务句柄*/
    return 0;
}




static void UpgradeProcess(AppController *sys, const ImuAction *act_info)
{
    if (act_info->action == BACK)
    {
        sys->AppExit();
        return;
    }
    if(wifi_flag == false)
    {
        SetUpgradeTextSrc("unable to update","Please connect to wifi");
    } else {
        SetUpgradeTextSrc("able to update","If you want to update\nPlease push three sec");
       if(act_info->action == GO_FORWORD){
            SetUpgradeTextSrc("upgrading .... ","Please wait, Don't Shutdown\nAutomatically restart after completion");
            vTaskDelay(500/portTICK_PERIOD_MS);
            ap_semUp = xSemaphoreCreateBinary();//创建信号量
            Upgrade();
            uint32_t result = xSemaphoreTake(ap_semUp,portMAX_DELAY);
            // if(result == pdPASS){
            
            // }
       }
        
    }
    vTaskDelay((100)/ portTICK_PERIOD_MS);

} 

static void UpgradeBackground(AppController *sys, const ImuAction *act_info)
{
  
}

static int UpgradeExit(void *param)
{

    // 查杀任务
    if (xReturned_lvtask == pdPASS)
    {
        vTaskDelete(xHandle_lvtask);
    }

    UpgradeUIDel();
    return 0;
}


APPOBJ UpgradeApp = {UPGRADE_APP_NAME, &UpgradeIco, "",
                UpgradeInit,UpgradeProcess,UpgradeBackground,
                UpgradeExit  };