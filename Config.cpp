#include "Config.h"
#include <fstream>
#include <string>

static float clampFloat(float v, float minV, float maxV) {
    return (v < minV) ? minV : (v > maxV ? maxV : v);
}

bool loadConfig(const char* filename, Config& cfg) {
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    std::string key;

    while (in >> key) {

        if (key == "fieldSize") in >> cfg.fieldSize;

        else if (key == "clusterCount") in >> cfg.clusterCount;
        else if (key == "clusterSize") in >> cfg.clusterSize;
        else if (key == "clusterBuffer") in >> cfg.clusterBuffer;

        else if (key == "initialPreyRabbit") in >> cfg.initialPreyRabbit;
        else if (key == "initialPreySheep") in >> cfg.initialPreySheep;

        else if (key == "initialPredatorWolf") in >> cfg.initialPredatorWolf;
        else if (key == "initialPredatorFox") in >> cfg.initialPredatorFox;

        else if (key == "metabolismCost") {
            in >> cfg.metabolismCost;
            cfg.metabolismCost = clampFloat(cfg.metabolismCost, 0.0f, 1.0f);
        }

        else if (key == "grassRegrowthPerTick") {
            in >> cfg.grassRegrowthPerTick;
            cfg.grassRegrowthPerTick = clampFloat(cfg.grassRegrowthPerTick, 0.0f, 1.0f);
        }
    }

    return true;
}