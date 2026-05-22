#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"

class ConfigEditor {
public:
    ConfigEditor(Config& cfg);

    void handleEvent(const sf::Event& e);
    void draw(sf::RenderWindow& w);

private:
    Config& config;

    sf::Font font;
    sf::Text text;
};