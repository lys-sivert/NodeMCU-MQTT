#include "wifi_connector.h"

#include <ESP8266WiFi.h>


WIFIConnector::WIFIConnector(const char *ssid, const char *password) {
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    }
}
