#include "WifiApp.h"
#include "lvgl.h"
#include "WifiAppUI.h"
#include "MPU.h"
#include "AppController.h"
#include "Wifi.h"
#include "string.h"
#include "Spiffs.h"

#define WIFI_APP_NAME "WIFI SET"
#define WIFI_APP_CFG_NAME "wifi.cfg"

extern const lv_img_dsc_t WifiIco;


//wifi连接成功标志

char * statetext[]= { "Wifi Connected ","Wifi Connecting...","Wifi Disconnected","Start Wifi AP Mode"};



/**************************************************************************************************/

static int WifiInit(AppController *sys)
{
    WifiUIInit();
    WifiUIScrinit();
    SetWifiTextSrc(statetext[1]);
    Readwificfg();
    return 0;
}


void Writewificfg()
{
    char temp[100];
    sprintf(temp,"%s\n%s\n",wifi_ssid,wifi_password);
    SpiffsWriteFile(WIFI_APP_CFG_NAME,temp);

}

void Readwificfg()
{
    char readtemp[100];
    int size = SpiffsReadFile(WIFI_APP_CFG_NAME,readtemp);
    if(size == 0){
        char writetemp[100];
        sprintf(writetemp,"%s\n%s",wifi_ssid,wifi_password);
        SpiffsWriteFile(WIFI_APP_CFG_NAME,writetemp);
    }
    else {
        char *wificfg[2];
        SpiffsAnalyseParam(readtemp,2,wificfg);
        strcpy(wifi_ssid,wificfg[0]);
        strcpy(wifi_password,wificfg[1]);
    }

}


static void WifiProcess(AppController *sys, const ImuAction *act_info)
{
    if (act_info->action == BACK)
    {
        sys->AppExit();
        return;
    }
    if(wifi_flag == false){
        SetWifiTextSrc(statetext[3]);
        lv_task_handler();//更新屏幕数据
        startwifi();
        Writewificfg(); 
    }else {
        SetWifiTextSrc(statetext[0]);
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