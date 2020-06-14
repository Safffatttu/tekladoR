#include <ArduinoJson.h>
#include <vector>

#include "configManager.hpp"
#include "iopair.hpp"
#include "ioport.hpp"

ConfigManager::ConfigManager() : ports({}), pairs({}) { loadConfiguration(""); }

bool ConfigManager::loadConfiguration(const std::string &configuration) {
    DynamicJsonDocument configDoc(1024);

    DeserializationError error = deserializeJson(configDoc, configuration);

    if (error) {
        return false;
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

    const auto pairRoot = root["ports"].as<JsonArrayConst>();
    for (const auto &pairRoot : pairRoot) {
        auto const pairData = pairRoot.as<JsonObjectConst>();

        const auto &name = pairData["name"].as<std::string>();

        std::vector<IOPort *> inputs{0};
        for (auto const port : pairData["inputPorts"].as<JsonArrayConst>()) {
            const auto index = port.as<int>();

            inputs.push_back(&ports[index]);
        }

        std::vector<IOPort *> outputs{0};
        for (auto const port : pairData["inputPorts"].as<JsonArrayConst>()) {
            const auto index = port.as<int>();

            outputs.push_back(&ports[index]);
        }

        IOPair(std::move(inputs), std::move(outputs), name);
    }

    return true;
}