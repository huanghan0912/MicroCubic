#include "Game2048App.h"
#include "esp_random.h"
#include <iostream>
#include "Game2048Controller.h"
#include "Game2048AppUI.h"
#include "Screen.h"
#include "MPU.h"
#include "AppController.h"
#define Game2048_APP_NAME "2048"


struct Game2048AppRunData
{
    int Normal = 0;       // 记录移动的方向
    int BornLocation = 0; // 记录新棋子的位置
    int *pBoard;
    int *moveRecord;
    BaseType_t xReturned_lvtask = pdFALSE;
    TaskHandle_t xHandle_lvtask = NULL;
};


GAME2048 game;
static Game2048AppRunData *run_data = NULL;


static void lvtask(void *parameter)
{
    while (1)
    {
        // LVGL任务主函数
        vTaskDelay(5 / portTICK_PERIOD_MS);
        LVGL_OPERATE_LOCK(lv_task_handler();)
    }
    vTaskDelete(NULL);
}


static int Game2048Init(AppController *sys)
{


    Game2048UIInit();
    run_data = (Game2048AppRunData *)malloc(1*sizeof(Game2048AppRunData));
    game.init();
    run_data->pBoard = game.getBoard();
    run_data->moveRecord = game.getMoveRecord();
    
    int new1 = game.addRandom();
    int new2 = game.addRandom();

    run_data->xReturned_lvtask = xTaskCreate(
        lvtask,                      /*任务函数*/
        "lvtask",                    /*带任务名称的字符串*/
        8 * 1024,                     /*堆栈大小，单位为字节*/
        NULL,                         /*作为任务输入传递的参数*/
        1,                            /*任务的优先级*/
        &run_data->xHandle_lvtask); /*任务句柄*/


    vTaskDelay(100 / portTICK_PERIOD_MS);
    LVGL_OPERATE_LOCK(showBoard(run_data->pBoard);)
    // 棋子出生动画
    LVGL_OPERATE_LOCK(showNewBorn(game.addRandom(), run_data->pBoard);)
	LVGL_OPERATE_LOCK(showNewBorn(game.addRandom(), run_data->pBoard);)
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return 0;
}
static void Game2048Process(AppController *sys,
                              const ImuAction *act_info)
{
    if (act_info->action == BACK)
    {
        sys->AppExit(); // 退出APP
        return;
    }

    // 具体操作
    if (act_info->action == RIGHT )
    {
        game.moveRight();
        if (game.comparePre() == 0)
        {
            LVGL_OPERATE_LOCK(showAnim(run_data->moveRecord, 4);)
            vTaskDelay(700 / portTICK_PERIOD_MS);
            LVGL_OPERATE_LOCK(showNewBorn(game.addRandom(), run_data->pBoard);)
        }
    }
    else if (act_info->action == LEFT)
    {
        game.moveLeft();
        if (game.comparePre() == 0)
        {
            LVGL_OPERATE_LOCK(showAnim(run_data->moveRecord, 3);)
            vTaskDelay(700 / portTICK_PERIOD_MS);
            LVGL_OPERATE_LOCK(showNewBorn(game.addRandom(), run_data->pBoard);)
        }
    }
    else if (act_info->action == UP )
    {
        game.moveUp();
        if (game.comparePre() == 0)
        {
            LVGL_OPERATE_LOCK(showAnim(run_data->moveRecord, 1);)
            vTaskDelay(700 / portTICK_PERIOD_MS);
            LVGL_OPERATE_LOCK(showNewBorn(game.addRandom(), run_data->pBoard);)
        }
    }
    else if ( act_info->action == DOWN)
    {
        game.moveDown();
        if (game.comparePre() == 0)
        {
            LVGL_OPERATE_LOCK(showAnim(run_data->moveRecord, 2);)
            vTaskDelay(700 / portTICK_PERIOD_MS);
            LVGL_OPERATE_LOCK(showNewBorn(game.addRandom(), run_data->pBoard);)
        }
    }

    if (game.judge() == 1)
    {
        //   rgb.setRGB(0, 255, 0);
    
    }
    else if (game.judge() == 2)
    {
        //   rgb.setRGB(255, 0, 0);
    }

    // 程序需要时可以适当加延时
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

static void Game2048Background(AppController *sys,
                                      const ImuAction *act_info)
{
    // 本函数为后台任务，主控制器会间隔一分钟调用此函数
    // 本函数尽量只调用"常驻数据",其他变量可能会因为生命周期的缘故已经释放
}

static int Game2048WeatherExit(void *param)
{
    // 查杀任务
    if (run_data->xReturned_lvtask == pdPASS)
    {
        vTaskDelete(run_data->xHandle_lvtask);
    }

    xSemaphoreGive(lvgl_mutex);

    Game2048UIdel();

    // 释放运行数据
    if (NULL != run_data)
    {
        free(run_data);
        run_data = NULL;
    }
    return 0;
}

APPOBJ Game2048App = {Game2048_APP_NAME, &app_game_2048, "",
                       Game2048Init,Game2048Process,Game2048Background,
                      Game2048WeatherExit
                        };
