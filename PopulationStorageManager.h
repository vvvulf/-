#pragma once

#include "Field.h"
#include "Agent.h"
#include "AgentFactory.h"

#include <vector>
#include <memory>
#include <string>
#include <fstream>

class PopulationStorageManager {
public:
    PopulationStorageManager(
        GameField& field,
        std::vector<std::unique_ptr<Agent>>& agents
    );

    bool saveSimulation(const std::string& file) const;
    bool loadSimulation(const std::string& file);
    bool exportCSV(const std::string& file) const;

private:
    GameField& gf;
    std::vector<std::unique_ptr<Agent>>& agents;
    AgentFactory factory;
};