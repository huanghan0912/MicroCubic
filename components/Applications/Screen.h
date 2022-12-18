#ifndef _Screen_H_
#define _Screen_H_

#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl_helpers.h"


#define LCD_W 240
#define LCD_H 240

class Screen
{
private:
    //lvgl定时器回调创建
    void _lv_timer_create(void);
public:

    void Init();

};

    //lvgl定时器回调
    static void lv_timer_cb(void *arg);
    //lvgl初始化显示屏幕驱动
    void lv_port_disp_init();


#endif