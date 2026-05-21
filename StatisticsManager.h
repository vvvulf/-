#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class StatisticsManager {
public:
    void update(int prey, int pred);
    void reset();

    void draw(sf::RenderWindow& window,
        float x, float y,
        float w, float h);

private:
    std::vector<int> prey;
    std::vector<int> pred;

    const size_t maxPoints = 500;
};