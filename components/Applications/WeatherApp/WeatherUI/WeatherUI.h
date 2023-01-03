#ifndef _WEATHERUI_H_
#define _WEATHERUI_H_


#ifdef __cplusplus
extern "C"
{
#endif


#include "lvgl.h"


// struct tm timeinfo ;

LV_FONT_DECLARE(lv_font_ibmplex_115);
LV_FONT_DECLARE(lv_font_ibmplex_64);
LV_FONT_DECLARE(ch_font20);


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



void WeatherUIInit();
//设置时钟的数据
void SetTimeSrc();
//设置小人动画
void SetManGifSrc();
//设置天气的数据
void SetWeatherSrc(char *Local_name);

void WeatherUIDel();


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif