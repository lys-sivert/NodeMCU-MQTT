#pragma once
#include <Client.h>
#include <PubSubClient.h>

#define MAX_COMMANDS 64  // the maximum of assignable callbacks to topics
#define MAX_POINTS 64

class MQTT_Class {
   public:
    typedef std::function<void(byte *)> MQTT_CALLBACK;
    typedef std::function<String()> MQTT_DATA_CALLBACK;
    enum SendMode { Interval, Change };

    MQTT_Class(Client &client, const char *broker, const char *name);
    MQTT_Class(Client &client, const char *broker, const char *name, int port);

    void update();

    /**
     * @brief Adds a command to the MQTT client, a command is simply a function that is called when a message is
     * recieved on the specifiet topic. The topic is automatically subscribed to
     *
     * @param path the MQTT topic to listen to
     * @param callback a function that takes in the MQTT payload as input
     * @return true if the command is succesfully added
     */
    bool add_command(const char *path, MQTT_CALLBACK callback);

    /**
     * @brief Adds a datapoint to the MQTT client, a datapoint is simply a function that is called on an interval.
     * The callback is executed every [interval]ms, if the mode is set to change, the value is only published on a
     * changed value, or else the value is published every interval
     *
     * @param path the MQTT topic to write to
     * @param callback a function that returns a String containing the data
     * @return true if the datapoint is succesfully added
     */
    bool add_datapoint(const char *path, SendMode mode, uint32_t interval, MQTT_DATA_CALLBACK callback);

   private:
    void _process_message(char *topic, uint8_t *payload, unsigned int length);
    void _reconnect();

    PubSubClient _client;
    char *broker;
    char *name;
    int port;

    typedef struct {
        const char *topic;
        MQTT_CALLBACK callback;
    } MQTT_Command;

    typedef struct {
        const char *topic;
        MQTT_DATA_CALLBACK callback;
        uint32_t interval;
        uint32_t next_trigger;
        MQTT_Class::SendMode mode;
        String prev_value;
    } MQTT_Point;

    uint8_t _num_commands_assigned = 0;
    uint8_t _num_points_assigned = 0;

    MQTT_Command _commands[MAX_COMMANDS];
    MQTT_Point _points[MAX_POINTS];
};