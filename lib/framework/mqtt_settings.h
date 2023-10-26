#pragma once
#include <PubSubClient.h>
#define MAX_COMMANDS 64  // the maximum of assignable callbacks to topics
#define MAX_POINTS 64

typedef std::function<void(byte *)> MQTT_CALLBACK;
typedef std::function<String()> MQTT_DATA_CALLBACK;

namespace MQTT {
enum SendMode { Interval, Change };
extern char *name;
extern char *broker;

extern PubSubClient client;
void init(const char *broker, const char *name);
void init(const char *broker, const char *name, int port);
void update();

bool add_command(char *path, MQTT_CALLBACK callback);
bool add_datapoint(char *path, SendMode mode, uint32_t interval, MQTT_DATA_CALLBACK callback);
}  // namespace MQTT
