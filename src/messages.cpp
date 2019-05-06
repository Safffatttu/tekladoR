#include "messages.hpp"
#include "AsyncMqttClient.hpp"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <io.hpp>
#include <animationStore.hpp>
#include <settings.hpp>
#include <update.h>

AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  const char *ssid = Settings::getInstance()->wifi_ssid.c_str();
  const char *password = Settings::getInstance()->wifi_password.c_str();
  WiFi.begin(ssid, password);
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
      std::string subscribe = std::string(Settings::getInstance()->deviceTopic);
      subscribe = subscribe.append("pair/");
      subscribe = subscribe.append(pair.name);
      mqttClient.subscribe(subscribe.c_str(),2);
      Serial.println(subscribe.c_str());
  }
}

void subscribeToAnimations(){
  std::string subscribe = Settings::getInstance()->deviceTopic;
  subscribe.append("animation/#");
  mqttClient.subscribe(subscribe.c_str(),2);
}

void subscribeToSettingsUpdates(){
  std::string subscribe = Settings::getInstance()->deviceTopic;
  subscribe.append("settings/#");
  mqttClient.subscribe(subscribe.c_str(), 2);
}

void subscribeToRemoteUpdate(){
  std::string subscribe = Settings::getInstance()->deviceTopic;
  subscribe.append("startUpdate");
  mqttClient.subscribe(subscribe.c_str(), 2);
}

void onMqttConnect(bool sessionPresent) {
  subscribeToPairs();
  subscribeToAnimations();
  subscribeToSettingsUpdates();
  subscribeToSettingsUpdates();
  mqttClient.publish("deviceState", 2, true, "reconnected");
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

void parseIoMessage(char* topic, char* payload){
  std::string payloadString = std::string(payload);
  auto indexOfName = payloadString.find_last_of("/") + 1;
  std::string pairName = payloadString.substr(indexOfName);

  int newState = atoi(payload);

  for(auto&& pair : io)
  {
    if(pair.name == pairName)
    {
      pair.changeState(newState);
    }
  }
}

void updateMqttState()
{
  for(auto&& pair : io)
  {
    pair.updateMqttState();
  }
}

void parseAnimationMessage(std::string topic, char* payload)
{
  std::size_t foundNewAnimation = topic.find("new");

  if (foundNewAnimation != std::string::npos)
  {
    AnimationStore::getInstance()->addAnimation(payload);
  }
  else
  {
    int animationControll = atoi(payload);
    auto positionOfNumber = topic.find_last_of("/");
    auto animationNumberString = topic.substr(positionOfNumber + 1);
    int animationNumber = strtoul(animationNumberString.c_str(), nullptr, 10);
    
    if (animationControll == 0)
    {
      AnimationStore::getInstance()->stopAnimation();
    }
    else if (animationControll == 1)
    {
      AnimationStore::getInstance()->runAnimation(animationNumber);
    }
  }
}

void updateAnimationCount()
{
  uint count = AnimationStore::getInstance()->animationCount();
  char countChar[5];
  itoa(count, countChar, 10);
  std::string topic = (Settings::getInstance()->deviceTopic).append("animation/count");
  mqttClient.publish(topic.c_str(), 2, true, countChar);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  std::string topicString = std::string(topic);
  std::size_t foundAnimation = topicString.find("animation");

  if (foundAnimation != std::string::npos)
  {
    parseAnimationMessage(topicString, payload);
  }
  else if (topicString.find("checkState") != std::string::npos)
  {
    updateMqttState();
  }
  else if (topicString.find("animation/count") != std::string::npos)
  {
    updateAnimationCount();
  }
  else if (topicString.find("pair/") != std::string::npos)
  {
    parseIoMessage(topic, payload);
  }
  else if (topicString.find("startUpdate") != std::string::npos) {
    updateAndRebot();
  }
  else if (topicString.find("/settings") != std::string::npos)
  {
    Settings::getInstance()->updateSettings(topic, payload);
  }

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

void publishMqtt(std::string name, bool state){
  std::string publishTo = std::string(Settings::getInstance()->deviceTopic);
  publishTo = publishTo.append("pair/");
  publishTo = publishTo.append(name);

  char* stateToPublish = new char[2];
  itoa((int) state, stateToPublish, 10);

  mqttClient.publish(publishTo.c_str(), 2, true, stateToPublish);
}

void setupNetwork() {
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  WiFi.mode(WiFiMode::WIFI_STA);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(Settings::getInstance()->mqtt_host, Settings::getInstance()->mqtt_port);
  auto willTopic = Settings::getInstance()->deviceTopic;
  mqttClient.setWill(willTopic.append("deviceState").c_str(), 2, true, "disconnected");
  
  connectToWifi();
}