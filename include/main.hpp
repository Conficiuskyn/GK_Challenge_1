#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



class Main
{
    static constexpr const char* TAG = "[main]";

    public:
        void run();
};