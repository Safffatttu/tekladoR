#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <string.h>

const char* ssid     = "***REMOVED***";
const char* password = "***REMOVED***";
const char* deviceTopic = "room/";

WiFiClient WiFiclient;
MQTTClient client;

const int RELAYS_COUNT = 1;

int relayPins[RELAYS_COUNT] = {12};
int inputPins[RELAYS_COUNT] = {13};
int states   [RELAYS_COUNT] = {0};

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  setupOutputs();
  setupInputs();
  
  client.begin("192.168.1.71", 1883, WiFiclient);
  client.onMessage(messageReceived);
  connectMqtt();
}

void loop() {
  checkMqttConnection();
  checkInputs();
  client.loop();
}
