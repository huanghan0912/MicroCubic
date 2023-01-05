/**
 * @file Sd.h
 * @author huanghan0912 (huanghan0912@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-18
 * 
 * 
 */

#ifndef _SD_H_
#define _SD_H_

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

/*******************************************************个人配置***********************************************/
#define SD_MISO GPIO_NUM_4
#define SD_MOSI GPIO_NUM_6
#define SD_CLK  GPIO_NUM_5
#define SD_CS   GPIO_NUM_7
//#define MOUNT_POINT  "/sdcard"
#define SPI_DMA_CHAN    SPI_DMA_CH_AUTO


/*******************************************************Sd类******************************************************/
class Sd
{
private:
   spi_bus_config_t bus_cfg;
   esp_vfs_fat_mount_config_t  mount_config ;
   sdmmc_host_t host;
   sdspi_device_config_t slot_config ;
   sdmmc_card_t *card;
   char mount_point[20] = "/sdcard";
public:
    //初始化sd卡
    void Init();
   
};












#endif