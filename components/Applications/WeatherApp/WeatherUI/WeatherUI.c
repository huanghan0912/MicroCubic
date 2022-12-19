#include "WeatherUI.h"
#include "Weather_image.h"
#include "time.h"
#include "WeatherApp.h"
static lv_obj_t *scr_1 = NULL;


static lv_style_t default_style;  //默认背景
static lv_style_t chFont_style;  //中文字体
static lv_style_t numberSmall_style; //小数字字体
static lv_style_t numberBig_style; //大数字字体
static lv_style_t btn_style; //按钮样式
static lv_style_t bar_style; //进度条样式

static lv_obj_t *weatherImg = NULL;
static lv_obj_t *cityLabel = NULL;  //城市
static lv_obj_t *btn = NULL, *btnLabel = NULL; //天气空气质量
static lv_obj_t *txtLabel = NULL;
static lv_obj_t *clockLabel_1 = NULL, *clockLabel_2 = NULL;
static lv_obj_t *dateLabel = NULL;
static lv_obj_t *tempImg = NULL, *tempBar = NULL, *tempLabel = NULL;
static lv_obj_t *humiImg = NULL, *humiBar = NULL, *humiLabel = NULL;
static lv_obj_t *spaceImg = NULL;

static int ManNumber=-1;
// 天气图标路径的映射关系
const void *weaImage_map[] = {&weather_0, &weather_9, &weather_14, &weather_5, &weather_25,
                              &weather_30, &weather_26, &weather_11, &weather_23};
// 太空人图标路径的映射关系
const void *manImage_map[] = {&man_0, &man_1, &man_2, &man_3, &man_4, &man_5, &man_6, &man_7, &man_8, &man_9};
static const char weekDayCh[7][4] = {"日", "一", "二", "三", "四", "五", "六"};
static const char airQualityCh[6][10] = {"优", "良", "轻度", "中度", "重度", "严重"};


extern struct WeatherText Weather_text;
extern struct tm timeinfo;


