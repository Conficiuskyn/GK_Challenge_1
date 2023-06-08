#include "esp_http_server.h"
#include "esp_log.h"

class HttpServer 
{
    public:
        bool isRunning();
        void start();
        void stop();
        static constexpr const char* TAG = "[http_server]";
    
    private:
        bool _running{false};
        httpd_handle_t _server;
};