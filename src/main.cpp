#include "main.hpp"

void Main::run()
{
    while(true)
    {
        ESP_LOGI(TAG, "Infinite loop");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main(void)
{
    Main app;
    app.run();
}