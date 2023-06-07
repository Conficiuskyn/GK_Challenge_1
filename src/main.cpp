#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Main
{
    const char* TAG = "[main]";
    public:
        void run()
        {   
            while(true)
            {
                ESP_LOGI(TAG, "Infinite loop");
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
};


extern "C" void app_main(void)
{
    Main app;
    app.run();
}