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

/**
 * @brief Adds a command to the MQTT client, a command is simply a function that is called when a message is recieved on
 * the specifiet topic. The topic is automatically subscribed to
 *
 * @param path the MQTT topic to listen to
 * @param callback a function that takes in the MQTT payload as input
 * @return true if the command is succesfully added
 */
bool add_command(char *path, MQTT_CALLBACK callback);

/**
 * @brief Adds a datapoint to the MQTT client, a datapoint is simply a function that is called on an interval.
 * The callback is executed every [interval]ms, if the mode is set to change, the value is only published on a changed
 * value, or else the value is published every interval
 *
 * @param path the MQTT topic to write to
 * @param callback a function that returns a String containing the data
 * @return true if the datapoint is succesfully added
 */
bool add_datapoint(char *path, SendMode mode, uint32_t interval, MQTT_DATA_CALLBACK callback);
}  // namespace MQTT
