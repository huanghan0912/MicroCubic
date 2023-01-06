#include "WifiAppUI.h"
#include "Wifi.h"

static lv_obj_t *scr = NULL;

static lv_style_t default_style;  //默认背景
static lv_style_t enfont_style; //英文字体
static lv_obj_t *statusLabel = NULL;
static lv_obj_t *textLabel = NULL;
static lv_obj_t *wifiLabel = NULL;


void WifiUIInit()
{
    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));

    lv_style_init(&enfont_style);
    lv_style_set_text_opa(&enfont_style, LV_OPA_COVER);
    lv_style_set_text_color(&enfont_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&enfont_style, &lv_font_montserrat_18);
}

void WifiUIScrinit()
{

    lv_obj_t *act_obj = lv_scr_act(); // 获取当前活动页
    if (act_obj == scr)
        return;

    WifiUIDel();
    lv_obj_clean(act_obj);


    scr = lv_obj_create(NULL);
    lv_obj_add_style(scr, &default_style, LV_STATE_DEFAULT);

    statusLabel = lv_label_create(scr);
    lv_obj_add_style(statusLabel, &enfont_style, LV_STATE_DEFAULT);
    lv_obj_set_size(statusLabel, 180, 30);
    lv_label_set_text_fmt(statusLabel, "%s ","wifi checking...");
    lv_obj_align(statusLabel, LV_ALIGN_CENTER, 0, -50);


    textLabel = lv_label_create(scr);
    lv_obj_add_style(textLabel, &enfont_style, LV_STATE_DEFAULT);
    lv_obj_set_size(textLabel, 200, 60);
    lv_label_set_text_fmt(textLabel, "%s", "wifi_ssid: MircoCubic\nwifi_pass: 12345678\nip: 192.168.4.1");
    lv_obj_align_to(textLabel, statusLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);


    wifiLabel = lv_label_create(scr);
    lv_obj_add_style(wifiLabel, &enfont_style, LV_STATE_DEFAULT);
    lv_obj_set_size(wifiLabel, 200, 40);
    lv_label_set_text_fmt(wifiLabel, "your_ssid: %s\nyour_pass: %s",wifi_ssid,wifi_password);
    lv_obj_align_to(wifiLabel, textLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);


    lv_scr_load(scr);

}

void SetTextSrc(char *text)
{
    WifiUIScrinit();
    lv_label_set_text_fmt(statusLabel, "%s ",text);
    lv_label_set_text_fmt(wifiLabel, "your_ssid: %s\nyour_pass: %s",wifi_ssid,wifi_password);
}


void WifiUIDel()
{
    if (scr != NULL)
    {
        lv_obj_clean(scr);
    }
}

