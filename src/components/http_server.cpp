#include "http_server.h"

void HttpServer::start()
{
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&_server, &cfg) == ESP_OK) {
        _running = true;
        ESP_LOGI(TAG, "Started");
    }
    else {
        _running = false;
        ESP_LOGI(TAG, "Starting Failed");
    }
}

void HttpServer::stop()
{
    httpd_stop(&_server);
    _running = false;
    ESP_LOGI(TAG, "Stopped");
}

bool HttpServer::isRunning() {
    return _running;
}

esp_err_t HttpServer::_writeStringToEEPROM(httpd_req_t *req) 
{   
    char content[200];
    int content_length = httpd_req_get_hdr_value_len(req, "Content-Length") + 1;
    if (content_length > sizeof(content)) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Content too long");
        return ESP_FAIL;
    }
    httpd_req_recv(req, content, content_length);
    content[content_length] = '\0';
    httpd_resp_sendstr(req, "POST request received");
    ESP_LOGI(TAG, "Received data: %s", content);

    nvs_handle_t nvs_handler;
    nvs_open("storage", NVS_READWRITE, &nvs_handler);

    nvs_set_str(nvs_handler, _EEPROM_KEY, content);
    nvs_commit(nvs_handler);

    const char *response = "String was written to the EEPROM";
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

esp_err_t HttpServer::_readStringFromEEPROM(httpd_req_t *req) 
{  
    nvs_handle_t nvs_handler;
    nvs_open("storage", NVS_READONLY, &nvs_handler);
    
    size_t required_size = 0;
    nvs_get_str(nvs_handler, _EEPROM_KEY, NULL, &required_size);
    char *data = static_cast<char*>(malloc(required_size));
    nvs_get_str(nvs_handler, _EEPROM_KEY, data, &required_size);

    httpd_resp_send(req, data, sizeof(data) - 1);

    ESP_LOGI(TAG, "Sending data: %s", data);
    return ESP_OK;
}