#pragma once
#include <WiFiClient.h>
namespace WIFI {
extern WiFiClient client;
void init(const char *ssid, const char *password);
}  // namespace WIFI