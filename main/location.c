#include "cJSON.h"

#include "location.h"


extern Location_t location_data;

void parse_location(char *location_json) {
    cJSON *data = cJSON_Parse(location_json);
    location_data.latitude = cJSON_GetObjectItem(data, "latitude")->valuedouble;
    location_data.longitude = cJSON_GetObjectItem(data, "longitude")->valuedouble;
    cJSON_Delete(data);
}