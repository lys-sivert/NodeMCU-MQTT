#include "wifi_connector.h"

#include <ESP8266WiFi.h>

WIFIConnector::WIFIConnector() {}

bool WIFIConnector::connect_blocking(const char *ssid, const char *password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    wdt_disable();
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        if (WiFi.status() == WL_CONNECT_FAILED) {
            return false;
        }
        if (WiFi.status() == WL_WRONG_PASSWORD) {
            return false;
        }
    }
    wdt_enable(5000);

    return true;
}