#include <Arduino.h>
#include "wifi_settings.h"
#include "mqtt_settings.h"
using namespace MQTT;

const char *ssid = "........";
const char *password = "........";

const char *mqtt_broker = "........";
const char *mqtt_client_name = "........";

void set_pin_0(byte *payload)
{
  digitalWrite(0, payload[0] != '0');
}

void setup()
{
  WIFI::init(ssid, password);
  MQTT::init(mqtt_broker, mqtt_client_name);

  MQTT::add_command("gpio/set/0", set_pin_0);
}

void loop()
{
  MQTT::update();
}