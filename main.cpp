#include <SFML/Graphics.hpp>

#include "Config.h"
#include "Field.h"
#include "SimulationUI.h"
#include "StatisticsManager.h"
#include "PopulationProcessManager.h"

int main() {

    Config cfg;
    loadConfig("default.cfg", cfg);

    GameField field(cfg.fieldSize, 123);
    field.randomizeClusters(cfg.clusterCount, cfg.clusterSize, cfg.clusterBuffer);

    PopulationProcessManager sim(field, cfg);
    sim.initializePopulation();

    SimulationUI ui;
    StatisticsManager stats;

    sf::RenderWindow window({ 1200, 800 }, "Ecosystem Simulation");
    window.setFramerateLimit(60);

    sf::Clock clock;
    const float tickDelay = 0.1f;

    bool running = false;

    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) {

            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::MouseButtonPressed) {

                sf::Vector2f m = window.mapPixelToCoords({
                    e.mouseButton.x,
                    e.mouseButton.y
                    });

                if (ui.startPausePressed(m))
                    running = !running;

                if (ui.stepPressed(m))
                    sim.tick();

                if (ui.resetPressed(m)) {
                    sim.reset();
                    stats.reset();
                }
            }
        }

        // simulation step
        if (running && clock.getElapsedTime().asSeconds() > tickDelay) {
            sim.tick();
            clock.restart();
        }

        // статистика
        int prey = 0, pred = 0;

        for (auto& a : sim.getAgents()) {
            if (!a || !a->alive) continue;

            if (a->kind() == AgentKind::Prey) prey++;
            else pred++;
        }

        stats.update(prey, pred);

        // render
        window.clear(sf::Color::White);

        field.draw(window, sim.getAgents(), 10.f, 10.f);
        stats.draw(window, 600, 50, 500, 200);
        ui.draw(window);

        window.display();
    }

    return 0;
}