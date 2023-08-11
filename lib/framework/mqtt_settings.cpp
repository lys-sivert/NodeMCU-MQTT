#include "wifi_settings.h"
#include "mqtt_settings.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// change command functionality
typedef struct
{
    const char *topic;
    MQTT_CALLBACK callback;
} MQTT_Command;

WiFiClient wifi_client;

#define MAX_COMMANDS 64 // the maximum of assignable callbacks to topics

extern MQTT_Command commands[MAX_COMMANDS]; // the commands themselves

namespace MQTT
{
    char *broker;
    char *name;

    uint8_t num_commands_assigned = 0;
    MQTT_Command commands[MAX_COMMANDS];

    PubSubClient client(wifi_client);

    void _reconnect()
    {
        // Loop until we're reconnected
        while (!client.connected())
        {
            String clientId = name;
            clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (client.connect(clientId.c_str()))
            {
            }
            else
            {
                delay(5000);
            }
        }
    }

    void update()
    {

        if (!client.connected())
            _reconnect();
        client.loop();
    }

    void _process_message(char *topic, byte *payload, unsigned int length)
    {
        for (int i = 0; i < num_commands_assigned; i++)
        {
            if (strcmp(commands[i].topic, topic))
            {
                commands[i].callback(payload);
            }
        }
    }

    bool add_command(char *path, MQTT_CALLBACK callback)
    {
        if (num_commands_assigned == MAX_COMMANDS)
            return false;
        commands[num_commands_assigned].callback = callback;
        commands[num_commands_assigned].topic = path;
        num_commands_assigned++;
        return true;
    }

    void init(const char *broker, const char *name, int port = 1883)
    {
        client.setServer(broker, port);
        client.setCallback(_process_message);
    }

    void init(const char *broker, const char *name)
    {
        init(broker, name, 1883);
    }
}
