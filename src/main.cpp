#include <Arduino.h>

#include "mqtt_client.h"
#include "wifi_connector.h"

const char *ssid = "........";
const char *password = "........";

const char *mqtt_broker = "........";
const char *mqtt_client_name = "........";

void set_pin_2(byte *payload) { digitalWrite(2, payload[0] != '0'); }
String get_pin_state() { return String(digitalRead(3)); }

WIFIConnector wifi(ssid, password);
MQTTClient mqtt(wifi.client, mqtt_broker, mqtt_client_name);

void setup() {
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    mqtt.add_command("gpio/set/2", set_pin_2);
    mqtt.add_datapoint("gpio/state/3", MQTTClient::Interval, 1000, get_pin_state);
}

void loop() { mqtt.update(); }