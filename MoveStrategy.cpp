#include "MoveStrategy.h"
#include "Agent.h"
#include "Field.h"

#include <random>
#include <limits>
#include <algorithm>

bool RandomMoveStrategy::decideMove(
    Agent& agent,
    GameField& field,
    const std::vector<Agent*>&,
    int& nx,
    int& ny
) {
    auto neighbors = Agent::neighbors8(
        agent.x,
        agent.y,
        field.size()
    );

    std::shuffle(neighbors.begin(), neighbors.end(), Agent::rng());

    for (auto& p : neighbors) {

        if (field.isFreeAt(p.first, p.second)) {
            nx = p.first;
            ny = p.second;
            return true;
        }
    }

    return false;
}

bool ChasePreyStrategy::decideMove(
    Agent& agent,
    GameField& field,
    const std::vector<Agent*>& agents,
    int& nx,
    int& ny
) {
    Agent* target = nullptr;
    int bestDist = std::numeric_limits<int>::max();

    // шукаємо найближчу здобич
    for (Agent* other : agents) {

        if (!other || !other->alive)
            continue;

        if (other->kind() != AgentKind::Prey)
            continue;

        int dx = other->x - agent.x;
        int dy = other->y - agent.y;

        int d2 = dx * dx + dy * dy;

        if (d2 > 100) // радіус
            continue;

        if (d2 < bestDist) {
            bestDist = d2;
            target = other;
        }
    }

    // якщо не знайшли — random
    if (!target) {
        RandomMoveStrategy r;
        return r.decideMove(agent, field, agents, nx, ny);
    }

    //  напрямок до цілі
    int sx =
        (target->x > agent.x) ? 1 :
        (target->x < agent.x) ? -1 : 0;

    int sy =
        (target->y > agent.y) ? 1 :
        (target->y < agent.y) ? -1 : 0;

    int tx = agent.x + sx;
    int ty = agent.y + sy;

    //  FIX №1: перевірка не тільки bounds, а й зайнятості
    if (field.inBounds(tx, ty) && field.isFreeAt(tx, ty)) {
        nx = tx;
        ny = ty;
        return true;
    }

    //  FIX №2: fallback якщо клітинка зайнята
    RandomMoveStrategy r;
    return r.decideMove(agent, field, agents, nx, ny);
}

