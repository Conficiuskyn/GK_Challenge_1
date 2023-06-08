#include "main.h"

//SSID and PASSWORD should be defined in the configuration file and not like that
#define SSID "roro"
#define PASSWORD "123456789"

//Main app start assigning wifi credentials and launches main loop
//The wifi connection is handled itself, automatic reconnection
//Once wifi is alive, webserver connection goes on
void Main::run()
{   
    wifi.setSSIDandPASSWORD(SSID, PASSWORD);
    wifi.start();
    while(true)
    {
        ESP_LOGI(TAG, "Infinite loop");
        wifi.handle_wifi(); //handles all the wifi states
        if (wifi.get_state() == WifiStation::WifiState::CONNECTED 
            && !webserver.isRunning()) {
                webserver.start(); //start the webserver for request endpoints
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main(void)
{   
    esp_event_loop_create_default();
    nvs_flash_init();
    
    Main app;
    app.run();
}