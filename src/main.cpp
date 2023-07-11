#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

typedef struct
{
  const char *topic;
  const uint8_t pin;
} Command;

#define COMMAND_NUM 7
Command commands[COMMAND_NUM] = {
    {"nodemcu/gpio/gpio0", 0},
    {"nodemcu/gpio/gpio1", 1},
    {"nodemcu/gpio/gpio2", 2},
    {"nodemcu/gpio/gpio3", 3},
    {"nodemcu/gpio/gpio4", 4},
    {"nodemcu/gpio/gpio5", 5},
    {"nodemcu/gpio/gpio6", 6}};

const char *ssid = "........";
const char *password = "........";
const char *mqtt_server = "broker.mqtt-dashboard.com";
const char *clientName = "ESP32";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < COMMAND_NUM; i++)
  {
    if (strcmp(commands[i].topic, topic))
    {
      digitalWrite(commands[i].pin, payload[0] != '0');
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = clientName;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");

      client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}