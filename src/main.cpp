#include <Arduino.h>

#include "mqtt_settings.h"
#include "wifi_settings.h"

const char *ssid = "........";
const char *password = "........";

const char *mqtt_broker = "........";
const char *mqtt_client_name = "........";

void set_pin_0(byte *payload) { digitalWrite(0, payload[0] != '0'); }
String get_pin_state() { return String("Hello"); }

WiFiClient wifi;
MQTT_Class mqtt(wifi, mqtt_broker, mqtt_client_name);

void setup() {
    WIFI::init(ssid, password);

    mqtt.add_command("gpio/set/0", set_pin_0);
    mqtt.add_datapoint("gpio/state/0", MQTT_Class::Interval, 1000, get_pin_state);
}

void loop() { mqtt.update(); }