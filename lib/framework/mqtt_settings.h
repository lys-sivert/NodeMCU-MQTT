#pragma once
#include <PubSubClient.h>
typedef std::function<void(byte *)> MQTT_CALLBACK;

namespace MQTT
{
    extern char *name;
    extern char *broker;

    extern PubSubClient client;
    void init(const char *broker, const char *name);
    void init(const char *broker, const char *name, int port);
    void update();
    bool add_command(char *path, MQTT_CALLBACK callback);
}
