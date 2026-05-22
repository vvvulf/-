#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Agent.h"

class StatisticsManager {
public:
    void update(int prey, int pred);
    void reset();
    void draw(sf::RenderWindow& window, float x, float y, float w, float h, const std::vector<std::unique_ptr<Agent>>& agents);

private:
    std::vector<int> preyHistory;
    std::vector<int> predHistory;
    sf::Font font;
};