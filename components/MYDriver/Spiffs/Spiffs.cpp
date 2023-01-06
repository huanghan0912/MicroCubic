#include "Spiffs.h"




static const char *TAG = "Spiffs";


esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",          //与文件系统关联的文件路径前缀
        .partition_label = NULL,         //SPIFFS分区使用的标签。如果设置为NULL，将使用subtype=spiffs的第一个分区
        .max_files = 5,                  //同时打开文件的最大数量;文件[]数组的大小
        .format_if_mount_failed = true
        }; //如果为true，如果挂载失败，它将格式化文件系统

void SpiffsInit(){
     esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "挂载或格式化文件系统失败");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "找不到SPIFFS分区");
        }
        else
        {
            ESP_LOGE(TAG, "初始化SPIFFS失败 (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "日志含义获取SPIFFS分区信息失败 (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "分区大小: 合计: %d, used: %d", total, used);
    }
}


void SpiffsWriteFile(const char *Filename, const char *message)
{
    char temp[100];
    sprintf(temp, "%s%s","/spiffs/", Filename);
    FILE *f = fopen(temp, "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for Writting");
        return;
    }
    fprintf(f, "%s", message);
    fclose(f);
    ESP_LOGI(TAG, "file wrote successfully");
}

int SpiffsReadFile(const char *Filename, char *message)
{
    char temp[100];
    sprintf(temp, "%s%s","/spiffs/", Filename);
    FILE *f= fopen(temp, "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return 0;
    }
    fseek(f, 0, SEEK_END);      //将文件指针指向该文件的最后
    int file_size = ftell(f);       //根据指针位置，此时可以算出文件的字符数
    printf("文件字符数为 %d\n", file_size);
    fseek(f, 0, SEEK_SET);                 //重新将指针指向文件首部
    fread(message, sizeof(char),file_size, f); //开始读取整个文件
    fclose(f);
    return file_size;
    ESP_LOGI(TAG, "file read successfully");
}


void SpiffsDelFile(const char *Filename)
{
    char temp[100];
    sprintf(temp, "%s%s","/spiffs/", Filename);
    if (remove(temp) == 0) {
        ESP_LOGI(TAG, "Delete file successfully");
    }else {
        ESP_LOGE(TAG, "Delete file failed");
    }
}


bool SpiffsAnalyseParam(char *info, int argc, char **argv)
{
    int cnt; // 记录解析到第几个参数
    for (cnt = 0; cnt < argc; ++cnt)
    {
        argv[cnt] = info;
        while (*info != '\n')
        {
            ++info;
        }
        *info = 0;
        ++info;
    }
    return true;
}

