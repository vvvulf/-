#include "ConfigEditor.h"
#include <sstream>

ConfigEditor::ConfigEditor(Config& cfg)
    : config(cfg)
{
    font.loadFromFile("arial.ttf");

    text.setFont(font);
    text.setCharacterSize(16);
    text.setPosition(835.f, 520.f);
}

void ConfigEditor::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed)
        return;

    switch (event.key.code) {

    case sf::Keyboard::F1:
        config.initialPreyRabbit = std::min(config.initialPreyRabbit + 5, 500);
        break;

    case sf::Keyboard::F2:
        config.initialPredatorWolf = std::min(config.initialPredatorWolf + 2, 200);
        break;

    case sf::Keyboard::F3:
        config.grassRegrowthPerTick = std::min(config.grassRegrowthPerTick + 0.001f, 0.2f);
        break;

    case sf::Keyboard::F4:
        config.metabolismCost = std::min(config.metabolismCost + 0.001f, 0.2f);
        break;

    default:
        break;
    }
}

void ConfigEditor::draw(sf::RenderWindow& window) {
    sf::RectangleShape panel({ 280.f, 320.f });
    panel.setPosition(820.f, 500.f);
    panel.setFillColor(sf::Color(55, 55, 55));

    window.draw(panel);

    std::stringstream ss;

    ss << "CONFIG EDITOR\n\n";
    ss << "F1 Rabbits +5\n";
    ss << "F2 Wolves +2\n";
    ss << "F3 Grass++\n";
    ss << "F4 Metabolism++\n\n";

    ss << "Rabbits: " << config.initialPreyRabbit << "\n";
    ss << "Wolves: " << config.initialPredatorWolf << "\n";
    ss << "Grass: " << config.grassRegrowthPerTick << "\n";
    ss << "Metabolism: " << config.metabolismCost << "\n";

    text.setString(ss.str());
    window.draw(text);
}