#pragma once

#include <ESP8266WiFi.h>
#include <FS.h>

class Settings {
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
    std::string updateIp;
    uint16_t updatePort;
    std::string updateUrl;
};