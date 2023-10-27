#pragma once
#include <WiFiClient.h>
class WIFIConnector {
   public:
    WiFiClient client;
    WIFIConnector();

    /**
     * @brief Tries to connect to a network, returns wether the connection was successful
     *
     * @param ssid the ssid of the network
     * @param password the password of the network
     * @return true if the connection is successful
     */
    bool connect_blocking(const char *ssid, const char *password);
};