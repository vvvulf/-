#include "PopulationProcessManager.h"

#include "Prey.h"
#include "Predator.h"

#include <algorithm>
#include <random>

PopulationProcessManager::PopulationProcessManager(
    GameField& field,
    const Config& cfg_
)
    : gf(field), cfg(cfg_) {}

void PopulationProcessManager::initializePopulation() {

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, gf.size() - 1);

    auto spawn = [&](AgentKind kind, int subtype, int count) {

        for (int i = 0; i < count; i++) {

            for (int t = 0; t < 200; t++) {

                int x = dist(rng);
                int y = dist(rng);

                if (!gf.isFreeAt(x, y))
                    continue;

                std::unique_ptr<Agent> a;

                if (kind == AgentKind::Prey)
                    a = std::make_unique<Prey>(
                        nextId++,
                        (PreySubtype)subtype,
                        x,
                        y
                    );
                else
                    a = std::make_unique<Predator>(
                        nextId++,
                        (PredatorSubtype)subtype,
                        x,
                        y
                    );

                // FIX: ÍÅ idx, à id + position only
                gf.setAgentAt(x, y, a->id);

                agents.push_back(std::move(a));
                break;
            }
        }
        };

    spawn(AgentKind::Prey, 0, cfg.initialPreyRabbit);
    spawn(AgentKind::Prey, 1, cfg.initialPreySheep);

    spawn(AgentKind::Predator, 0, cfg.initialPredatorWolf);
    spawn(AgentKind::Predator, 1, cfg.initialPredatorFox);
}

void PopulationProcessManager::handleMovements() {

    std::vector<Agent*> view;
    view.reserve(agents.size());

    for (auto& a : agents)
        view.push_back(a.get());

    for (Agent* a : view) {

        if (!a || !a->alive)
            continue;

        int nx = a->x;
        int ny = a->y;

        if (!a->decideMove(gf, view, nx, ny))
            continue;

        if (!gf.inBounds(nx, ny))
            continue;

        // EATING LOGIC
        int targetId = gf.agentAt(nx, ny);

        if (targetId != -1) {
            for (auto& other : agents) {
                if (!other || !other->alive)
                    continue;

                if (other->id == targetId && other->kind() != a->kind()) {
                    other->alive = false;
                    a->energy += 2.0f;
                    gf.removeAgentFrom(nx, ny);
                    break;
                }
            }
        }

        gf.removeAgentFrom(a->x, a->y);

        a->x = nx;
        a->y = ny;

        gf.setAgentAt(nx, ny, a->id);
    }
}

void PopulationProcessManager::handleLifecycle() {

    std::vector<std::unique_ptr<Agent>> newborns;

    for (auto& a : agents) {

        if (!a || !a->alive)
            continue;

        // 1. âèòðàòà åíåðã³¿
        a->energy -= cfg.metabolismCost;

        // 2. ÐÅÏÐÎÄÓÊÖ²ß
        if (a->energy > cfg.predatorReproduceThreshold) {

            a->energy *= 0.5f;

            std::unique_ptr<Agent> child;

            if (a->kind() == AgentKind::Prey) {
                child = std::make_unique<Prey>(
                    nextId++,
                    PreySubtype::Rabbit,
                    a->x,
                    a->y
                );
            }
            else {
                child = std::make_unique<Predator>(
                    nextId++,
                    PredatorSubtype::Wolf,
                    a->x,
                    a->y
                );
            }

            child->energy = a->energy;
            newborns.push_back(std::move(child));
        }

        // 3. ÑÌÅÐÒÜ
        if (a->energy <= 0.f) {
            a->alive = false;
            gf.removeAgentFrom(a->x, a->y);
        }
    }

    // äîäàºìî íîâèõ Ï²ÑËß öèêëó (ÂÀÆËÈÂÎ!)
    for (auto& n : newborns)
        agents.push_back(std::move(n));
}

void PopulationProcessManager::reset() {

    for (auto& a : agents)
        if (a && a->alive)
            gf.removeAgentFrom(a->x, a->y);

    agents.clear();
    nextId = 0;

    initializePopulation();
}

void PopulationProcessManager::tick() {
    handleMovements();
    handleLifecycle();
}

std::vector<std::unique_ptr<Agent>>&
PopulationProcessManager::getAgents() {
    return agents;
}