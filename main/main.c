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

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle blinkSemaphore;

Location_t location_data;
Weather_t weather_data;

void HTTPRequestChain(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(wifiSemaphore, portMAX_DELAY))
        {
            char url[128];
            ESP_LOGI("Main Task", "Request Location");
            blink_led();
            build_ipstack_url(url);
            http_request(url);
            ESP_LOGI("Main Task", "Request Weather");
            blink_led();
            build_owm_url(url);
            http_request(url);

            xSemaphoreGive(wifiSemaphore);
            vTaskDelay(REQUEST_TIMER / portTICK_PERIOD_MS);
        }
    }
}

void Blink(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(blinkSemaphore, portMAX_DELAY))
        {
            blink_led();
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            xSemaphoreGive(blinkSemaphore);
        }
    }
}

void app_main(void)
{
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    init_led();

    wifiSemaphore = xSemaphoreCreateBinary();
    blinkSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(&Blink, "Blink", 2048, NULL, 1, NULL);
    xSemaphoreGive(blinkSemaphore);


    wifi_start();

    xTaskCreate(&HTTPRequestChain, "Processa HTTP", 4096, NULL, 1, NULL);
}
