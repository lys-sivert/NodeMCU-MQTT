#include "mqtt_settings.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "wifi_settings.h"

// change command functionality
typedef struct {
    const char *topic;
    MQTT_CALLBACK callback;
} MQTT_Command;

typedef struct {
    const char *topic;
    MQTT_DATA_CALLBACK callback;
    uint32_t interval;
    uint32_t next_trigger;
    MQTT::SendMode mode;
    String prev_value;
} MQTT_Points;

WiFiClient wifi_client;

namespace MQTT {
char *broker;
char *name;

uint8_t num_commands_assigned = 0;
uint8_t num_points_assigned = 0;

MQTT_Command commands[MAX_COMMANDS];
MQTT_Points points[MAX_POINTS];

PubSubClient client(wifi_client);

void _reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        String clientId = name;
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
        } else {
            delay(5000);
        }
    }
}

void update() {
    if (!client.connected()) {
        _reconnect();
    }

    uint32_t current_time = millis();
    client.loop();
    for (int i = 0; i < num_points_assigned; i++) {
        if (current_time + points[i].interval > points[i].next_trigger) {
            points[i].next_trigger = current_time + points[i].interval;

            String new_value = points[i].callback();
            if (points[i].mode == SendMode::Change) {
                if (new_value.equals(points[i].prev_value)) {
                    continue;
                }
            }
            points[i].prev_value = new_value;
            client.publish(points[i].topic, new_value.c_str());
        }
    }

    client.loop();
}

void _process_message(char *topic, byte *payload, unsigned int length) {
    for (int i = 0; i < num_commands_assigned; i++) {
        if (strcmp(commands[i].topic, topic)) {
            commands[i].callback(payload);
        }
    }
}

bool add_command(char *path, MQTT_CALLBACK callback) {
    if (num_commands_assigned == MAX_COMMANDS) return false;
    commands[num_commands_assigned].callback = callback;
    commands[num_commands_assigned].topic = path;
    num_commands_assigned++;
    return true;
}

bool add_datapoint(char *path, SendMode mode, uint32_t interval, MQTT_DATA_CALLBACK callback) {
    if (num_points_assigned == MAX_POINTS) return false;
    points[num_points_assigned].callback = callback;
    points[num_points_assigned].interval = interval;
    points[num_points_assigned].next_trigger = 0;
    points[num_points_assigned].mode = mode;
    points[num_points_assigned].topic = path;
    num_points_assigned++;
    return true;
}

void init(const char *broker, const char *name, int port) {
    client.setServer(broker, port);
    client.setCallback(_process_message);
}

void init(const char *broker, const char *name) { init(broker, name, 1883); }
}  // namespace MQTT
