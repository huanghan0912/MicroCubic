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



void WeatherUIInit();





#ifdef __cplusplus
} /* extern "C" */
#endif


#endif