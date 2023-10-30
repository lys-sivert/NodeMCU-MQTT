#include "mqtt_client.h"

#include <Client.h>
#include <PubSubClient.h>

void MQTTClient::_process_message(char *topic, byte *payload, unsigned int length) {
    for (int i = 0; i < _num_commands_assigned; i++) {
        if (strcmp(_commands[i].topic, topic) == 0) {
            Serial.println("Eq");
            _commands[i].callback(payload);
        }
    }
}

int MQTTClient::connect(Client &client, const char *broker, const char *id, int port) {
    return connect(client, broker, id, 1883, NULL, NULL);
}

int MQTTClient::connect(Client &client, const char *broker, const char *id, int port, const char *username,
                        const char *password) {
    _client.setClient(client);

    _client.setServer(broker, port);

    _client.connect(id, username, password);

    // this needed to be wrapped up in a lambda to coerce into a std::function type, this is big dumbo code
    _client.setCallback([this](char *topic, uint8_t *payload, unsigned int length) {
        Serial.println("Hello");
        Serial.println(topic);
        this->_process_message(topic, payload, length);
    });
    return status();
}

int MQTTClient::connect(Client &client, const char *broker, const char *id) {
    return connect(client, broker, id, 1883);
}

int MQTTClient::status() { return _client.state(); }

bool timer_expired(unsigned long current_time, unsigned long started, unsigned long target) {
    return (current_time - started) >= target;
}

void MQTTClient::update() {
    if (!_client.connected()) {
        _reconnect();
    }

    uint32_t current_time = millis();
    _client.loop();
    for (int i = 0; i < _num_points_assigned; i++) {
        if (timer_expired(current_time, _points[i].prev_triggered, _points[i].interval)) {
            _points[i].prev_triggered = current_time;

            String new_value = _points[i].callback();
            if (_points[i].mode == SendMode::Change) {
                if (new_value.equals(_points[i].prev_value)) {
                    continue;
                }
            }
            _points[i].prev_value = new_value;
            _client.publish(_points[i].topic, new_value.c_str());
        }
    }

    _client.loop();
}

void MQTTClient::_reconnect() {
    // Loop until we're reconnected
    while (!_client.connected()) {
        String clientId = name;
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (_client.connect(clientId.c_str())) {
        } else {
            delay(5000);
        }
    }
}

bool MQTTClient::add_datapoint(const char *path, SendMode mode, uint32_t interval, MQTT_DATA_CALLBACK callback) {
    if (_num_points_assigned == MAX_POINTS) return false;
    _points[_num_points_assigned].callback = callback;
    _points[_num_points_assigned].interval = interval;
    _points[_num_points_assigned].prev_triggered = 0;
    _points[_num_points_assigned].mode = mode;
    _points[_num_points_assigned].topic = path;
    _num_points_assigned++;
    return true;
}

bool MQTTClient::add_command(const char *path, MQTT_CALLBACK callback) {
    if (_num_commands_assigned == MAX_COMMANDS) return false;
    _commands[_num_commands_assigned].callback = callback;
    _commands[_num_commands_assigned].topic = path;
    _num_commands_assigned++;
    _client.subscribe(path);
    return true;
}