#include "messages.hpp"
#include "AsyncMqttClient.hpp"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <io.hpp>
#include <animationStore.hpp>

AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

const char* deviceTopic = "hall/";

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

void subscribeToAnimations(){
  std::string subscribe = deviceTopic;
  subscribe.append("animation/#");
  mqttClient.subscribe(subscribe.c_str(),2);
}

void onMqttConnect(bool sessionPresent) {
  subscribeToPairs();
  subscribeToAnimations();
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
  size_t topicLen = sizeof(deviceTopic);
  char* topicEnd = topic + topicLen + 1;
  int channelNumber = strtoul(topicEnd, nullptr, 10);
  int newState = atoi(payload);

  if (channelNumber >= 0 && channelNumber <= 7){
    if(newState == 0 || newState == 1){
      io[channelNumber].changeState(newState);
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
    AnimationStore::getInstance()->addAnimation(std::string(payload));
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
  std::string topic = std::string(deviceTopic).append("animation/count");
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
  else
  {
    parseIoMessage(topic, payload);
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

void publishMqtt(int number, bool state){
  std::string publishTo = std::string(deviceTopic);
  char buff[10];
  itoa((int) number, buff, 10);
  publishTo.append(std::string(buff));
  const char* publishToChar = publishTo.c_str();

  char* stateToPublish = new char[2];
  itoa((int) state, stateToPublish, 10);

  mqttClient.publish(publishToChar, 2, true, stateToPublish);
  Serial.println(publishToChar);
  Serial.println(stateToPublish);
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