#include "MoveStrategy.h"
#include "Agent.h"
#include "Field.h"

#include <random>
#include <limits>
#include <algorithm>

bool RandomMoveStrategy::decideMove(
    Agent& agent,
    GameField& field,
    const std::vector<Agent*>& agents,
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
        int targetId = field.agentAt(p.first, p.second);

        if (targetId == -1) {
            nx = p.first;
            ny = p.second;
            return true;
        }
        
        else if (agent.kind() == AgentKind::Predator) {
            for (Agent* other : agents) {
                if (other && other->id == targetId && other->alive && other->kind() == AgentKind::Prey) {
                    nx = p.first;
                    ny = p.second;
                    return true;
                }
            }
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

    for (Agent* other : agents) {

        if (!other || !other->alive)
            continue;

        if (other->kind() != AgentKind::Prey)
            continue;

        int dx = other->x - agent.x;
        int dy = other->y - agent.y;

        int d2 = dx * dx + dy * dy;

        if (d2 > 100) 
            continue;

        if (d2 < bestDist) {
            bestDist = d2;
            target = other;
        }
    }

    if (!target) {
        RandomMoveStrategy r;
        return r.decideMove(agent, field, agents, nx, ny);
    }

    int sx = (target->x > agent.x) ? 1 : (target->x < agent.x) ? -1 : 0;
    int sy = (target->y > agent.y) ? 1 : (target->y < agent.y) ? -1 : 0;

    int tx = agent.x + sx;
    int ty = agent.y + sy;

    if (field.inBounds(tx, ty)) {
        int targetId = field.agentAt(tx, ty);

        if (targetId == -1) {
            nx = tx;
            ny = ty;
            return true;
        }
        
        else {
            for (Agent* other : agents) {
                if (other && other->id == targetId && other->alive && other->kind() == AgentKind::Prey) {
                    nx = tx;
                    ny = ty;
                    return true; 
                }
            }
        }
    }

    RandomMoveStrategy r;
    return r.decideMove(agent, field, agents, nx, ny);
}