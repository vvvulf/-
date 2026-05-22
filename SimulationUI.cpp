#include "SimulationUI.h"
#include <sstream>
#include <iomanip>

SimulationUI::SimulationUI() {
    font.loadFromFile("arial.ttf");

    btnStart.setSize({ 100.f, 35.f });
    btnStart.setPosition(20.f, 540.f);
    btnStart.setFillColor(sf::Color(100, 200, 100));

    btnStep.setSize({ 100.f, 35.f });
    btnStep.setPosition(130.f, 540.f);
    btnStep.setFillColor(sf::Color(100, 150, 255));

    btnReset.setSize({ 100.f, 35.f });
    btnReset.setPosition(240.f, 540.f);
    btnReset.setFillColor(sf::Color(255, 100, 100));

    btnRegen.setSize({ 100.f, 35.f });
    btnRegen.setPosition(350.f, 540.f);
    btnRegen.setFillColor(sf::Color(200, 100, 200));

    txtStart = sf::Text("Start/Pause", font, 14); txtStart.setPosition(25.f, 548.f); txtStart.setFillColor(sf::Color::Black);
    txtStep = sf::Text("One Step", font, 14);   txtStep.setPosition(145.f, 548.f);  txtStep.setFillColor(sf::Color::Black);
    txtReset = sf::Text("Reset", font, 14);     txtReset.setPosition(270.f, 548.f); txtReset.setFillColor(sf::Color::Black);
    txtRegen = sf::Text("Regen Field", font, 14); txtRegen.setPosition(357.f, 548.f); txtRegen.setFillColor(sf::Color::Black);

    setupConfigButtons();
}

void SimulationUI::setupConfigButtons() {
    buttons.clear();
    float startY = 465.f;
    float rowHeight = 26.f;

    for (int i = 0; i < 6; i++) {
        float y = startY + i * rowHeight;
        buttons.push_back({ {930.f, y, 25.f, 20.f}, "minus_" + std::to_string(i) });
        buttons.push_back({ {1000.f, y, 25.f, 20.f}, "plus_" + std::to_string(i) });
    }

    buttons.push_back({ {560.f, 740.f, 130.f, 35.f}, "save" });
    buttons.push_back({ {710.f, 740.f, 130.f, 35.f}, "load" });
}

bool SimulationUI::startPausePressed(sf::Vector2f m) { return btnStart.getGlobalBounds().contains(m); }
bool SimulationUI::stepPressed(sf::Vector2f m) { return btnStep.getGlobalBounds().contains(m); }
bool SimulationUI::resetPressed(sf::Vector2f m) { return btnReset.getGlobalBounds().contains(m); }
bool SimulationUI::regenPressed(sf::Vector2f m) { return btnRegen.getGlobalBounds().contains(m); }

void SimulationUI::handleConfigInteraction(sf::Vector2f m, Config& cfg) {
    for (auto& btn : buttons) {
        if (btn.rect.contains(m)) {
            if (btn.action == "save") saveConfig("default.cfg", cfg);
            if (btn.action == "load") loadConfig("default.cfg", cfg);

            if (btn.action == "minus_0") cfg.initialPreyRabbit = std::max(0, cfg.initialPreyRabbit - 5);
            if (btn.action == "plus_0")  cfg.initialPreyRabbit = std::min(500, cfg.initialPreyRabbit + 5);

            if (btn.action == "minus_1") cfg.initialPreySheep = std::max(0, cfg.initialPreySheep - 5);
            if (btn.action == "plus_1")  cfg.initialPreySheep = std::min(500, cfg.initialPreySheep + 5);

            if (btn.action == "minus_2") cfg.initialPredatorWolf = std::max(0, cfg.initialPredatorWolf - 2);
            if (btn.action == "plus_2")  cfg.initialPredatorWolf = std::min(200, cfg.initialPredatorWolf + 2);

            if (btn.action == "minus_3") cfg.initialPredatorFox = std::max(0, cfg.initialPredatorFox - 2);
            if (btn.action == "plus_3")  cfg.initialPredatorFox = std::min(200, cfg.initialPredatorFox + 2);

            if (btn.action == "minus_4") cfg.metabolismCost = std::max(0.001f, cfg.metabolismCost - 0.005f);
            if (btn.action == "plus_4")  cfg.metabolismCost = std::min(0.5f, cfg.metabolismCost + 0.005f);

            if (btn.action == "minus_5") cfg.preyReproduceThreshold = std::max(1.f, cfg.preyReproduceThreshold - 0.5f);
            if (btn.action == "plus_5")  cfg.preyReproduceThreshold = std::min(10.f, cfg.preyReproduceThreshold + 0.5f);
            return;
        }
    }
}

