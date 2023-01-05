#include "WeatherApp.h"
#include "HttpClient.h"
#include "WeatherUI.h"
#include "esp_log.h"
#include "cJSON.h"
#include <time.h>
#include "sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#include "AppController.h"


extern const lv_img_dsc_t WeatherIco;


#define WEATHER_APP_NAME "Weather"

struct tm timeinfo ;
struct WeatherText Weather_text;


HttpClient httpclient;
static const char *TAG="WeatherApp";
bool get_weather_flag=false;

char Weather_url[150];  //调用天气的Url
char Local_name[8]="合肥";  //所在地名称
char Local_code[15]="101220101";    //所在地代码
#define key "3af5fbac7e89420188958bd85c0fb740"   //调用天气的秘钥

//获取互联网天气任务句柄
TaskHandle_t GetWeather = NULL;


void WeatherAppInit()
{
  sprintf(Weather_url,"https://devapi.qweather.com/v7/weather/now?location=%s&key=%s&gzip=n",Local_code,key);
  ESP_LOGI(TAG,"%s",Weather_url);
  httpclient.Init(Weather_url);
  WeatherUIInit();
}

/**
 * @brief 从互联网上获取界面上所需的所有信息
 * 
 * @param pvParameters 
 */
void GetWeatherTextTask(void *pvParameters)
{
  char *json_root =(char*) malloc(450*sizeof(char));
  while(1){
    GetnetworkTime();
    httpclient.GetResponse(json_root);
    
    //判断是否成功获取天气
    cJSON* check;
    cJSON* root =cJSON_Parse(json_root);
    check =cJSON_GetObjectItem(root,"code");
    

    cJSON* now =cJSON_GetObjectItem(root,"now");
    cJSON* cjson_temp = cJSON_GetObjectItem(now,"temp");
    cJSON* cjson_icon = cJSON_GetObjectItem(now,"icon");
    cJSON* cjson_weather = cJSON_GetObjectItem(now,"text");
    cJSON* cjson_wind = cJSON_GetObjectItem(now,"windDir");
    cJSON* cjson_windlevel = cJSON_GetObjectItem(now,"windScale");
    cJSON* cjson_windSpeed = cJSON_GetObjectItem(now,"windSpeed");
    cJSON* cjson_humidity = cJSON_GetObjectItem(now,"humidity");

    Weather_text.temp=atoi(cjson_temp->valuestring);
    Weather_text.icon=atoi(cjson_icon->valuestring);
    strcpy(Weather_text.weather, cjson_weather->valuestring);
    strcpy(Weather_text.wind, cjson_wind->valuestring);
    strcpy(Weather_text.windlevel, cjson_windlevel->valuestring);
    strcpy(Weather_text.windSpeed,cjson_windSpeed->valuestring );
    Weather_text.humidity=atoi(cjson_humidity->valuestring);

    ESP_LOGI(TAG,"%s   %d",Weather_text.weather,Weather_text.icon);
    if(atoi(check->valuestring)==200){
      get_weather_flag=true;
      ESP_LOGI(TAG,"获取天气成功");
    }
    else{
      ESP_LOGW(TAG,"%s",check->valuestring);
    }

    vTaskDelay(((1000*60*60*2)/ portTICK_PERIOD_MS));
  }
   free(json_root);
  
}


void MakeWeatherJson(){
}
/**
 * @brief 刷新时间的信息
 * 
 */
void FlushTimeScrTask(){
  time_t now = 0;
  time(&now);
  localtime_r(&now, &timeinfo);
  SetTimeSrc();
}


void FlushManGifTask(){
  SetManGifSrc();
}

void WeatherPlay(){

    WeatherAppInit();
    xTaskCreatePinnedToCore(GetWeatherTextTask,"GetWeatherTextTask",4096*2,NULL,4,&GetWeather,APP_CPU_NUM);
    
}




/***************************************时钟sntp**************************************************/


void GetnetworkTime(){
  setenv("TZ", "CST-8", 1);
  tzset();
  sntp_init();
  while(sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET)
  {
      vTaskDelay(((50)/ portTICK_PERIOD_MS));
  }
  sntp_stop();
  ESP_LOGI(TAG,"获取时间成功");
}

/***************************************天气调用接口************************************/


static int WeatherInit(AppController *sys)
{
  WeatherPlay();
  return 0;
}

static void WeatherProcess(AppController *sys, const ImuAction *act_info)
{
  
    if (act_info->action == BACK)
    {
        sys->AppExit();
        return;
    }
    if(get_weather_flag == true){
      SetWeatherSrc(Local_name);
      get_weather_flag = false;
    }
    FlushTimeScrTask();
    FlushManGifTask();
    vTaskDelay((10)/ portTICK_PERIOD_MS);

} 

static void WeatherBackground(AppController *sys, const ImuAction *act_info)
{
  
}

static int WeatherExit(void *param)
{
  vTaskDelete(GetWeather);
  httpclient.del();
  WeatherUIDel();
  return 0;
}

APPOBJ WeatherApp = {WEATHER_APP_NAME, &WeatherIco, "",
                    WeatherInit,WeatherProcess,WeatherBackground,
                    WeatherExit};


