#pragma once

#include "Field.h"
#include "Agent.h"
#include "Config.h"

#include <vector>
#include <memory>

class PopulationProcessManager {
public:
    PopulationProcessManager(GameField& field, const Config& cfg);

    void initializePopulation();
    void tick();
    void reset();

    std::vector<std::unique_ptr<Agent>>& getAgents();

private:
    GameField& gf;
    Config cfg;

    std::vector<std::unique_ptr<Agent>> agents;
    int nextId = 0;

    void handleMovements();
    void handleLifecycle();
};