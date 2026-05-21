#pragma once

struct Config {
    int fieldSize = 50;

    int clusterCount = 8;
    int clusterSize = 20;
    int clusterBuffer = 3;

    int initialPreyRabbit = 30;
    int initialPreySheep = 10;

    int initialPredatorWolf = 5;
    int initialPredatorFox = 3;

    float preyReproduceThreshold = 3.0f;
    float predatorReproduceThreshold = 5.0f;

    float metabolismCost = 0.02f;
    float grassRegrowthPerTick = 0.005f;
};

bool loadConfig(const char* filename, Config& cfg);