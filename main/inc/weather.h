#ifndef WEATHER_H
#define WEATHER_H

typedef struct Weather_t {
    double temp_current;
    double temp_max;
    double temp_min;
    double humidity;
} Weather_t;

void parse_weather(char *weather_json);
void print_weather_data();

#endif