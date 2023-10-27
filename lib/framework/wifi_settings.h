#pragma once
#include <WiFiClient.h>
class WIFI_Class {
   public:
    WiFiClient client;
    WIFI_Class(const char *ssid, const char *password);
};