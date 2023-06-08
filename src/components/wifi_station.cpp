#include "wifi_station.h"

wifi_init_config_t WifiStation::_wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
wifi_config_t WifiStation::_wifi_cfg{};
WifiStation::WifiState WifiStation::_state{WifiStation::WifiState::NOT_INITIALIZED};

void WifiStation::setSSIDandPASSWORD(const char* ssid, const char* password)
{
    memcpy(_wifi_cfg.sta.ssid, ssid, sizeof(_wifi_cfg.sta.ssid));
    memcpy(_wifi_cfg.sta.password, password, sizeof(_wifi_cfg.sta.password));
}

void WifiStation::start() {
    _start();
}

void WifiStation::_start() {

    if (_state == WifiState::NOT_INITIALIZED) 
    {   
        ESP_LOGI(TAG, "Starting...");

        ESP_ERROR_CHECK(esp_netif_init());
        esp_netif_create_default_wifi_sta();
        ESP_ERROR_CHECK(esp_wifi_init(&_wifi_init_cfg));

        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &wifi_event_handler,
                                                            NULL,
                                                            NULL));

        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                            IP_EVENT_STA_GOT_IP,
                                                            &ip_event_handler,
                                                            NULL,
                                                            NULL));

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

        _wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        _wifi_cfg.sta.pmf_cfg.capable = true;
        _wifi_cfg.sta.pmf_cfg.required = false;
        
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &_wifi_cfg));
        ESP_ERROR_CHECK(esp_wifi_start());    

        _state = WifiState::INITIALIZED;  
        ESP_LOGI(TAG, "INITIALIZED");
    }
    else {
        ESP_LOGI(TAG, "Starting Failed");
    }
}   

void WifiStation::wifi_event_handler(
            void *arg,
            esp_event_base_t event_base,
            int32_t event_id,
            void *event_data) 
    {

        if (event_base == WIFI_EVENT) {
            const wifi_event_t event_type{static_cast<wifi_event_t>(event_id)};
            switch(event_type)
            {
                case WIFI_EVENT_STA_START:
                {
                    _state = WifiState::READY_TO_CONNECT;
                    break;
                }
                case WIFI_EVENT_STA_CONNECTED:
                {
                    _state = WifiState::WAITING_FOR_IP;
                    break;
                }
                case WIFI_EVENT_STA_DISCONNECTED:
                {
                    _state = WifiState::DISCONNECTED;
                    break;
                }
                default:
                    break;
            }
        }    
    }

void WifiStation::ip_event_handler(
            void *arg,
            esp_event_base_t event_base,
            int32_t event_id,
            void *event_data) 
    {

        if (event_base == IP_EVENT) {
            const ip_event_t event_type{static_cast<ip_event_t>(event_id)};
            switch(event_type)
            {
                case IP_EVENT_STA_GOT_IP:
                {
                    _state = WifiState::CONNECTED;
                    break;
                }
                case IP_EVENT_STA_LOST_IP:
                {   
                    if (_state != WifiState::DISCONNECTED) {
                        _state = WifiState::WAITING_FOR_IP;
                    }
                    break;
                }
                default:
                    break;
            }
        }    
    }

void WifiStation::_connect() {
    ESP_LOGI(TAG, "connecting......");
}

void WifiStation::handle_wifi() 
{
    switch (_state)
    {
        case (WifiState::READY_TO_CONNECT):
            ESP_LOGI(TAG, "READY_TO_CONNECT");
            _connect();
            break;
        
        case (WifiState::DISCONNECTED):
            ESP_LOGI(TAG, "READY_TO_CONNECT");
            _connect();
            break;
        
        default:
            break;
    }
}