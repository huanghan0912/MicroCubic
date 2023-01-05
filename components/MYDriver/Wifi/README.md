# API说明

## Wifi类

### init

```c

/**
 * @brief 初始化wifi
 * 
 */
void init();
```

内容

- xEventGroupCreate():创建默认事件组
- esp_netif_init(): //初始化潜在的TCP/IP 栈
- esp_event_loop_create_default():创建默认循环组
- esp_netif_create_default_wifi_sta():
- esp_wifi_init()初始化wifi驱动
- esp_event_handler_instance_register:建立事件

说明

- 使用wifi类其他api必须先初始化

### STA_begin

```c

/**
 * @brief wifi的STA模式开启
 * 
 * @param ssid wifi广播
 * @param password wifi密码
 */
void Wifi::STA_begin(char* ssid,char* ssid)
```

内容

- 向wifi_config导入ssid和ssid(利用strcpy 函数)
- esp_wifi_set_mode(WIFI_MODE_STA),esp_wifi_set_config(WIFI_IF_STA, &wifi_config) 设置STA模式
- esp_wifi_start:开启wifi

### scan

``` c
/**
 * @brief 开启wifi扫描
 * 
 */
void Wifi::scan()
```

内容

- esp_wifi_set_mode(WIFI_MODE_STA)
- esp_wifi_start:开启wifi
- esp_wifi_scan_start:开启扫描
- 获取wifi信息和数目到ap_info中

### AP_begin

```c
/**
 * @brief wifi的STA模式开启
 * 
 * @param ssid wifi广播
 * @param password wifi密码
 */
void Wifi::AP_begin(char* ssid,char* password)
```

内容

- 向wifi_config导入相关配置
- esp_wifi_set_mode(WIFI_MODE_AP),esp_wifi_set_config(WIFI_IF_AP, &wifi_config) 设置STA模式
- esp_wifi_start:开启wifi

# 变量结构体说明

## Wifi类

### 宏定义(可根据个人修改)

``` c
//esp连接最大等待时间
#define ESP_MAXIMUM_RETRY  1000
//AP模式最大可连接量
#define MAX_STA_CONNECT 5
//AP模式信道
#define CONFIG_ESP_WIFI_CHANNEL 12
//扫描ap的最大数量
#define DEFAULT_SCAN_LIST_SIZE 10
```

### 变量

```c
//扫描出ap,和其数目
wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
uint16_t ap_count = 0;	
//wifi配置
wifi_config_t wifi_config;
//wifi名称和密码
uint8_t Wifi_ssid[30];
uint8_t Wifi_password[30];
```





# 待补充
- wifi中扫描的ap_info在flash分配了,后续可以节省空间进行删除操作