#include "Predator.h"
#include "MoveStrategy.h"

Predator::Predator(int id, PredatorSubtype st, int x, int y)
    : Agent(id, x, y), subtypeVal(st)
{
    energy = 2.0f;
    moveBehavior = std::make_unique<ChasePreyStrategy>();
}