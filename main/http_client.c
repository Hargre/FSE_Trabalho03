#include <string.h>


#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "http_client.h"
#include "location.h"
#include "weather.h"


#define TAG "HTTP"

extern Location_t location_data;
extern Weather_t weather_data;

char buffer[512];
int read_pointer = 0;

char url_buf[50];

esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{

    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            esp_http_client_get_url(evt->client, url_buf, 256);
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%s", url_buf);
            
            memcpy(buffer + read_pointer, evt->data, evt->data_len);
            read_pointer += evt->data_len;

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            buffer[read_pointer+1] = '\0';

            if (strcmp(url_buf, IPSTACK_BASE) == 0) {
                parse_location(buffer);
            } else {
                parse_weather(buffer);
                print_weather_data();
            }

            read_pointer = 0;
            memset(buffer, 0, 512);
            memset(url_buf, 0, 50);

            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

void http_request(char *url){
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handle,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
    ESP_LOGI(TAG, "Status = %d, content_length = %d",
            esp_http_client_get_status_code(client),
            esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);
}


void build_ipstack_url(char *url) {
    sprintf(
        url,
        "%s?access_key=%s&fields=latitude,longitude",
        IPSTACK_BASE, IPSTACK_KEY
    );
}

void build_owm_url(char *url) {
    sprintf(
        url, 
        "%s?lat=%.2f&lon=%.2f&appid=%s&units=metric",
        OWM_BASE,
        location_data.latitude, location_data.longitude,
        OWM_KEY
    );
}