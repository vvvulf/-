#pragma once
#include <vector>

class Agent;
class GameField;

class MoveStrategy {
public:
    virtual ~MoveStrategy() = default;

    virtual bool decideMove(
        Agent& agent,
        GameField& field,
        const std::vector<Agent*>& agents,
        int& nx,
        int& ny
    ) = 0;
};

class RandomMoveStrategy : public MoveStrategy {
public:
    bool decideMove(
        Agent& agent,
        GameField& field,
        const std::vector<Agent*>& agents,
        int& nx,
        int& ny
    ) override;
};

class ChasePreyStrategy : public MoveStrategy {
public:
    bool decideMove(
        Agent& agent,
        GameField& field,
        const std::vector<Agent*>& agents,
        int& nx,
        int& ny
    ) override;
};