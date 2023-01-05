/**
 * @file AppController.h
 * @author huanghan0912 (2235812260@qq.com)
 * @brief app控制器，负责app的初始化，启动，退出等操作
 * @version 0.1
 * @date 2022-12-26
 * 
 * 
 */
#ifndef _APPCONTROLLER_H_
#define _APPCONTROLLER_H_


#include "Screen.h"
#include "Sd.h"
#include "Wifi.h"

#include "lvgl.h"
#include "AppControllerType.h"
#include "MPU.h"

#define APP_MAX_NUM 20
#define APP_CONTROLLER_NAME_LEN 16



class AppController
{
private:    
    Screen screen;
    Sd sdCard;

    APPOBJ *appList[APP_MAX_NUM];      // 预留APP_MAX_NUM个APP注册位
    APPTYPE appTypeList[APP_MAX_NUM];  // 对应APP的运行类型
    
    unsigned int app_num;

    int cur_app_index;     // 当前运行的APP下标
    int pre_app_index;     // 上一次运行的APP下标
    bool app_exit_flag; // 表示是否退出APP应用
private:
    int AppIsLegal(const APPOBJ *app_obj);
public:
    void Init();
    void Display();
    int AppInstall(APPOBJ *app, APPTYPE app_type = APP_TYPE_REAL_TIME);
    int MainProcess(ImuAction *act_info);
    void AppExit();


    void text();
};







#endif