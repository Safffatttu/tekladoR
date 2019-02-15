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
    void updateSettings();

    std::string deviceTopic;
    std::string wifi_ssid;
    std::string wifi_password;
    IPAddress mqtt_host;
    uint mqtt_port;
};