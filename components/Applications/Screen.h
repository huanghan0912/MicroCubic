#ifndef _Screen_H_
#define _Screen_H_

#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl_helpers.h"



#define LCD_W 240
#define LCD_H 240


// lvgl 操作的锁
extern SemaphoreHandle_t lvgl_mutex;

// LVGL操作的安全宏（避免脏数据）
#define LVGL_OPERATE_LOCK(CODE)                          \
    if (pdTRUE == xSemaphoreTake(lvgl_mutex, portMAX_DELAY)) \
    {                                                        \
        CODE                                                 \
        xSemaphoreGive(lvgl_mutex);                          \
    }




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
    void lvglTask(void* Parameter);
    //输入设备初始化
    void lv_port_indev_init();

#endif