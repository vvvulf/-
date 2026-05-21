#pragma once
#include <memory>
#include "Agent.h"
#include "Prey.h"
#include "Predator.h"

class AgentFactory {
public:
    std::unique_ptr<Agent> create(
        AgentKind kind,
        int id,
        int subtype,
        int x,
        int y
    ) const {
        if (kind == AgentKind::Prey)
            return std::make_unique<Prey>(
                id,
                static_cast<PreySubtype>(subtype),
                x,
                y
            );

        return std::make_unique<Predator>(
            id,
            static_cast<PredatorSubtype>(subtype),
            x,
            y
        );
    }
};