#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "mqtt_client.h"
#include "wifi_connector.h"

const char *ssid = ".......";
const char *password = ".......";

const char *mqtt_broker = ".......";
const char *mqtt_client_name = ".......";

void set_pin_2(byte *payload) {
    Serial.println("Set pin 2");
    Serial.println(payload[0] != '0');
    digitalWrite(2, payload[0] != '0');
}
String get_pin_state() { return String(digitalRead(3)); }

WIFIConnector wifi;
MQTTClient mqtt;

void setup() {
    Serial.begin(9600);
    while (Serial.available()) {
    }
    Serial.println("");

    while (!wifi.connect_blocking(ssid, password)) {
        Serial.println("Could not connect to WiFi");
        delay(1000);
        Serial.println("Retrying");
    };

    ;
    while (!(mqtt.connect(wifi.client, mqtt_broker, mqtt_client_name) == 0)) {
        Serial.println("Could not connect to MQTT broker");
        delay(1000);
        Serial.println("Retrying");
    };

    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    mqtt.add_command("gpio/set/2", set_pin_2);
    mqtt.add_datapoint("gpio/state/3", MQTTClient::Interval, 1000, get_pin_state);

    Serial.println("DONE!");
}

void loop() { mqtt.update(); }