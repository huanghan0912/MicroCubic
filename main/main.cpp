#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "lvgl.h"
#include "Screen.h"
#include "Sd.h"
#include "Wifi.h"
#include "nvs_flash.h"
#include "esp_timer.h"

#include "mpu6050.h"




/***********************************全局变量***************************************/
//使用的Espidf_Study全局变量全都定义在此


Screen screen;
Sd sdCard;
Wifi wifi;


#define I2C_MASTER_SCL_IO 10     /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 9      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

static const char *TAG = "mpu6050 test";
static mpu6050_handle_t mpu6050 = NULL;

/**
 * @brief i2c master initialization
 */
void i2c_bus_init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    i2c_param_config(I2C_MASTER_NUM, &conf);
 

    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

}

/**
 * @brief i2c master initialization
 */
 void i2c_sensor_mpu6050_init(void)
{
    esp_err_t ret;

    i2c_bus_init();
    mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);

    ret = mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS);
  

    ret = mpu6050_wake_up(mpu6050);

}





extern "C" void app_main(void)
{

    // esp_err_t err = nvs_flash_init();
    // if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     err = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(err);
    // wifi.Init();
    // sdCard.Init();
    // wifi.BeginSTA("304","22358122");
    // screen.Init();
    esp_err_t ret;
    uint8_t mpu6050_deviceid;
    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;
    mpu6050_temp_value_t temp;

    i2c_sensor_mpu6050_init();

    ret = mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);

   while (1)
   {
    mpu6050_get_acce(mpu6050, &acce);
    ESP_LOGI(TAG, "acce_x:%.2f, acce_y:%.2f, acce_z:%.2f\n", acce.acce_x, acce.acce_y, acce.acce_z);

    mpu6050_get_gyro(mpu6050, &gyro);

    ESP_LOGI(TAG, "gyro_x:%.2f, gyro_y:%.2f, gyro_z:%.2f\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);

    mpu6050_get_temp(mpu6050, &temp);

    ESP_LOGI(TAG, "t:%.2f \n", temp.temp);
    vTaskDelay(300 / portTICK_RATE_MS);

   }
   

}