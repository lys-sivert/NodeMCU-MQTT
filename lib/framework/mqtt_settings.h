#pragma once
#include <PubSubClient.h>
#define MAX_COMMANDS 64 // the maximum of assignable callbacks to topics

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
