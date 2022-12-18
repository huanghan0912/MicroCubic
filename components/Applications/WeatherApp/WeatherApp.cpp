#include "WeatherApp.h"
#include "HttpClient.h"
#include "WeatherUI.h"
#include "esp_log.h"
#include "cJSON.h"
#include <time.h>
#include "lwip/apps/sntp.h"



struct WeatherText
{
    char temp[3]; //温度
    int icon;  //对应图标
    char weather[12]; //天气
    char wind[12];   //风向
    char windlevel[2]; //风速等级
    char windSpeed[3];  //风速
    char humidity[3];   //相对湿度
};
struct tm timeinfo ;
struct WeatherText Weather_text;


HttpClient httpclient;
static const char *TAG="WeatherApp";

char Weather_url[150];  //调用天气的Url
char Local_name[8]="北京";  //所在地名称
char Local_code[15]="101010100";    //所在地代码
char key[40]="3af5fbac7e89420188958bd85c0fb740";    //调用天气的秘钥
char json_root[450];
void WeatherAppInit()
{

  sprintf(Weather_url,"https://devapi.qweather.com/v7/weather/now?location=%s&key=%s&gzip=n",Local_code,key);
  ESP_LOGI(TAG,"%s",Weather_url);
  httpclient.Init(Weather_url);
  WeatherUIInit();//显示初始化
}

void GetWeatherText()
{
  httpclient.GetResponse(json_root);
  MakeWeatherJson();
}

void MakeWeatherJson(){
  cJSON* root =cJSON_Parse(json_root);


  cJSON* now =cJSON_GetObjectItem(root,"now");
  cJSON* cjson_temp = cJSON_GetObjectItem(now,"temp");
  cJSON* cjson_icon = cJSON_GetObjectItem(now,"icon");
  cJSON* cjson_weather = cJSON_GetObjectItem(now,"text");
  cJSON* cjson_wind = cJSON_GetObjectItem(now,"windDir");
  cJSON* cjson_windlevel = cJSON_GetObjectItem(now,"windScale");
  cJSON* cjson_windSpeed = cJSON_GetObjectItem(now,"windSpeed");
  cJSON* cjson_humidity = cJSON_GetObjectItem(now,"humidity");

  strcpy(Weather_text.temp,cjson_temp->valuestring);
  Weather_text.icon=atoi(cjson_icon->valuestring);
  strcpy(Weather_text.weather, cjson_weather->valuestring);
  strcpy(Weather_text.wind, cjson_wind->valuestring);
  strcpy(Weather_text.windlevel, cjson_windlevel->valuestring);
  strcpy(Weather_text.windSpeed,cjson_windSpeed->valuestring );
  strcpy(Weather_text.humidity, cjson_humidity->valuestring);

  ESP_LOGI(TAG,"%s   %d",Weather_text.weather,Weather_text.icon);

}


/***************************************时钟sntp**************************************************/



void  GetnetworkTimeInit(){
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "ntp.aliyun.com");
  sntp_setservername(1, "ntp1.aliyun.com");
  sntp_setservername(2, "cn.pool.ntp.org");
  setenv("TZ", "CST-8", 1);
  tzset();
}

void  GetnetworkTime(){
  sntp_init();
  time_t now = 0;
  time(&now);
  localtime_r(&now, &timeinfo);
  char str[64];
  strftime(str, sizeof(str), "%c", &timeinfo);
  ESP_LOGI(TAG,"%s",str);
  sntp_stop();
}