void SimulationUI::draw(sf::RenderWindow& window, const Config& cfg, int preyCount, int predCount) {
    sf::RectangleShape fieldFrame({ 502.f, 502.f });
    fieldFrame.setPosition(9.f, 9.f);
    fieldFrame.setFillColor(sf::Color::Transparent);
    fieldFrame.setOutlineThickness(2.f);
    fieldFrame.setOutlineColor(sf::Color(50, 50, 50));
    window.draw(fieldFrame);

    window.draw(btnStart); window.draw(txtStart);
    window.draw(btnStep);  window.draw(txtStep);
    window.draw(btnReset); window.draw(txtReset);
    window.draw(btnRegen); window.draw(txtRegen);

    std::stringstream ss;
    ss << "=== LIVE DIGITAL STATISTICS ===\n"
        << "Total Live Prey: " << preyCount << "\n"
        << "Total Live Predators: " << predCount << "\n"
        << "Ecosystem Balance: " << (predCount == 0 ? 0 : preyCount / (float)predCount) << " prey/pred";

    sf::Text statsTxt(ss.str(), font, 15);
    statsTxt.setPosition(20.f, 600.f);
    statsTxt.setFillColor(sf::Color::Black);
    window.draw(statsTxt);

    sf::RectangleShape panel({ 600.f, 350.f });
    panel.setPosition(540.f, 430.f);
    panel.setFillColor(sf::Color(230, 230, 230));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(70, 70, 70));
    window.draw(panel);

    sf::Text panelTitle("Live Ecosystem Configuration Editor", font, 14);
    panelTitle.setFillColor(sf::Color::Blue);
    panelTitle.setPosition(550.f, 435.f);
    window.draw(panelTitle);

    std::string paramNames[6] = {
        "Initial Rabbits", "Initial Sheep", "Initial Wolves",
        "Initial Foxes", "Metabolism Cost", "Prey Repr. Threshold"
    };
    std::string paramValues[6] = {
        std::to_string(cfg.initialPreyRabbit), std::to_string(cfg.initialPreySheep),
        std::to_string(cfg.initialPredatorWolf), std::to_string(cfg.initialPredatorFox),
        std::to_string(cfg.metabolismCost).substr(0, 5), std::to_string(cfg.preyReproduceThreshold).substr(0, 4)
    };

    float startY = 465.f;
    float rowHeight = 26.f;

    for (int i = 0; i < 6; i++) {
        float y = startY + i * rowHeight;

        sf::Text nameText(paramNames[i], font, 13);
        nameText.setFillColor(sf::Color::Black);
        nameText.setPosition(560.f, y);
        window.draw(nameText);

        sf::RectangleShape bMin({ 25.f, 20.f }); bMin.setPosition(930.f, y); bMin.setFillColor(sf::Color(200, 200, 200));
        sf::Text tMin("-", font, 14); tMin.setPosition(939.f, y - 2); tMin.setFillColor(sf::Color::Black);
        window.draw(bMin); window.draw(tMin);

        sf::Text valText(paramValues[i], font, 13);
        valText.setFillColor(sf::Color::Black);
        valText.setPosition(965.f, y);
        window.draw(valText);

        sf::RectangleShape bPlu({ 25.f, 20.f }); bPlu.setPosition(1000.f, y); bPlu.setFillColor(sf::Color(200, 200, 200));
        sf::Text tPlu("+", font, 14); tPlu.setPosition(1007.f, y - 2); tPlu.setFillColor(sf::Color::Black);
        window.draw(bPlu); window.draw(tPlu);
    }

    sf::RectangleShape bSave({ 130.f, 35.f }); bSave.setPosition(560.f, 740.f); bSave.setFillColor(sf::Color(210, 180, 140));
    sf::Text tSave("Save Config", font, 13); tSave.setPosition(585.f, 745.f); tSave.setFillColor(sf::Color::Black);
    window.draw(bSave); window.draw(tSave);

    sf::RectangleShape bLoad({ 130.f, 30.f }); bLoad.setPosition(710.f, 740.f); bLoad.setFillColor(sf::Color(180, 210, 140));
    sf::Text tLoad("Load Config", font, 13); tLoad.setPosition(735.f, 745.f); tLoad.setFillColor(sf::Color::Black);
    window.draw(bLoad); window.draw(tLoad);
}