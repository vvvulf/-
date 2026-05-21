#include "Prey.h"
#include "MoveStrategy.h"

Prey::Prey(int id, PreySubtype st, int x, int y)
    : Agent(id, x, y), subtypeVal(st)
{
    energy = 1.0f;
    moveBehavior = std::make_unique<RandomMoveStrategy>();
}