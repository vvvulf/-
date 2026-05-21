#include "StatisticsManager.h"

void StatisticsManager::update(int p, int d) {

    prey.push_back(p);
    pred.push_back(d);

    if (prey.size() > maxPoints) {
        prey.erase(prey.begin());
        pred.erase(pred.begin());
    }
}

void StatisticsManager::reset() {
    prey.clear();
    pred.clear();
}

void StatisticsManager::draw(sf::RenderWindow& w,
    float x, float y,
    float wdt, float h)
{
    if (prey.size() < 2) return;

    sf::VertexArray linePrey(sf::LineStrip);
    sf::VertexArray linePred(sf::LineStrip);

    int maxVal = 1;

    for (size_t i = 0; i < prey.size(); i++) {
        maxVal = std::max({ maxVal, prey[i], pred[i] });
    }

    for (size_t i = 0; i < prey.size(); i++) {

        float t = (float)i / (float)prey.size();
        float px = x + t * wdt;

        float py = y + h - (prey[i] / (float)maxVal) * h;
        float pd = y + h - (pred[i] / (float)maxVal) * h;

        linePrey.append({ {px, py}, sf::Color::Blue });
        linePred.append({ {px, pd}, sf::Color::Red });
    }

    w.draw(linePrey);
    w.draw(linePred);
}