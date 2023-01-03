#ifndef _APPCONTROLLERUI_H_
#define _APPCONTROLLERUI_H_



#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

extern const lv_img_dsc_t app_loading;

#define ANIEND_WAIT                 \
    while (lv_anim_count_running()) \
        lv_task_handler(); // 等待动画完成

void AppControllerUIInit(void);
void AppControllerUIRelease(void);
void DisplayAppScrInit(const void *src_img, const char *app_name);
void DisplayAppScrRelease(void);
void AppControlDisplayScr(const void *src_img, const char *app_name,
                                 lv_scr_load_anim_t anim_type, bool force);



#ifdef __cplusplus
} 
#endif

#endif