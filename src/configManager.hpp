#pragma once

#include <vector>

#include "iopair.hpp"
#include "ioport.hpp"

class ConfigManager {
  public:
    ConfigManager();

    bool loadConfiguration(const std::string &configuration);

  private:
    std::vector<IOPort> ports;
    std::vector<IOPair> pairs;
};