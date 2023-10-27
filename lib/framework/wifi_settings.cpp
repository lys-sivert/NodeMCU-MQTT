#include "wifi_settings.h"

#include <ESP8266WiFi.h>

WIFI_Class::WIFI_Class(const char *ssid, const char *password) {
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    }
}
