#include "messages.hpp"
#include "AsyncMqttClient.hpp"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <io.hpp>

AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

const char* deviceTopic = "room/";

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
 Serial.println("Disconnected from Wi-Fi.");
 mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
 wifiReconnectTimer.once(2, connectToWifi);
}

void subscribeToPairs(){
    for(auto&& pair : io){
      std::string subscribe = deviceTopic;
      char buff[10];
      itoa(pair.number, buff, 10);
      subscribe.append(std::string(buff));
      mqttClient.subscribe(subscribe.c_str(),2);
      Serial.println(subscribe.c_str());
  }
}

void onMqttConnect(bool sessionPresent) {
  subscribeToPairs();
  
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  mqttClient.publish("testTopic", 2, false, "reconnected");
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
 Serial.println("Disconnected from MQTT.");

 if (WiFi.isConnected()) {
   mqttReconnectTimer.once(2, connectToMqtt);
 }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  size_t topicLen = sizeof(deviceTopic);
  char* topicEnd = topic + topicLen + 1;
  Serial.println(topicEnd);
  int chanelNumber = strtoul(topicEnd, nullptr, 10);
  int newState = atoi(payload);

  if (newState < 0 || newState > 8){
    return;
  }

  io[chanelNumber].changeState(newState);

  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void publishMqtt(int number, bool state){
  std::string publishTo = std::string(deviceTopic);
  char buff[10];
  itoa((int) number, buff, 10);
  publishTo.append(std::string(buff));
  const char* publishToChar = publishTo.c_str();

  char* stateToPublish = new char[2];
  itoa((int) state, stateToPublish, 10);

  mqttClient.publish(publishToChar, 2, 0, stateToPublish);
  Serial.println(publishToChar);
  Serial.println(stateToPublish);
  delete [] publishToChar;
  delete [] stateToPublish;
}

void setupNetwork() {
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  WiFi.mode(WiFiMode::WIFI_STA);
  connectToWifi(); 

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}