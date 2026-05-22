#pragma once
#include <SFML/Graphics.hpp>

class SimulationUI {
public:
    SimulationUI();

    bool startPausePressed(sf::Vector2f mouse);
    bool stepPressed(sf::Vector2f mouse);
    bool resetPressed(sf::Vector2f mouse);

    void draw(sf::RenderWindow& window) const;

    bool isRunning() const { return running; }
    void toggleRunning() { running = !running; }

private:
    sf::RectangleShape startBtn;
    sf::RectangleShape stepBtn;
    sf::RectangleShape resetBtn;

    sf::Font font;
    sf::Text startText;
    sf::Text stepText;
    sf::Text resetText;

    bool running = false;

    void setupButton(sf::RectangleShape& btn,
        sf::Text& text,
        const std::string& label,
        float x, float y);
};