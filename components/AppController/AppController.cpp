#include "AppController.h"
#include "AppControllerUI.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"


void AppController::Init(){
    //初始化APP控制器变量
    app_num = 0;
    cur_app_index = 0;
    pre_app_index = 0;
    app_exit_flag = 0;



    wifi.Init();
    sdCard.Init();
    wifi.BeginSTA("304","22358122");
    screen.Init();
    AppControllerUIInit();

    appList[0] = new APPOBJ();
    appList[0]->app_image = &app_loading;
    appList[0]->app_name = "Loading...";
    appTypeList[0] = APP_TYPE_REAL_TIME;
    AppControlDisplayScr(appList[cur_app_index]->app_image,
                            appList[cur_app_index]->app_name,
                            LV_SCR_LOAD_ANIM_NONE, true);
}

void AppController::Display(){
    AppControlDisplayScr(appList[cur_app_index]->app_image,
                            appList[cur_app_index]->app_name,
                            LV_SCR_LOAD_ANIM_NONE, true);
}

int AppController::AppIsLegal(const APPOBJ *app_obj)
{
    // APP的合法性检测
    if (NULL == app_obj)
        return 1;
    if (APP_MAX_NUM <= app_num)
        return 2;
    return 0;
}

int AppController::AppInstall(APPOBJ *app, APPTYPE app_type)
{
    int ret_code = AppIsLegal(app);
    if (ret_code != 0)
    {
        return ret_code;
    }

    appList[app_num] = app;
    appTypeList[app_num] = app_type;
    ++app_num;
    return 0; // 安装成功
}

int AppController::MainProcess(ImuAction *act_info)
{
    if ( act_info->action != UNKNOWN)
    {
        printf("act_info->action: ");
        printf("%d\n", act_info->action);
    }

    if (app_exit_flag == 0)
    {
        // 当前没有进入任何app
        lv_scr_load_anim_t anim_type = LV_SCR_LOAD_ANIM_NONE;
        if (act_info->action == RIGHT)
        {
            anim_type = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
            pre_app_index = cur_app_index;
            cur_app_index = (cur_app_index + 1) % app_num;
        }
        else if (act_info->action == LEFT)
        {
            anim_type = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            pre_app_index = cur_app_index;
            // 以下等效与 processId = (processId - 1 + APP_NUM) % 4;
            // +3为了不让数据溢出成负数，而导致取模逻辑错误
            cur_app_index = (cur_app_index - 1 + app_num) % app_num; // 此处的3与p_processList的长度一致
        }
        else if (act_info->action == UP)
        {
            app_exit_flag = 1; // 进入app
            if ( appList[cur_app_index]->app_init != NULL)
            {
                (*(appList[cur_app_index]->app_init))(this); // 执行APP初始化
            }
        }
        
        if (act_info->action != UP) 
        {
            AppControlDisplayScr(appList[cur_app_index]->app_image,
                                    appList[cur_app_index]->app_name,
                                    anim_type, false);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
    else
    {
        // 运行APP进程 等效于把控制权交给当前APP
        (*(appList[cur_app_index]->main_process))(this, act_info);
    }
    act_info->action = UNKNOWN;
    act_info->is_valid = 0;
    return 0;
}


void AppController::AppExit()
{
    app_exit_flag = 0; // 退出APP
    if (NULL != appList[cur_app_index]->exit_callback) 
    {
        // 执行APP退出回调
        (*(appList[cur_app_index]->exit_callback))(this);
    }

    AppControlDisplayScr(appList[cur_app_index]->app_image,
                            appList[cur_app_index]->app_name,
                            LV_SCR_LOAD_ANIM_NONE, true);
}





