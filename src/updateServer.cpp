#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

#include "settings.hpp"
#include "updateServer.hpp"

ESP8266WebServer web_server(80);
ESP8266HTTPUpdateServer update_server(true);

void startUpdateServer() {
    MDNS.begin(Settings::getInstance()->mDNShostName.c_str());
    web_server.begin();
    update_server.setup(&web_server);

    MDNS.addService("http", "tcp", 80);
}

void serverTask() {
    web_server.handleClient();
    MDNS.update();
}