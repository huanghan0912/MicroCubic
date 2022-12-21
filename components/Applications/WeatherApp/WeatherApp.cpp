#include "WeatherApp.h"
#include "HttpClient.h"
#include "WeatherUI.h"
#include "esp_log.h"
#include "cJSON.h"
#include <time.h>
#include "sntp.h"



struct tm timeinfo ;
struct WeatherText Weather_text;


HttpClient httpclient;
static const char *TAG="WeatherApp";

char Weather_url[150];  //调用天气的Url
char Local_name[8]="合肥";  //所在地名称
char Local_code[15]="101220101";    //所在地代码
char key[40]="3af5fbac7e89420188958bd85c0fb740";    //调用天气的秘钥
char json_root[450];


void WeatherAppInit()
{
  sprintf(Weather_url,"https://devapi.qweather.com/v7/weather/now?location=%s&key=%s&gzip=n",Local_code,key);
  ESP_LOGI(TAG,"%s",Weather_url);
  httpclient.Init(Weather_url);
  WeatherUIInit();//显示初始化
}

void GetWeatherTextTask(void* Parameter)
{
  while(1){
    GetnetworkTime();
    vTaskDelay(((2000)/ portTICK_PERIOD_MS));
    httpclient.GetResponse(json_root);
    MakeWeatherJson();
    vTaskDelay(((1000*60*60*2)/ portTICK_PERIOD_MS));
  }
  
}
void GetWeatherText(){
    httpclient.GetResponse(json_root);
    MakeWeatherJson();
}

void MakeWeatherJson(){
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
    SetWeatherSrc(Local_name);
    ESP_LOGI(TAG,"获取天气成功");
  }
  else{
    ESP_LOGW(TAG,"%s",check->valuestring);
  }
}
/**
 * @brief 刷新时间的信息
 * 
 */
void FlushTimeScrTask(void* Parameter){
  while(1){
    time_t now = 0;
    time(&now);
    localtime_r(&now, &timeinfo);
     SetTimeSrc();
     vTaskDelay((250)/ portTICK_PERIOD_MS);
  }
}


void FlushManGifTask(void* Parameter){
  while(1){
     SetManGifSrc();
     vTaskDelay(((50)/ portTICK_PERIOD_MS));
  }
}

void WeatherPlay(){

    WeatherAppInit();
    xTaskCreatePinnedToCore(FlushManGifTask,"FlushManGifTask",4096*2,NULL,3,NULL,APP_CPU_NUM);
    xTaskCreatePinnedToCore(FlushTimeScrTask,"FlushWeatherScrTask",4096*2,NULL,2,NULL,APP_CPU_NUM);
    xTaskCreatePinnedToCore(GetWeatherTextTask,"GetWeatherTextTask",4096*2,NULL,4,NULL,APP_CPU_NUM);
    
}




/***************************************时钟sntp**************************************************/




  


void GetnetworkTime(){
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "cn.ntp.org.cn");
  sntp_setservername(1, "ntp1.aliyun.com");

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