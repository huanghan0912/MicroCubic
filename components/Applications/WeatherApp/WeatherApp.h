#ifndef _WEATHERAPP_H_
#define _WEATHERAPP_H_

#ifdef __cplusplus
extern "C"
{
#endif

struct WeatherText
{
    int temp; //温度
    int icon;  //对应图标
    char weather[12]; //天气
    char wind[12];   //风向
    char windlevel[2]; //风速等级
    char windSpeed[3];  //风速
    int humidity;   //相对湿度
};



void WeatherAppInit();

//从网络上获取天气和时钟 任务
void GetWeatherTextTask(void* Parameter);

void MakeWeatherJson();


void  GetnetworkTime();
//刷新时钟界面的信息任务
void FlushTimeScrTask(void* Parameter);
//刷新动画的界面任务
void FlushManGifTask(void* Parameter);

void WeatherPlay();
void GetWeatherText();

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif