#ifndef _WEATHERAPP_H_
#define _WEATHERAPP_H_



#include "AppControllerType.h"

extern APPOBJ WeatherApp;







void WeatherAppInit();

//从互联网上获取界面上所需的所有信息
void GetWeatherTextTask(void* Parameter);



void  GetnetworkTime();
//刷新时钟界面的信息任务
void FlushTimeScrTask();
//刷新动画的界面任务
void FlushManGifTask();

void WeatherPlay();





#endif