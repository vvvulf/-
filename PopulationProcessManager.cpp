#include "PopulationProcessManager.h"

#include "Prey.h"
#include "Predator.h"

#include <random>

PopulationProcessManager::PopulationProcessManager(
    GameField& field,
    const Config& cfg
)
    : gf(field), cfg(cfg)
{
}

void PopulationProcessManager::initializePopulation() {

    std::uniform_int_distribution<int>
        dist(0, gf.size() - 1);

    auto spawn =
        [&](AgentKind kind, int subtype, int count)
        {
            for (int i = 0; i < count; i++) {

                for (int t = 0; t < 500; t++) {

                    int x = dist(Agent::rng());
                    int y = dist(Agent::rng());

                    if (!gf.isFreeAt(x, y))
                        continue;

                    std::unique_ptr<Agent> a;

                    if (kind == AgentKind::Prey) {

                        a = std::make_unique<Prey>(
                            nextId,
                            (PreySubtype)subtype,
                            x,
                            y
                        );
                    }
                    else {

                        a = std::make_unique<Predator>(
                            nextId,
                            (PredatorSubtype)subtype,
                            x,
                            y
                        );
                    }

                    gf.placeAgent(nextId, x, y);

                    agents.push_back(std::move(a));

                    nextId++;

                    break;
                }
            }
        };

    spawn(AgentKind::Prey, 0, cfg.initialPreyRabbit);
    spawn(AgentKind::Prey, 1, cfg.initialPreySheep);

    spawn(AgentKind::Predator, 1, cfg.initialPredatorWolf);
    spawn(AgentKind::Predator, 2, cfg.initialPredatorFox);
}

void PopulationProcessManager::tick() {

    handleMovements();
    handleLifecycle();
}

void PopulationProcessManager::reset() {

    for (auto& a : agents) {

        if (a && a->alive)
            gf.removeAgentFrom(a->x, a->y);
    }

    agents.clear();

    nextId = 0;

    initializePopulation();
}

void PopulationProcessManager::handleMovements() {

    std::vector<Agent*> view;

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

        // ====================================
        // PREY
        // ====================================

        if (a->kind() == AgentKind::Prey) {

            if (!gf.isFreeAt(nx, ny))
                continue;

            gf.removeAgentFrom(a->x, a->y);

            a->x = nx;
            a->y = ny;

            gf.placeAgent(a->id, nx, ny);

            // їсть траву
            if (gf.foodAt(nx, ny) > 0.f) {

                a->energy += 0.2f;

                gf.eatFood(nx, ny, 0.3f);
            }
        }

        // ====================================
        // PREDATOR
        // ====================================

        else {

            int victimId = gf.agentAt(nx, ny);

            // ====================================
            // якщо там prey -> з'їсти
            // ====================================

            if (victimId >= 0 &&
                victimId < agents.size())
            {
                Agent* victim = agents[victimId].get();

                if (victim &&
                    victim->alive &&
                    victim->kind() == AgentKind::Prey)
                {
                    // prey помирає
                    victim->alive = false;

                    gf.removeAgentFrom(
                        victim->x,
                        victim->y
                    );

                    // predator рухається
                    gf.removeAgentFrom(a->x, a->y);

                    a->x = nx;
                    a->y = ny;

                    gf.placeAgent(a->id, nx, ny);

                    // енергія
                    a->energy += 2.0f;

                    continue;
                }
            }

            // ====================================
            // звичайний рух
            // ====================================

            if (gf.isFreeAt(nx, ny)) {

                gf.removeAgentFrom(a->x, a->y);

                a->x = nx;
                a->y = ny;

                gf.placeAgent(a->id, nx, ny);
            }
        }
    }
}

void PopulationProcessManager::handleLifecycle() {

    std::vector<std::unique_ptr<Agent>> newborns;

    for (auto& a : agents) {

        if (!a || !a->alive)
            continue;

        // витрата енергії
        a->energy -= cfg.metabolismCost;

        // смерть
        if (a->energy <= 0.f) {

            a->alive = false;

            gf.removeAgentFrom(
                a->x,
                a->y
            );

            continue;
        }

        // ==========================
        // REPRODUCTION
        // ==========================

        bool canReproduce = false;

        if (a->kind() == AgentKind::Prey &&
            a->energy >= cfg.preyReproduceThreshold)
        {
            canReproduce = true;
        }

        if (a->kind() == AgentKind::Predator &&
            a->energy >= cfg.predatorReproduceThreshold)
        {
            canReproduce = true;
        }

        if (!canReproduce)
            continue;

        auto neighbors =
            Agent::neighbors8(
                a->x,
                a->y,
                gf.size()
            );

        for (auto& p : neighbors) {

            int nx = p.first;
            int ny = p.second;

            if (!gf.isFreeAt(nx, ny))
                continue;

            std::unique_ptr<Agent> child;

            if (a->kind() == AgentKind::Prey) {

                child = std::make_unique<Prey>(
                    nextId,
                    (PreySubtype)a->subtype(),
                    nx,
                    ny
                );

                a->energy *= 0.5f;
            }
            else {

                child = std::make_unique<Predator>(
                    nextId,
                    (PredatorSubtype)a->subtype(),
                    nx,
                    ny
                );

                a->energy *= 0.5f;
            }

            gf.placeAgent(nextId, nx, ny);

            newborns.push_back(std::move(child));

            nextId++;

            break;
        }
    }

    for (auto& n : newborns)
        agents.push_back(std::move(n));
}

std::vector<std::unique_ptr<Agent>>&
PopulationProcessManager::getAgents() {

    return agents;
}