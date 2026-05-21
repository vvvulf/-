#pragma once
#include "Agent.h"

class Predator : public Agent {
public:
    Predator(int id, PredatorSubtype subtype, int x, int y);

    AgentKind kind() const override {
        return AgentKind::Predator;
    }

    int subtype() const override {
        return static_cast<int>(subtypeVal);
    }

private:
    PredatorSubtype subtypeVal;
};