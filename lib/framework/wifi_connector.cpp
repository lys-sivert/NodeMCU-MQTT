#include "wifi_connector.h"

#include <ESP8266WiFi.h>

WIFIConnector::WIFIConnector() {
    delay(10);

    WiFi.mode(WIFI_STA);
}

bool WIFIConnector::connect(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        if (WiFi.status() == WL_CONNECT_FAILED) {
            return false;
        }
        if (WiFi.status() == WL_WRONG_PASSWORD) {
            return false;
        }
        if (WiFi.status() == WL_DISCONNECTED) {
            return false;
        }
    }

    return true;
}