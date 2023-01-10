#ifndef _UPGRADEAPPUI_H_
#define _UPGRADEAPPUI_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include "lvgl.h"

void UpgradeUIInit();

void UpgradeUIScrinit();

void SetUpgradeTextSrc(char *statustext,char *remaintext);

void UpgradeUIDel();



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif