#ifndef _MPU_H_
#define _MPU_H_
#include <MPU6050.h>
#include <I2Cdev.h>



//历史动作缓存长度
#define ACTION_HISTORY_BUF_LEN 5

/**
 * @brief Mpu活动类型
 * 
 */
 enum ACTIVETYPE
{ 
    RIGHT = 0,
    LEFT,
    UP,
    DOWN,
    GO_FORWORD,
    BACK,
    SHAKE,
    UNKNOWN 
};

/**
 * @brief Mpu动作信息
 * 
 */

 struct ImuAction{
    volatile ACTIVETYPE action;
    bool is_valid; //是否有效
    bool long_time;
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
};




class Mpu
{
private: 
    MPU6050 mpu6050;
    I2Cdev i2cdev;
    //历史动作缓存
    ACTIVETYPE action_history[ACTION_HISTORY_BUF_LEN];
    //历史动作缓存下标
    int act_info_history_index=0;
    ImuAction action_info;
public:
    void Init();
    ImuAction *GetAction(void);
    //获取陀螺仪的角度
    void getVirtureMotion6(ImuAction *action_info);
};




#endif