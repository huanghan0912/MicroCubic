#include "AppControllerUI.h"

LV_FONT_DECLARE(lv_font_montserrat_24);

static lv_obj_t *app_scr = NULL;//当前显示屏幕
static lv_obj_t *app_scr_t = NULL;
static lv_obj_t *now_app_image = NULL;
static lv_obj_t *now_app_name = NULL;
const void *pre_img_path = NULL;


static lv_style_t default_style;
static lv_style_t app_name_style;

void  AppControllerUIInit(){
    if (app_scr != NULL)
    {
        lv_obj_clean(app_scr);
        app_scr = NULL;
    }

    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));
    lv_style_set_radius(&default_style, 0); // 设置控件圆角半径
    // 设置边框宽度
    lv_style_set_border_width(&default_style, 0);

    lv_style_init(&app_name_style);
    lv_style_set_text_opa(&app_name_style, LV_OPA_COVER);
    lv_style_set_text_color(&app_name_style, lv_color_white());
    lv_style_set_text_font(&app_name_style, &lv_font_montserrat_24);                 
}

void AppcontrollerUIScrInit(){
    lv_obj_t *act_obj = lv_scr_act(); // 获取当前活动页
    if (act_obj== app_scr)
    {
        return;
    }
    AppControllerUIRelease();
    lv_obj_clean(act_obj);

     // APP图标页
    app_scr = lv_obj_create(NULL);
    lv_obj_add_style(app_scr, &default_style, LV_STATE_DEFAULT);
    // 设置不显示滚动条
    lv_obj_set_style_bg_opa(app_scr, LV_OPA_0,
                            LV_PART_SCROLLBAR | LV_STATE_DEFAULT);


    now_app_image = lv_img_create(app_scr);
    lv_img_set_src(now_app_image,&app_loading);
    lv_obj_align(now_app_image, LV_ALIGN_CENTER, 0, 0);

    // 添加APP的名字
    now_app_name = lv_label_create(app_scr);
    lv_obj_add_style(now_app_name, &app_name_style, LV_STATE_DEFAULT);
    // lv_label_set_recolor(now_app_name, true); //先得使能文本重绘色功能
    lv_label_set_text(now_app_name, "Loading...");
    lv_obj_align_to(now_app_name, now_app_image, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_scr_load(app_scr);        
}

void AppControllerUIRelease(){
    if (NULL != app_scr)
    {
        lv_obj_clean(app_scr);
        app_scr = NULL;
    }
}


void AppControlDisplayScr(const void *src_img, const char *app_name){
    AppcontrollerUIScrInit();
    lv_label_set_text(now_app_name, app_name);
    lv_img_set_src(now_app_image,src_img);
}



