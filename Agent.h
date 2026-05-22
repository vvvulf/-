#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <random>

class MoveStrategy;
class GameField;

enum class AgentKind {
    Prey,
    Predator
};

enum class PreySubtype {
    Sheep,
    Deer,
    Rabbit
};

enum class PredatorSubtype {
    Bear,
    Wolf,
    Fox
};

class Agent {
public:
    Agent(int id, int px, int py);

    virtual ~Agent() = default;

    int id;   // ™ƒ»Õ»… ID
    int x;
    int y;

    bool alive = true;
    float energy = 0.f;

    virtual AgentKind kind() const = 0;
    virtual int subtype() const = 0;

    bool decideMove(
        GameField& field,
        const std::vector<Agent*>& agents,
        int& nx,
        int& ny
    );

    static std::vector<std::pair<int, int>> neighbors8(int x, int y, int size);
    static std::mt19937& rng();

protected:
    std::unique_ptr<MoveStrategy> moveBehavior;
};