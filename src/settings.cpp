#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>

#include "defaults.hpp"
#include "settings.hpp"

Settings *Settings::instance = nullptr;

Settings *Settings::getInstance() {
    if (instance == nullptr) {
        instance = new Settings();
    }
    return instance;
}

Settings::Settings() {
    if (!SPIFFS.exists("/settings")) {
        Serial.println("No settings file");
        Serial.println("Loading default");
        loadDefaults();
        saveSettings();
    } else {
        Serial.println("Loading settings from file");
        loadSettings();
    }
}

void Settings::loadDefaults() {
    deviceTopic = defaults::deviceTopic;
    wifi_ssid = defaults::wifi_ssid;
    wifi_password = defaults::wifi_password;
    mqtt_host = defaults::mqtt_host;
    mqtt_port = defaults::mqtt_port;
}

void Settings::saveSettings() {

    DynamicJsonDocument settingsDoc(1024);

    auto settingsObj = settingsDoc.to<JsonObject>();

    settingsObj["deviceTopic"] = deviceTopic.c_str();
    settingsObj["wifi_ssid"] = wifi_ssid.c_str();
    settingsObj["wifi_password"] = wifi_password.c_str();
    settingsObj["mqtt_host"] = mqtt_host.toString();
    settingsObj["mqtt_port"] = mqtt_port;

    File settingsFile = SPIFFS.open("/settings", "w");
    serializeJsonPretty(settingsDoc, settingsFile);
    settingsFile.close();
}

void Settings::loadSettings() {
    File settingsFile = SPIFFS.open("/settings", "r");
    DynamicJsonDocument settingsDoc(1024);

    auto error = deserializeJson(settingsDoc, settingsFile);
    settingsFile.close();
    if (error) {
        Serial.println("Could not deserialize settings");
        Serial.println(error.c_str());
        loadDefaults();
        return;
    }

    auto settingsObj = settingsDoc.as<JsonObjectConst>();

    deviceTopic = settingsObj["deviceTopic"].as<const char *>();
    wifi_ssid = settingsObj["wifi_ssid"].as<const char *>();
    wifi_password = settingsObj["wifi_password"].as<const char *>();
    const auto mqtt_host_string = settingsObj["mqtt_host"].as<const char *>();
    mqtt_host = IPAddress().fromString(mqtt_host_string);
    mqtt_port = settingsObj["mqtt_port"].as<uint>();
}