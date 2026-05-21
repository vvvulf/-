#pragma once
#include "Agent.h"

class Prey : public Agent {
public:
    Prey(int id, PreySubtype subtype, int x, int y);

    AgentKind kind() const override {
        return AgentKind::Prey;
    }

    int subtype() const override {
        return static_cast<int>(subtypeVal);
    }

private:
    PreySubtype subtypeVal;
};