#include "esp_http_server.h"
#include "esp_log.h"
#include "nvs_flash.h"

class HttpServer 
{
    public:
        bool isRunning();
        void start();
        void stop();
        static constexpr const char* TAG = "[http_server]";
    
    private:
        static constexpr const char* _EEPROM_KEY = "0x123";
        bool _running{false};
        httpd_handle_t _server;
        static esp_err_t _writeStringToEEPROM(httpd_req_t *req);
        static esp_err_t _readStringFromEEPROM(httpd_req_t *req);
        void _registerRequests();
};