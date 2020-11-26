#include <stdio.h>
#include "cJSON.h"

#include "weather.h"

extern Weather_t weather_data;

void parse_weather(char *weather_json) {
    cJSON *data = cJSON_Parse(weather_json);
    cJSON *data_main = cJSON_GetObjectItem(data, "main");
    weather_data.temp_current = cJSON_GetObjectItem(data_main, "temp")->valuedouble;
    weather_data.temp_max = cJSON_GetObjectItem(data_main, "temp_max")->valuedouble;
    weather_data.temp_min = cJSON_GetObjectItem(data_main, "temp_min")->valuedouble;
    weather_data.humidity = cJSON_GetObjectItem(data_main, "humidity")->valuedouble;
    cJSON_Delete(data);
}

void print_weather_data() {
    printf(
        "Temperatura atual:\t%.2f\n"
        "Temperatura mínima:\t%.2f\n"
        "Temperatura máxima:\t%.2f\n"
        "Umidade:\t\t%.2f\n",
        weather_data.temp_current,
        weather_data.temp_min,
        weather_data.temp_max,
        weather_data.humidity
    );
}