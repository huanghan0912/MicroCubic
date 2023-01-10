#ifndef _WIFIAPPUI_H_
#define _WIFIAPPUI_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include "lvgl.h"

void WifiUIInit();

void WifiUIScrinit();

void SetWifiTextSrc(char *text);

void WifiUIDel();



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif