#include "PopulationStorageManager.h"

PopulationStorageManager::PopulationStorageManager(
    GameField& field,
    std::vector<std::unique_ptr<Agent>>& agents_
)
    : gf(field), agents(agents_) {}

bool PopulationStorageManager::saveSimulation(const std::string& file) const
{
    std::ofstream out(file);
    if (!out.is_open()) return false;

    out << agents.size() << "\n";

    for (auto& a : agents)
    {
        if (!a) continue;

        out << (int)a->kind() << " "
            << a->subtype() << " "
            << a->id << " "
            << a->x << " "
            << a->y << " "
            << a->energy << " "
            << a->alive << "\n";
    }

    return true;
}

bool PopulationStorageManager::loadSimulation(const std::string& file)
{
    std::ifstream in(file);
    if (!in.is_open()) return false;

    agents.clear();

    // reset field (ÂÀÆËÈÂÎ)
    gf = GameField(gf.size());

    int count;
    in >> count;

    for (int i = 0; i < count; i++)
    {
        int kind, subtype, id, x, y;
        float energy;
        bool alive;

        in >> kind >> subtype >> id >> x >> y >> energy >> alive;

        std::unique_ptr<Agent> a;

        if ((AgentKind)kind == AgentKind::Prey)
        {
            a = std::make_unique<Prey>(
                id,
                (PreySubtype)subtype,
                x,
                y
            );
        }
        else
        {
            a = std::make_unique<Predator>(
                id,
                (PredatorSubtype)subtype,
                x,
                y
            );
        }

        a->energy = energy;
        a->alive = alive;
        a->x = x;
        a->y = y;

        // ÊËŞ×ÎÂÅ ÂÈÏĞÀÂËÅÍÍß:
        gf.placeAgent(x, y, id);

        agents.push_back(std::move(a));
    }

    return true;
}

bool PopulationStorageManager::exportCSV(const std::string& file) const
{
    std::ofstream out(file);
    if (!out.is_open()) return false;

    out << "type,x,y,energy,alive\n";

    for (auto& a : agents)
    {
        if (!a) continue;

        out << (int)a->kind() << ","
            << a->x << ","
            << a->y << ","
            << a->energy << ","
            << a->alive << "\n";
    }

    return true;
}