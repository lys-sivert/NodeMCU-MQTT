#include "wifi_settings.h"

#include <ESP8266WiFi.h>

namespace WIFI {
WiFiClient client;
void init(const char *ssid, const char *password) {
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    }
}
}  // namespace WIFI