#include "Field.h"
#include "Agent.h"

GameField::GameField(int size, unsigned seed)
    : n(size),
    field(size, std::vector<Cell>(size)),
    grid(size, std::vector<int>(size, -1)),
    rng(seed)
{}

bool GameField::inBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < n && y < n;
}

bool GameField::isFreeAt(int x, int y) const {
    return inBounds(x, y) && grid[y][x] == -1;
}

void GameField::setAgentAt(int x, int y, int agentId) {
    if (!inBounds(x, y)) return;
    grid[y][x] = agentId;
}

int GameField::agentAt(int x, int y) const {
    if (!inBounds(x, y)) return -1;
    return grid[y][x];
}

void GameField::placeAgent(int x, int y) {
    if (!inBounds(x, y)) return;
    grid[y][x] = 1; // зайнято
}

void GameField::removeAgentFrom(int x, int y) {
    if (!inBounds(x, y)) return;
    grid[y][x] = -1;
}

void GameField::draw(
    sf::RenderWindow& w,
    const std::vector<std::unique_ptr<Agent>>& agents,
    float ox,
    float oy
) const
{
    float ts = 10.f;

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {

            sf::RectangleShape r({ ts, ts });
            r.setPosition(ox + x * ts, oy + y * ts);
            r.setFillColor(field[y][x].color());
            w.draw(r);

            int id = grid[y][x];

            if (id >= 0 && id < (int)agents.size()) {
                const auto& a = agents[id];

                if (!a || !a->alive) continue;

                sf::CircleShape c(ts / 3);
                c.setPosition(ox + x * ts, oy + y * ts);

                if (a->kind() == AgentKind::Prey)
                    c.setFillColor(sf::Color::Blue);
                else
                    c.setFillColor(sf::Color::Red);

                w.draw(c);
            }
        }
    }
}

void GameField::randomizeClusters(int count, int size, int buffer) {

    for (auto& row : field)
        for (auto& c : row)
            c.food = 0.f;

    std::uniform_int_distribution<int> dist(0, n - 1);

    for (int i = 0; i < count; i++) {

        int cx = dist(rng);
        int cy = dist(rng);

        for (int dy = -size; dy <= size; dy++) {
            for (int dx = -size; dx <= size; dx++) {

                int x = cx + dx;
                int y = cy + dy;

                if (inBounds(x, y)) {
                    field[y][x].food = 1.f;
                }
            }
        }
    }
}