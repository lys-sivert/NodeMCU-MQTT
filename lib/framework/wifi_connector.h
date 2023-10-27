#pragma once
#include <WiFiClient.h>
class WIFIConnector {
   public:
    WiFiClient client;
    WIFIConnector(const char *ssid, const char *password);
};