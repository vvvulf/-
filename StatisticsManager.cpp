#include "StatisticsManager.h"
#include <algorithm>
#include <string>

void StatisticsManager::update(int prey, int pred) {
    preyHistory.push_back(prey);
    predHistory.push_back(pred);
    if (preyHistory.size() > 200) {
        preyHistory.erase(preyHistory.begin());
        predHistory.erase(predHistory.begin());
    }
}

void StatisticsManager::reset() {
    preyHistory.clear();
    predHistory.clear();
}

void StatisticsManager::draw(sf::RenderWindow& window, float x, float y, float w, float h, const std::vector<std::unique_ptr<Agent>>& agents) {
    if (!font.getInfo().family.size()) {
        font.loadFromFile("arial.ttf");
    }

    sf::RectangleShape lineFrame({ w, h });
    lineFrame.setPosition(x, y);
    lineFrame.setFillColor(sf::Color(240, 240, 240));
    lineFrame.setOutlineThickness(2.f);
    lineFrame.setOutlineColor(sf::Color(70, 70, 70));
    window.draw(lineFrame);

    sf::Text title("Population Dynamics (Line Chart)", font, 14);
    title.setFillColor(sf::Color::Black);
    title.setPosition(x + 10, y + 5);
    window.draw(title);

    if (preyHistory.size() > 1) {
        int maxVal = 10;
        for (int v : preyHistory) if (v > maxVal) maxVal = v;
        for (int v : predHistory) if (v > maxVal) maxVal = v;

        sf::VertexArray preyLines(sf::LineStrip, preyHistory.size());
        sf::VertexArray predLines(sf::LineStrip, predHistory.size());

        float stepX = w / (float)(preyHistory.size() - 1);

        for (size_t i = 0; i < preyHistory.size(); ++i) {
            float px = x + i * stepX;
            float pyPrey = y + h - (preyHistory[i] / (float)maxVal) * (h - 30.f) - 10.f;
            float pyPred = y + h - (predHistory[i] / (float)maxVal) * (h - 30.f) - 10.f;

            preyLines[i].position = { px, pyPrey };
            preyLines[i].color = sf::Color::Blue;

            predLines[i].position = { px, pyPred };
            predLines[i].color = sf::Color::Red;
        }
        window.draw(preyLines);
        window.draw(predLines);
    }

    int rabbits = 0, sheep = 0, wolves = 0, foxes = 0;
    for (const auto& a : agents) {
        if (!a || !a->alive) continue;
        if (a->kind() == AgentKind::Prey) {
            if (a->subtype() == 0) rabbits++;
            else if (a->subtype() == 1) sheep++;
        }
        else {
            if (a->subtype() == 1) wolves++;
            else if (a->subtype() == 2) foxes++;
        }
    }

    float hY = y + h + 20;
    float hH = 140.f;
    sf::RectangleShape histoFrame({ w, hH });
    histoFrame.setPosition(x, hY);
    histoFrame.setFillColor(sf::Color(240, 240, 240));
    histoFrame.setOutlineThickness(2.f);
    histoFrame.setOutlineColor(sf::Color(70, 70, 70));
    window.draw(histoFrame);

    sf::Text histoTitle("Subtype Distribution (Histogram)", font, 14);
    histoTitle.setFillColor(sf::Color::Black);
    histoTitle.setPosition(x + 10, hY + 5);
    window.draw(histoTitle);

    int counts[4] = { rabbits, sheep, wolves, foxes };
    std::string labels[4] = { "Rabbit", "Sheep", "Wolf", "Fox" };
    sf::Color colors[4] = { sf::Color(100, 150, 255), sf::Color(30, 80, 255), sf::Color(255, 100, 100), sf::Color(200, 20, 20) };

    int maxHisto = 1;
    for (int c : counts) if (c > maxHisto) maxHisto = c;

    float barWidth = (w - 40.f) / 4.f;
    for (int i = 0; i < 4; ++i) {
        float barHeight = (counts[i] / (float)maxHisto) * 80.f;
        sf::RectangleShape bar({ barWidth - 15.f, barHeight });
        bar.setPosition(x + 20.f + i * barWidth, hY + hH - barHeight - 30.f);
        bar.setFillColor(colors[i]);
        window.draw(bar);

        sf::Text txt(labels[i] + "\n  " + std::to_string(counts[i]), font, 12);
        txt.setFillColor(sf::Color::Black);
        txt.setPosition(x + 18.f + i * barWidth, hY + hH - 26.f);
        window.draw(txt);
    }
}