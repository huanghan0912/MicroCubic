# 

## 简介

本项目灵感来自稚晖君 [bilibili](https://www.bilibili.com/video/BV1VA411p7MD/) 其[github](https://github.com/peng-zhihui/HoloCubic)

代码参考[HoloCubic AIO](https://github.com/ClimbSnail/HoloCubic_AIO)

本项目没有用arduino开发，而是选用乐鑫的[esp-idf](https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.3/esp32s3/index.html)

esp-idf版本 V4.4.3

## 整体代码框架

引用的库 

1. [lvgl](https://github.com/lvgl/lvgl) : 单片机的小型化UI库
2. [lvgl_esp32_drivers](https://github.com/lvgl/lvgl_esp32_drivers) ：集合常见的显示芯片的驱动
3. [i2Cdev](https://github.com/jrowberg/i2cdevlib)和[mpu6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050) 陀螺仪驱动

以上的库个人做了移植的修改

1. AppConTroller: 主控制器，切换app，进入app，退出app
2. Application ：各种app
   1. weatherApp ：天气（需要联网）
   2. game 2048 : 2048小游戏
   3. wifi set ： 配网简介，利用web配网
   4. upgrade ： 可以进行远程升级，ota
   5. 待续。。。。
3. mpu6050 ：包含上述[mpu6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)引用的库，还添加识别动作信息
4. MYDriver：基本外设代码，包含wifi，sd卡，spiffs，httpclient，可在esp-idf上有说明

## 硬件信息

设备： 

	1. esp32-s3 flash>=16mb（也可以用其他乐鑫芯片）
	1. mpu6050
	1. cp2102
	1. sd卡



待续。。。。。

