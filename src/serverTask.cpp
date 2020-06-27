#include <ESP8266WebServer.h>
#include <FS.h>

#include "IOManager.hpp"
#include "configPage.h"

ESP8266WebServer server(80);

void handleGetSite() {
    server.send(200, "text/html",
                reinterpret_cast<const char *>(src_configPage_html),
                src_configPage_html_len);
}

void handleGetConfig() {
    auto configFile = SPIFFS.open("/ioconfig", "r");
    const auto config = configFile.readString();
    server.send(200, "application/json", config);
    configFile.close();
}

void handleGetSettings() {
    auto settingsFile = SPIFFS.open("/settings", "r");
    const auto settings = settingsFile.readString();
    server.send(200, "application/json", settings);
    settingsFile.close();
}

void handlePostIo() {
    auto postBody = server.arg("plain");
    auto newConfigFile = SPIFFS.open("/ioconfig", "w");
    newConfigFile.print(postBody);
    newConfigFile.close();

    Serial.print(postBody);

    IOManager::the().setup();
}

void handlePostSettings() {
    auto postBody = server.arg("plain");
    auto newConfigFile = SPIFFS.open("/settings", "w");
    newConfigFile.print(postBody);
    newConfigFile.close();

    Serial.print(postBody);
    SPIFFS.end();
    ESP.reset();
}

void setupServer() {
    server.on("/", HTTP_GET, handleGetSite);

    server.on("/getioconfig", HTTP_GET, handleGetConfig);
    server.on("/getsettings", HTTP_GET, handleGetSettings);

    server.on("/setioconfig", HTTP_POST, handlePostIo);
    server.on("/setsettings", HTTP_POST, handlePostSettings);

    server.begin();
}

void serverTask() { server.handleClient(); }