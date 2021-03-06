#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "http_client.h"
#include "driver/gpio.h"

#include "wifi.h"
#include "led.h"
#include "weather.h"
#include "location.h"
#include "control.h"

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle blinkSemaphore;

Location_t location_data;
Weather_t weather_data;

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifiSemaphore = xSemaphoreCreateBinary();
    blinkSemaphore = xSemaphoreCreateBinary();

    init_led();
    xTaskCreate(&TaskBlink, "Blink", 2048, NULL, 1, NULL);
    xSemaphoreGive(blinkSemaphore);

    wifi_start();
    xTaskCreate(&TaskHTTPRequestChain, "Processa HTTP", 4096, NULL, 1, NULL);
}
