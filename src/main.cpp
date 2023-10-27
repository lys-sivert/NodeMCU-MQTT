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
MQTTClient mqtt(wifi.client, mqtt_broker, mqtt_client_name);

void setup() {
    Serial.begin(9600);

    if (!wifi.connect(ssid, password)) {
        Serial.println("Could not connect to network");
        wdt_disable();
        while (true) {
            delay(100);
        }
    };
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    mqtt.add_command("gpio/set/2", set_pin_2);
    Serial.println("added command");
    mqtt.add_datapoint("gpio/state/3", MQTTClient::Interval, 1000, get_pin_state);
    Serial.println("added datapoint");

    Serial.println("DONE!");
}

void loop() {
    // mqtt.update();
}