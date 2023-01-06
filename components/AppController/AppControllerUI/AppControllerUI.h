#ifndef _APPCONTROLLERUI_H_
#define _APPCONTROLLERUI_H_



#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

extern const lv_img_dsc_t app_loading;


void AppControllerUIInit(void);
void AppcontrollerUIScrInit();
void AppControllerUIRelease(void);

void AppControlDisplayScr(const void *src_img, const char *app_name);



#ifdef __cplusplus
} 
#endif

#endif