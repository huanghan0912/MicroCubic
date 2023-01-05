#include "HttpServer.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>


/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */
char wifi_name[30]={0};
char wifi_password[30]={0};

static const char *TAG = "HttpServer";




static esp_err_t index_get_handler(httpd_req_t *req)
{
    extern const unsigned char upload_script_start[] asm("_binary_wifi_html_start");
    extern const unsigned char upload_script_end[]   asm("_binary_wifi_html_end");
    const size_t upload_script_size = (upload_script_end - upload_script_start);
    ESP_LOGI(TAG, "sfsdf1");
    /* Add file upload form and script which on execution sends a POST request to /upload */
    httpd_resp_set_type(req,HTTPD_TYPE_TEXT);
    httpd_resp_send(req, (const char *)upload_script_start, upload_script_size);

     ESP_LOGI(TAG, "sfsdf1");
    return ESP_OK;

}
const char  index_string[] = 
"<!DOCTYPE html> \
<head> \
<meta charset=\"utf-8\"> \
<title>wifi config</title> \
</head>";
httpd_uri_t index_page = {
    .uri       = "/",    //192.168.1.4
    .method    = HTTP_GET,  
    .handler   = index_get_handler, 
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL, 
};
/* An HTTP POST handler */
static esp_err_t echo_post_handler(httpd_req_t *req)
{
    char buf[100];
    // char ssid[10];
    // char pswd[10];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        /* Send back the same data */
        httpd_resp_send_chunk(req, buf, ret);
        remaining -= ret;

        esp_err_t e = httpd_query_key_value(buf,"ssid",wifi_name,sizeof(wifi_name));
        if(e == ESP_OK) {
            printf("ssid = %s\r\n",wifi_name);
        }
        else {
            printf("error = %d\r\n",e);
        }

        e = httpd_query_key_value(buf,"password",wifi_password,sizeof(wifi_password));
        if(e == ESP_OK) {
            printf("pswd = %s\r\n",wifi_password);
        }
        else {
            printf("error = %d\r\n",e);
        }
        /* Log data received */
        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "====================================");
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    if(strcmp(wifi_name ,"\0")!=0 && strcmp(wifi_password,"\0")!=0)
    {
        xSemaphoreGive(ap_sem);
        ESP_LOGI(TAG, "set wifi name and password successfully! goto station mode");
    }
    return ESP_OK;
}

static const httpd_uri_t echo = {
    .uri       = "/",
    .method    = HTTP_POST,
    .handler   = echo_post_handler,
    .user_ctx  = NULL
};


httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        ESP_ERROR_CHECK( httpd_register_uri_handler(server, &echo));
        ESP_ERROR_CHECK( httpd_register_uri_handler(server, &index_page));
        
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}



