#include <ArduinoJson.h>
#include <FS.h>
#include <vector>

#include "IOManager.hpp"
#include "iopair.hpp"
#include "ioport.hpp"

IOManager &IOManager::the() {
    static IOManager *instance;
    if (instance == nullptr) {
        instance = new IOManager();
    }
    return *instance;
}

IOManager::IOManager() : ports({}), pairs({}) {}

void IOManager::setup() {

    DynamicJsonDocument configDoc(1024);

    if (!SPIFFS.exists("/ioconfig")) {
        Serial.println("Did not found io config file");
        return;
    }

    auto configFile = SPIFFS.open("/ioconfig", "r");
    DeserializationError error = deserializeJson(configDoc, configFile);

    configFile.close();

    if (error) {
        Serial.println("COULD NO DESERIALIZE CONFIGS");
        Serial.println(error.c_str());
        return;
    }

    ports.clear();
    pairs.clear();

    auto root = configDoc.as<JsonObjectConst>();

    const auto portsRoot = root["ports"].as<JsonArrayConst>();
    for (const auto &portRoot : portsRoot) {
        auto const portData = portRoot.as<JsonObjectConst>();
        const auto pin = portData["pin"].as<unsigned int>();
        const auto type =
            portData["type"].as<int>() ? IOType::input : IOType::output;

        const IOPort newPort(IODevice::local, pin, type);
        ports.push_back(newPort);
    }

    const auto pairsRoot = root["pairs"].as<JsonArrayConst>();

    for (const auto &pairRoot : pairsRoot) {
        auto const pairData = pairRoot.as<JsonObjectConst>();

        const auto name = pairData["name"].as<const char *>();

        std::vector<IOPort *> inputs;
        for (auto const port : pairData["inputPorts"].as<JsonArrayConst>()) {
            const auto index = port.as<int>();

            inputs.push_back(&ports[index]);
        }

        std::vector<IOPort *> outputs;
        for (auto const port : pairData["outputPorts"].as<JsonArrayConst>()) {
            const auto index = port.as<int>();
            outputs.push_back(&ports[index]);
        }

        IOPair newPair(std::move(inputs), std::move(outputs), name);
        pairs.push_back(std::move(newPair));
    }

    for (auto &pair : pairs) {
        pair.setup();
    }
}

void IOManager::loop() {
    for (auto &pair : pairs)
        pair.checkState();
}