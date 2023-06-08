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