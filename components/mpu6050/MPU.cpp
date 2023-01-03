#include "MPU.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";
void Mpu::Init()
{
    action_info.is_valid = false;
    action_info.action = UNKNOWN;
    action_info.long_time = true;

    i2cdev.initialize();
    mpu6050 = MPU6050(0x68);

    if (!mpu6050.testConnection())
    {
        ESP_LOGW(TAG,"Unable to connect to MPU6050.\n");
        return;
    }
    mpu6050.initialize();

    ESP_LOGI(TAG,"Initialization MPU6050 success.");



}

ImuAction *Mpu::GetAction()
{
    //每次的陀螺仪数据进行记录（5数组），用于判断长按还是短按
    ImuAction tmp_info;
    tmp_info.action = UNKNOWN;
    getVirtureMotion6(&tmp_info);
    //判断动作
    if (tmp_info.action == UNKNOWN)
    {
        if (tmp_info.ay > 4000)
            tmp_info.action = LEFT;
        else if (tmp_info.ay < -4000)
            tmp_info.action = RIGHT;
        // else if (tmp_info.ay > 1000 || tmp_info.ay < -1000)
        //     // 震动检测
        //     tmp_info.action = SHAKE;
    }

    if (tmp_info.action == UNKNOWN)
    {
        if (tmp_info.ax > 5000)
            tmp_info.action = UP;
        else if (tmp_info.ax < -5000)
            tmp_info.action = DOWN;
        // else if (action_info.ax > 1000 || action_info.ax < -1000)
        //     tmp_info.action =SHAKE;
    }
    //储存当前的动作数据
    act_info_history_index=(act_info_history_index+1) % ACTION_HISTORY_BUF_LEN;//下标加1
    int index = act_info_history_index;
    action_history[index] = tmp_info.action;

    //本次流程的动作识别
    if(!action_info.is_valid)
    {
        bool is_holdon = false; //是否长按
        int second =(index + ACTION_HISTORY_BUF_LEN - 1) % ACTION_HISTORY_BUF_LEN;
        int third = (index + ACTION_HISTORY_BUF_LEN - 2) % ACTION_HISTORY_BUF_LEN;

        if (tmp_info.action!=UNKNOWN )
        {
            action_info.is_valid = 1;
            action_info.action = tmp_info.action;
        }
        // printf("%d %d %d\n",action_history[index],action_history[second],action_history[third]);
        if (action_history[index] == action_history[second] && action_history[second] == action_history[third])
        {
            // 目前只识别前后的长按
            if (tmp_info.action == UP)
            {
                is_holdon = true;
                action_info.is_valid = 1;
                action_info.action =GO_FORWORD;
            }
            else if (tmp_info.action ==DOWN)
            {
                is_holdon = true;
                action_info.is_valid = 1;
                action_info.action = BACK;
            }
            // 如需左右的长按可在此处添加"else if"的逻辑

            if (is_holdon)
            {
                // 本次识别为长按，则手动清除识别过的历史数据 避免对下次动作识别的影响
                action_history[second] = UNKNOWN;
                action_history[third] = UNKNOWN;
            }
        }

    }

    return &action_info;
}
/**
 * @brief 获取陀螺仪的角度
 * 
 * @param action_info Mpu动作信息
 */

void Mpu::getVirtureMotion6(ImuAction *action_info)
{
    mpu6050.getMotion6(&(action_info->ax), &(action_info->ay),
                   &(action_info->az), &(action_info->gx),
                   &(action_info->gy), &(action_info->gz));
    // printf("gx = %d\tgy = %d\tgz = %d", action_info->gx, action_info->gy, action_info->gz);
    // printf("\tax = %d\tay = %d\taz = %d\n", action_info->ax, action_info->ay, action_info->az);

}