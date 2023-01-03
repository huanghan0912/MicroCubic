#include "Screen.h"




static const  char *TAG="Screen";
static lv_color_t buf1[LCD_W*LCD_H/10];
static lv_color_t buf2[LCD_W*LCD_H/10];

static esp_timer_handle_t lvgl_timer_handle = NULL;

static esp_timer_create_args_t lvgl_timer = {
    .callback = &lv_timer_cb,
    .arg = NULL,
    .dispatch_method = ESP_TIMER_TASK, 
    .name ="lvgl_timer"
};


/**
 * @brief Initialize the display
 */
void Screen::Init(){
    lv_init();		
    lvgl_driver_init();
    lv_port_disp_init();
    _lv_timer_create();
    // xTaskCreatePinnedToCore(lvglTask,"lvgl_task",4096*2,NULL,7,NULL,APP_CPU_NUM);

}

static IRAM_ATTR void lv_timer_cb(void *arg)
{
   lv_tick_inc(1);
}

void Screen::_lv_timer_create(void)
{
   esp_err_t err = esp_timer_create(&lvgl_timer, &lvgl_timer_handle);
   err = esp_timer_start_periodic(lvgl_timer_handle, 1000 );//1毫秒回调
   if(err!=ESP_OK){
      ESP_LOGE(TAG,"lvgl定时器创建成功\r\n");
   }

}



/**
 * @brief Initialize the lvgl
 */
void lv_port_disp_init()
{

    static lv_disp_draw_buf_t disp_buf;                         
   lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_H*10);

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);   

    disp_drv.hor_res = LCD_W;
    disp_drv.ver_res = LCD_H;

    /*Set up the functions to access to your display*/
    disp_drv.flush_cb = disp_driver_flush;   
    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;       
 

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);

}



/**
 * @brief Initialize the input device
 * 
 */
// void lv_port_indev_init()
// {
//     
//     static lv_indev_drv_t indev_drv;
// 	lv_indev_drv_init( &indev_drv );
// 	indev_drv.type = LV_INDEV_TYPE_ENCODER;
// 	indev_drv.read_cb = MyMpuRead;
// 	lv_indev_drv_register( &indev_drv );

// }

void lvglTask(void* Parameter){
    while(1)
    {
        vTaskDelay(20 / portTICK_PERIOD_MS);
        lv_task_handler();
    }
} 