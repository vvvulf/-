#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <memory>

class Agent;

struct Cell {
    float food = 0.f;

    sf::Color color() const {
        return sf::Color(
            50,
            static_cast<sf::Uint8>(80 + food * 175),
            50
        );
    }
};

class GameField {
public:
    GameField(int size, unsigned seed = std::random_device{}());

    int size() const;

    bool inBounds(int x, int y) const;

    bool isFreeAt(int x, int y) const;

    void placeAgent(int id, int x, int y);
    void removeAgentFrom(int x, int y);

    int agentAt(int x, int y) const;

    void randomizeClusters(int count, int size, int buffer);

    void eatFood(int x, int y, float amount = 1.0f);
    float foodAt(int x, int y) const;
    void clear() {
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                field[y][x].food = 0.f;
            }
        }

        for (int y = 0; y < n; y++) {
            std::fill(grid[y].begin(), grid[y].end(), -1);
        }
    }

    void draw(sf::RenderWindow&,
        const std::vector<std::unique_ptr<Agent>>&,
        float ox,
        float oy) const;

private:
    int n;
    std::vector<std::vector<Cell>> field;
    std::vector<std::vector<int>> grid;
    std::mt19937 rng;
};