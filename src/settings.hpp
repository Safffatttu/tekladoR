#pragma once

#include <ESP8266WiFi.h>
#include <FS.h>

class Settings {
private:
	Settings();
	void saveSettings();
	void loadSettings();
	void loadDefaults();
	static Settings *instance;

public:
	static Settings *getInstance();

	std::string deviceTopic;
	std::string wifi_ssid;
	std::string wifi_password;
	IPAddress mqtt_host;
	uint mqtt_port;
	const std::string reconfigurePass = "";
};