void WeatherUIInit()
{

    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));

    lv_style_init(&chFont_style);
    lv_style_set_text_opa(&chFont_style, LV_OPA_COVER);
    lv_style_set_text_color(&chFont_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&chFont_style, &ch_font20);

    lv_style_init(&numberSmall_style);
    lv_style_set_text_opa(&numberSmall_style, LV_OPA_COVER);
    lv_style_set_text_color(&numberSmall_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&numberSmall_style, &lv_font_ibmplex_64);

    lv_style_init(&numberBig_style);
    lv_style_set_text_opa(&numberBig_style, LV_OPA_COVER);
    lv_style_set_text_color(&numberBig_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&numberBig_style, &lv_font_ibmplex_115);

    lv_style_init(&btn_style);
    lv_style_set_border_width(&btn_style, 0);

    lv_style_init(&bar_style);
    lv_style_set_bg_color(&bar_style, lv_color_hex(0x000000));
    lv_style_set_border_width(&bar_style, 2);
    lv_style_set_border_color(&bar_style, lv_color_hex(0xFFFFFF));
    lv_style_set_pad_top(&bar_style, 1); // 指示器到背景四周的距离
    lv_style_set_pad_bottom(&bar_style, 1);
    lv_style_set_pad_left(&bar_style, 1);
    lv_style_set_pad_right(&bar_style, 1);


    scr_1 = lv_obj_create(NULL);
    lv_obj_add_style(scr_1, &default_style, LV_STATE_DEFAULT);

    weatherImg = lv_img_create(scr_1);
    lv_img_set_src(weatherImg, weaImage_map[3]);
    lv_obj_align(weatherImg, LV_ALIGN_TOP_RIGHT, -10, 10);


    cityLabel = lv_label_create(scr_1);
    lv_obj_add_style(cityLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(cityLabel, true);
    lv_label_set_text(cityLabel, "未知");
    lv_obj_align(cityLabel, LV_ALIGN_TOP_LEFT, 20, 15);

    btn = lv_btn_create(scr_1);
    lv_obj_add_style(btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_set_pos(btn, 75, 15);
    lv_obj_set_size(btn, 50, 25);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_ORANGE), LV_STATE_DEFAULT);

    btnLabel = lv_label_create(btn);
    lv_obj_add_style(btnLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_obj_align(btnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btnLabel, airQualityCh[0]);

    txtLabel = lv_label_create(scr_1);
    lv_obj_add_style(txtLabel, &chFont_style, LV_STATE_DEFAULT);
    // lvgl8之前版本，模式一旦设置 LV_LABEL_LONG_SCROLL_CIRCULAR
    // 宽度恒定等于当前文本的长度，所以下面先设置以下长度
    lv_label_set_text(txtLabel, "最低气温12°C, ");
    lv_obj_set_size(txtLabel, 120, 30);
    lv_label_set_long_mode(txtLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text_fmt(txtLabel, "%s ","not connect wifi");
    lv_obj_align(txtLabel, LV_ALIGN_TOP_LEFT, 10, 50);

    clockLabel_1 = lv_label_create(scr_1);
    lv_obj_add_style(clockLabel_1, &numberBig_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(clockLabel_1, true);
    lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", 5, 20);
    lv_obj_align(clockLabel_1, LV_ALIGN_LEFT_MID, 0, 10);

    clockLabel_2 = lv_label_create(scr_1);
    lv_obj_add_style(clockLabel_2, &numberSmall_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(clockLabel_2, true);
    lv_label_set_text_fmt(clockLabel_2, "%02d", 00);
    lv_obj_align(clockLabel_2, LV_ALIGN_LEFT_MID, 165, 9);

    dateLabel = lv_label_create(scr_1);
    lv_obj_add_style(dateLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", 2, 30, weekDayCh[1]);
    lv_obj_align(dateLabel, LV_ALIGN_LEFT_MID, 10, 32);

    tempImg = lv_img_create(scr_1);
    lv_img_set_src(tempImg, &temp);
    lv_img_set_zoom(tempImg, 180);
    lv_obj_align(tempImg, LV_ALIGN_LEFT_MID, 10, 70);

    tempBar = lv_bar_create(scr_1);
    lv_obj_add_style(tempBar, &bar_style, LV_STATE_DEFAULT);
    lv_bar_set_range(tempBar, -50, 50); // 设置进度条表示的温度为-50~50
    lv_obj_set_size(tempBar, 60, 12);
    lv_obj_set_style_bg_color(tempBar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_bar_set_value(tempBar, 10, LV_ANIM_ON);
    lv_obj_align(tempBar, LV_ALIGN_LEFT_MID, 35, 70);

    tempLabel = lv_label_create(scr_1);
    lv_obj_add_style(tempLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(tempLabel, "%2d°C", 18);
    lv_obj_align(tempLabel, LV_ALIGN_LEFT_MID, 103, 70);

    humiImg = lv_img_create(scr_1);
    lv_img_set_src(humiImg, &humi);
    lv_img_set_zoom(humiImg, 180);
    lv_obj_align(humiImg, LV_ALIGN_LEFT_MID, 0, 100);

    humiBar = lv_bar_create(scr_1);
    lv_obj_add_style(humiBar, &bar_style, LV_STATE_DEFAULT);
    lv_bar_set_range(humiBar, 0, 100);
    lv_obj_set_size(humiBar, 60, 12);
    lv_obj_set_style_bg_color(humiBar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_bar_set_value(humiBar, 49, LV_ANIM_ON);
    lv_obj_align(humiBar, LV_ALIGN_LEFT_MID, 35, 100);


    humiLabel = lv_label_create(scr_1);
    lv_obj_add_style(humiLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text(humiLabel, "50%");
    lv_obj_align(humiLabel, LV_ALIGN_LEFT_MID, 103, 100);


    // 太空人图标
    spaceImg = lv_img_create(scr_1);
    lv_img_set_src(spaceImg, manImage_map[0]);
    lv_obj_align(spaceImg, LV_ALIGN_BOTTOM_RIGHT, -10, -10);



    lv_scr_load(scr_1);
}

void SetTimeSrc(){
     lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#",timeinfo.tm_hour, timeinfo.tm_min);
     lv_label_set_text_fmt(clockLabel_2, "%02d",timeinfo.tm_sec );
     lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", timeinfo.tm_mon, timeinfo.tm_mday, weekDayCh[timeinfo.tm_wday]);
}

void SetWeatherSrc(char *Local_name){

    lv_img_set_src(weatherImg, weaImage_map[3]);
    //地区
    lv_label_set_text(cityLabel, Local_name);
    lv_label_set_text_fmt(txtLabel, "今日风 %s, 等级为%s级, 风速%sM/S",Weather_text.wind,Weather_text.windlevel,Weather_text.windSpeed);

    lv_bar_set_value(tempBar,Weather_text.temp, LV_ANIM_ON);
    lv_label_set_text_fmt(tempLabel, "%2d°C", Weather_text.temp);

    lv_bar_set_value(humiBar, Weather_text.humidity, LV_ANIM_ON);
    lv_label_set_text_fmt(humiLabel, "%d",Weather_text.humidity);
}


void SetManGifSrc(){
    if (ManNumber==9) ManNumber=1;
    ManNumber++;
    lv_img_set_src(spaceImg, manImage_map[ManNumber]);
    
}