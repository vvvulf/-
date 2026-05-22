#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"

struct ClickButton {
    sf::FloatRect rect;
    std::string action;
};

class SimulationUI {
public:
    SimulationUI();

    bool startPausePressed(sf::Vector2f m);
    bool stepPressed(sf::Vector2f m);
    bool resetPressed(sf::Vector2f m);
    bool regenPressed(sf::Vector2f m); 

    void handleConfigInteraction(sf::Vector2f m, Config& cfg);
    void draw(sf::RenderWindow& window, const Config& cfg, int preyCount, int predCount);

private:
    sf::Font font;
    std::vector<ClickButton> buttons;

    sf::RectangleShape btnStart;
    sf::RectangleShape btnStep;
    sf::RectangleShape btnReset;
    sf::RectangleShape btnRegen; 

    sf::Text txtStart;
    sf::Text txtStep;
    sf::Text txtReset;
    sf::Text txtRegen; 

    void setupConfigButtons();
};