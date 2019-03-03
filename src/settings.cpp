#include <settings.hpp>
#include <defaults.hpp>

Settings *Settings::instance = nullptr;

Settings::Settings()
{
    SPIFFS.begin();

    if (!SPIFFS.exists("/settings"))
    {
        Serial.println("No values");
        deviceTopic = defaults::deviceTopic;
        wifi_ssid = defaults::wifi_ssid;
        wifi_password = defaults::wifi_password;
        mqtt_host = defaults::mqtt_host;
        mqtt_port = defaults::mqtt_port;
        updateIp = defaults::updateIp;
        updatePort = defaults::updatePort;
        updateUrl = defaults::updateUrl;
        saveSettings();
    }
    else
    {
        Serial.println("LoadingData");
        loadSettings();
    }

    SPIFFS.end();
}

void Settings::saveSettings()
{
    SPIFFS.begin();
    File settingsFile = SPIFFS.open("/settings", "w+b");

    Serial.println("save");
    settingsFile.println(deviceTopic.c_str());
    settingsFile.println(wifi_ssid.c_str());
    settingsFile.println(wifi_password.c_str());
    settingsFile.println(mqtt_host.toString());
    settingsFile.println(mqtt_port);
    settingsFile.println(updateIp.c_str());
    settingsFile.println(updatePort);
    settingsFile.println(updateUrl.c_str());

    settingsFile.seek(0);
    Serial.println(settingsFile.readString());

    settingsFile.close();
    SPIFFS.end();
}

void Settings::loadSettings()
{
    SPIFFS.begin();
    File settingsFile = SPIFFS.open("/settings", "r");

    auto deviceTopicLine = std::string(settingsFile.readStringUntil('\n').c_str()).append("a");
    deviceTopicLine = deviceTopicLine.substr(0, deviceTopicLine.size() - 2);
    deviceTopic = deviceTopicLine.c_str();

    auto wifiSsidLine = std::string(settingsFile.readStringUntil('\n').c_str()).append("a");
    wifiSsidLine = wifiSsidLine.substr(0, wifiSsidLine.size() - 2);
    wifi_ssid = wifiSsidLine.c_str();

    auto wifiPasswordLine = std::string(settingsFile.readStringUntil('\n').c_str()).append("a");
    wifiPasswordLine = wifiPasswordLine.substr(0, wifiPasswordLine.size() - 2);
    wifi_password = wifiPasswordLine.c_str();

    auto mqttHostLine = std::string(settingsFile.readStringUntil('\n').c_str()).append("a");
    mqttHostLine = mqttHostLine.substr(0, mqttHostLine.size() - 2);
    mqtt_host.fromString(mqttHostLine.c_str());

    auto mqttPortLine = settingsFile.readStringUntil('\n').substring(0, settingsFile.size() - 5);
    mqtt_port = atoi(mqttPortLine.c_str());

    auto updateIpLine = std::string(settingsFile.readStringUntil('\n').c_str()).append("a");
    updateIp = updateIpLine.substr(0, updateIpLine.size() - 2);

    auto updatePortLine = settingsFile.readStringUntil('\n').substring(0, settingsFile.size() - 5);
    mqtt_port = atoi(updatePortLine.c_str());

    auto updateUrlLine = std::string(settingsFile.readStringUntil('\n').c_str()).append("a");
    updateUrl = updateUrlLine.substr(0, updateUrlLine.size() - 2);

    Serial.println("device");
    Serial.println(deviceTopicLine.c_str());
    Serial.println("ssid");
    Serial.println(wifiSsidLine.c_str());
    Serial.println(wifi_ssid.c_str());
    Serial.println("password");
    Serial.println(wifiPasswordLine.c_str());
    Serial.println(wifi_password.c_str());
    Serial.println("host");
    Serial.println(mqttHostLine.c_str());
    Serial.println(mqtt_host.toString());
    Serial.println("port");
    Serial.println(mqttPortLine);
    Serial.println(mqtt_port);
    Serial.println("closed");
    settingsFile.close();
    SPIFFS.end();
}

Settings *Settings::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Settings();
    }

    return instance;
}

void Settings::updateSettings(char *topic, char *payload)
{
    auto topicString = std::string(topic);
    bool changed = false;

    if (topicString.find("deviceTopic") != std::string::npos)
    {
        auto newDeviceTopic = std::string(payload);
        if (deviceTopic != newDeviceTopic)
        {
            changed = true;
            deviceTopic = newDeviceTopic;
        }
    }
    else if (topicString.find("wifiSsid") != std::string::npos)
    {
        auto newWifi_ssid = std::string(payload);
        if (wifi_ssid != newWifi_ssid)
        {
            changed = true;
            wifi_ssid = newWifi_ssid;
        }
    }
    else if (topicString.find("wifiPassword") != std::string::npos)
    {
        auto newWifi_password = std::string(payload);
        if (wifi_password != newWifi_password)
        {
            changed = true;
            wifi_password = newWifi_password;
        }
    }
    else if (topicString.find("mqttHost") != std::string::npos)
    {
        auto newMqttHost = IPAddress();
        newMqttHost.fromString(payload);
        if (mqtt_host != newMqttHost)
        {
            changed = true;
            mqtt_host = newMqttHost;
        }
    }
    else if (topicString.find("mqttPort") != std::string::npos)
    {
        uint newMqttPort = atoi(payload);
        if (mqtt_port != newMqttPort)
        {
            changed = true;
            mqtt_port = newMqttPort;
        }
    }
    else if (topicString.find("updateIp") != std::string::npos)
    {
        auto newUpdateIp = std::string(payload);
        if (updateIp != newUpdateIp)
        {
            changed = true;
            updateIp = newUpdateIp;
        }
    }
    else if (topicString.find("updatePort") != std::string::npos)
    {
        auto newUpdatePort = atoi(payload);
        if (updatePort != newUpdatePort)
        {
            changed = true;
            updatePort = newUpdatePort;
        }
    }
    else if (topicString.find("updateUrl") != std::string::npos)
    {
        auto newUpdateUrl = std::string(payload);
        if (updateUrl != newUpdateUrl)
        {
            changed = true;
            updateUrl = updateUrl;
        }
    }

    if (changed)
    {
        saveSettings();
    }
}
