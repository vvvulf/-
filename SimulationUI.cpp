#include "SimulationUI.h"

SimulationUI::SimulationUI() {
    font.loadFromFile("arial.ttf");

    setupButton(startBtn, startText, "Start/Pause", 20, 20);
    setupButton(stepBtn, stepText, "Step", 20, 80);
    setupButton(resetBtn, resetText, "Reset", 20, 140);
}

void SimulationUI::setupButton(sf::RectangleShape& btn,
    sf::Text& text,
    const std::string& label,
    float x, float y)
{
    btn.setSize({ 120, 40 });
    btn.setPosition(x, y);
    btn.setFillColor(sf::Color(80, 80, 80));

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(x + 10, y + 10);
}

bool SimulationUI::startPausePressed(sf::Vector2f mouse) {
    return startBtn.getGlobalBounds().contains(mouse);
}

bool SimulationUI::stepPressed(sf::Vector2f mouse) {
    return stepBtn.getGlobalBounds().contains(mouse);
}

bool SimulationUI::resetPressed(sf::Vector2f mouse) {
    return resetBtn.getGlobalBounds().contains(mouse);
}

void SimulationUI::draw(sf::RenderWindow& window) const {
    window.draw(startBtn);
    window.draw(stepBtn);
    window.draw(resetBtn);

    window.draw(startText);
    window.draw(stepText);
    window.draw(resetText);
}