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

        // якщо кл≥тинка в≥льна Ч будь-хто може туди п≥ти
        if (targetId == -1) {
            nx = p.first;
            ny = p.second;
            return true;
        }
        // якщо поточний агент Ч хижак, в≥н може випадково наступити на здобич ≥ з'њсти њњ
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

    // ЎукаЇмо найближчу живу здобич
    for (Agent* other : agents) {

        if (!other || !other->alive)
            continue;

        if (other->kind() != AgentKind::Prey)
            continue;

        int dx = other->x - agent.x;
        int dy = other->y - agent.y;

        int d2 = dx * dx + dy * dy;

        if (d2 > 100) // –ад≥ус зору (10 кл≥тинок, 10^2 = 100)
            continue;

        if (d2 < bestDist) {
            bestDist = d2;
            target = other;
        }
    }

    // якщо не знайшли здобич поруч Ч рухаЇмось випадково
    if (!target) {
        RandomMoveStrategy r;
        return r.decideMove(agent, field, agents, nx, ny);
    }

    // Ќапр€мок до ц≥л≥
    int sx = (target->x > agent.x) ? 1 : (target->x < agent.x) ? -1 : 0;
    int sy = (target->y > agent.y) ? 1 : (target->y < agent.y) ? -1 : 0;

    int tx = agent.x + sx;
    int ty = agent.y + sy;

    if (field.inBounds(tx, ty)) {
        int targetId = field.agentAt(tx, ty);

        //  л≥тинка в≥льна Ч крок дозволено
        if (targetId == -1) {
            nx = tx;
            ny = ty;
            return true;
        }
        //  л≥тинка зайн€та Ч перев≥р€Їмо, чи там наша њжа (здобич)
        else {
            for (Agent* other : agents) {
                if (other && other->id == targetId && other->alive && other->kind() == AgentKind::Prey) {
                    nx = tx;
                    ny = ty;
                    return true; // ƒозвол€Їмо наступити на кл≥тинку з≥ здобиччю!
                }
            }
        }
    }

    // Fallback рух: €кщо пр€мий шл€х заблокований ≥ншим хижаком, шукаЇмо обх≥дний в≥льний шл€х
    RandomMoveStrategy r;
    return r.decideMove(agent, field, agents, nx, ny);
}