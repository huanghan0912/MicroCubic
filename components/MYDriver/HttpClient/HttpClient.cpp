/**
 * @file HttpClient.cpp
 * @author huanghan0912 (huanghan0912@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-24
 * 
 * 
 */
#include "HttpClient.h"
#include "string.h"
#include "esp_log.h"


static const char *TAG_HTTP="HttpClient";
/**
 * @brief 默认初始化
 * 
 */
void HttpClient::Init(){
    client = esp_http_client_init(&client_config);
    ESP_LOGI(TAG_HTTP,"init finished");
}
/**
 * @brief 用于常见获取网络的初始化,默认为get方法
 * 
 * @param url 调用api的url
 */
void HttpClient::Init(char* url){
    client_config.url=url;
    client = esp_http_client_init(&client_config);
    //Set http request method
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    ESP_LOGI(TAG_HTTP,"init finished");
}
/**
 * @brief 获取get请求返回的数据
 * 
 * @param Response 保存获取的数据
 */
void HttpClient::GetResponse(char *Response){
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_HTTP, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        strcpy(Response, "error ");
        return;
    } 
    //如果连接成功
    else {
        //读取目标主机的返回内容的协议头
        int content_length = esp_http_client_fetch_headers(client);

        //如果协议头长度小于0，说明没有成功读取到
        if (content_length < 0) {
            ESP_LOGE(TAG_HTTP, "HTTP client fetch headers failed");
            strcpy(Response, "error ");
            return ;
        } 

        //如果成功读取到了协议头
        else {

            //读取目标主机通过http的响应内容
            int data_read = esp_http_client_read_response(client, Response,450);
            if (data_read >= 0) {

                //打印响应内容，包括响应状态，响应体长度及其内容
                ESP_LOGI(TAG_HTTP, "HTTP GET Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),				//获取响应状态信息
                esp_http_client_get_content_length(client));			//获取响应信息长度
                ESP_LOGI(TAG_HTTP,"data:%s",Response);
			
            } 
            //如果不成功
            else {
                ESP_LOGE(TAG_HTTP, "Failed to read response");
                strcpy(Response, "error ");
            }
        }
    }
}

/**
 * @brief 关闭连接
 * 
 */
void HttpClient::close(){
    esp_http_client_close(client);
}

void HttpClient::del(){
    esp_http_client_cleanup(client);
}