#include "WifiApp.h"
#include "lvgl.h"
#include "WifiAppUI.h"
#include "MPU.h"
#include "AppController.h"
#include "Wifi.h"

#define WIFI_APP_NAME "WIFI SET"


extern const lv_img_dsc_t WifiIco;


//wifi连接成功标志

char * statetext[]= { "Wifi Connected","Wifi Connecting...","Wifi Disconnected","Start Wifi AP Mode"};



/**************************************************************************************************/

static int WifiInit(AppController *sys)
{
    WifiUIInit();
    WifiUIScrinit();
    SetTextSrc(statetext[1]);
    return 0;
}


static void WifiProcess(AppController *sys, const ImuAction *act_info)
{
  
    if (act_info->action == BACK)
    {
        sys->AppExit();
        return;
    }
    if(wifi_flag == false){
        wifi_init_sta(wifi_ssid,wifi_password);
        SetTextSrc(statetext[3]);
        lv_task_handler();
        startwifi();
    }
    else{
        SetTextSrc(statetext[0]);
    }
    vTaskDelay((100)/ portTICK_PERIOD_MS);

} 

static void WifiBackground(AppController *sys, const ImuAction *act_info)
{
  
}

static int WifiExit(void *param)
{
    WifiUIDel();
    return 0;
}


APPOBJ WifiApp = {WIFI_APP_NAME, &WifiIco, "",
                WifiInit,WifiProcess,WifiBackground,
                WifiExit  };