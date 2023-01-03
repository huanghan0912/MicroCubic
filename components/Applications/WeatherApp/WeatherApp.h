#ifndef _WEATHERAPP_H_
#define _WEATHERAPP_H_



#include "AppControllerType.h"

extern APPOBJ WeatherApp;







void WeatherAppInit();

//从网络上获取天气和时钟 任务
void GetWeatherTextTask(void* Parameter);



void  GetnetworkTime();
//刷新时钟界面的信息任务
void FlushTimeScrTask(void* Parameter);
//刷新动画的界面任务
void FlushManGifTask(void* Parameter);

void WeatherPlay();





#endif