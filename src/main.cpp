#include "main.h"

#define SSID "roro"
#define PASSWORD "123456789"

void Main::run()
{   
    wifi.setSSIDandPASSWORD(SSID, PASSWORD);
    wifi.start();
    while(true)
    {
        ESP_LOGI(TAG, "Infinite loop");
        wifi.handle_wifi();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main(void)
{
    Main app;
    app.run();
}