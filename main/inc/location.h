#ifndef LOCATION_H
#define LOCATION_H

typedef struct Location_t {
    double latitude;
    double longitude;
} Location_t;

void parse_location(char *location_json);

#endif