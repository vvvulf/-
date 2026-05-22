#include "Agent.h"
#include "MoveStrategy.h"
#include <chrono>

Agent::Agent(int id_, int px, int py)
    : id(id_), x(px), y(py) {}

bool Agent::decideMove(GameField& field,
    const std::vector<Agent*>& agents,
    int& nx, int& ny)
{
    if (!moveBehavior)
        return false;

    return moveBehavior->decideMove(*this, field, agents, nx, ny);
}

std::vector<std::pair<int, int>> Agent::neighbors8(int cx, int cy, int size) {
    std::vector<std::pair<int, int>> r;

    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++) {
            if (!dx && !dy) continue;

            int nx = cx + dx;
            int ny = cy + dy;

            if (nx >= 0 && ny >= 0 && nx < size && ny < size)
                r.push_back({ nx, ny });
        }

    return r;
}

std::mt19937& Agent::rng() {
    static std::mt19937 gen(
        (unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    return gen;
}