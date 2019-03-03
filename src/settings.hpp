#pragma once

#include <FS.h>
#include <ESP8266WiFi.h>

class Settings
{
  private:
    Settings();
    void saveSettings();
    void loadSettings();
    static Settings *instance;

  public:
    static Settings *getInstance();
    void updateSettings(char *topic, char *payload);

    std::string deviceTopic;
    std::string wifi_ssid;
    std::string wifi_password;
    IPAddress mqtt_host;
    uint mqtt_port;
    std::string updateIp;
    uint16_t updatePort;
    std::string updateUrl;
};