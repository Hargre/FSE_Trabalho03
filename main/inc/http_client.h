#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#define REQUEST_TIMER 5 * 60 * 1000

#define IPSTACK_BASE "http://api.ipstack.com/check"
#define OWM_BASE "http://api.openweathermap.org/data/2.5/weather"

#define OWM_KEY CONFIG_OWM_API_KEY
#define IPSTACK_KEY CONFIG_IPSTACK_API_KEY

void http_request(char *url);
void build_ipstack_url(char *url);
void build_owm_url(char *url);

#endif
