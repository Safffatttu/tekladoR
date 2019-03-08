#include <update.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClient.h>
#include <settings.hpp>

#ifndef GIT_HASH
#define GIT_HASH "1"
#endif

void updateAndRebot()
{
    ESPhttpUpdate.rebootOnUpdate(true);

    auto client = WiFiClient();
    client.connect(
        String(Settings::getInstance()->updateIp.c_str()),
        Settings::getInstance()->updatePort);

    ESPhttpUpdate.update(
        client,
        String(Settings::getInstance()->updateUrl.c_str()),
        String(GIT_HASH));
}