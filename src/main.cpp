#include <Arduino.h>

#include "mqtt_client.h"
#include "wifi_connector.h"

const char *ssid = "........";
const char *password = "........";

const char *mqtt_broker = "........";
const char *mqtt_client_name = "........";

void set_pin_2(byte *payload) { digitalWrite(2, payload[0] != '0'); }
String get_pin_state() { return String(digitalRead(3)); }

WIFIConnector wifi;
MQTTClient mqtt;

void setup() {
    Serial.begin(9600);
    while (Serial.available()) {
    }
    Serial.println("Start");
    bool connected = wifi.connect(ssid, password);
    Serial.println(connected);
    if (!connected) {
        while (true) {
        }
    };

    mqtt.connect(wifi.client, mqtt_broker, mqtt_client_name);
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    // mqtt.add_command("gpio/set/2", set_pin_2);
    Serial.println("added command");
    // mqtt.add_datapoint("gpio/state/3", MQTTClient::Interval, 1000, get_pin_state);
    Serial.println("added datapoint");

    Serial.println("DONE!");
}

void loop() {
    Serial.print("R");
    // mqtt.update();
}