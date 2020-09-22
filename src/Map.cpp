#include "Map.h"

Map::Map() {
    this->territories = std::vector<std::shared_ptr<Territory>>(10);
    this->continents = std::vector<std::shared_ptr<Continent>>(10);
}

Map::Map(Map *map) {
    this->territories = map->territories;
    this->continents = map->continents;
}

Map::~Map() {
    // TODO: use smart pointers (shared_pointer<>)
    this->territories.clear();
    this->continents.clear();
}

// TODO: rewrite to_string
// go through continents
// print territories in those continents
// print territories adjacent to territory
std::string Map::to_string () {
    std::string s;

    for (std::shared_ptr<Territory> t : territories) {
        if (t == nullptr) continue;
        s += t->to_string();
        s += "\n";
    }

    for (std::shared_ptr<Continent> c : continents) {
        if (c == nullptr) continue;
        s += c->to_string();
        s += "\n";
    }
    return s;
}

std::shared_ptr<Territory> Map::add(Territory territory) {
    if (this->getTerritory(territory.id) != nullptr) {
        throw std::logic_error("duplicate ID: territory already exists");
    }

    this->territories[territory.id] = std::shared_ptr<Territory> (new Territory(territory));

    std::shared_ptr<Continent> c = getContinent(territory.continentID);
    if (c != nullptr) {
        if (!std::count(c->territories.begin(), c->territories.end(), territory.id)) {
            c->territories.push_back(territory.id);
        }
    }

    return this->territories[territory.id];
}

std::shared_ptr<Continent> Map::add(Continent continent) {
    if (this->getContinent(continent.id) != nullptr) {
        throw std::logic_error("duplicate ID: continent already exists");
    }

    this->continents[continent.id] = std::shared_ptr<Continent> (new Continent(continent));

    for (std::shared_ptr<Territory> t : this->territories) {
        if (t == nullptr) continue;
        if (t->continentID == continent.id) {
            if (!std::count(this->continents[continent.id]->territories.begin(), this->continents[continent.id]->territories.end(), t->id)) {
                this->continents[continent.id]->territories.push_back(t->id);
            }
        }
    }

    return this->continents[continent.id];
}

// TODO: implement validate
bool Map::validate() {
    return true;
}

bool Map::link(std::shared_ptr<Territory> a, std::shared_ptr<Territory> b) {
    bool a2b = false, b2a = false;

    for (int i : a->borders) {
        if (i == b->id) {
            a2b = true;
            continue;
        }
    }

    if (!a2b) {
        a->borders.push_back(b->id);
    }

    for (int i : b->borders) {
        if (i == a->id) {
            b2a = true;
            continue;
        }
    }

    if (!b2a) {
        b->borders.push_back(a->id);
    }

    return a2b && b2a;
}

bool Map::link(std::shared_ptr<Continent> a, std::shared_ptr<Continent> b) {
    bool a2b = false, b2a = false;

    for (int i : a->borders) {
        if (i == b->id) {
            a2b = true;
            continue;
        }
    }

    if (!a2b) {
        a->borders.push_back(b->id);
    }

    for (int i : b->borders) {
        if (i == a->id) {
            b2a = true;
            continue;
        }
    }

    if (!b2a) {
        b->borders.push_back(a->id);
    }

    return a2b && b2a;
}

std::shared_ptr<Territory> Map::findTerritory(std::string name) {
    for (std::shared_ptr<Territory> t : this->territories) {
        if (t == nullptr) continue;
        if (t->name == name) {
            return std::shared_ptr<Territory>(t);
        }
    }

    return nullptr;
}

std::shared_ptr<Territory> Map::getTerritory(unsigned int id) {
    return std::shared_ptr<Territory>(this->territories[id]);
}

std::shared_ptr<Continent> Map::findContinent(std::string name) {
    for (std::shared_ptr<Continent> c : this->continents) {
        if (c == nullptr) continue;
        if (c->name == name) {
            return std::shared_ptr<Continent>(c);
        }
    }

    return nullptr;
}

std::shared_ptr<Continent> Map::getContinent(unsigned int id) {
    return std::shared_ptr<Continent>(this->continents[id]);
}

void Map::operator=(Map map) {
    this->territories.clear();
    this->continents.clear();

    this->territories = map.territories;
    this->continents = map.continents;
}

Territory::Territory(unsigned int id, const char *name, unsigned int continentID) : Land(id, name) {
    this->units = 0;
    this->continentID = continentID;
}

std::string Territory::to_string() {
    char a[1024];

    sprintf_s(a, "Territory %d %s: %i unit(s), %li border(s)", this->id, this->name, this->units, this->borders.size());

    return std::string(a);
}

Continent::Continent(int id, const char *name) : Land(id, name) {
    this->territories = std::vector<unsigned int>();
}

std::string Continent::to_string() {
    char a[1024];

    sprintf(a, "Continent %d %s: %li territories(s), %li border(s)", this->id, this->name, this->territories.size(), this->borders.size());

    return std::string(a);
}

Land::Land(unsigned int id, const char *name) {
    this->id = id;
    this->name = name;

    this->borders = std::vector<unsigned int>();
}


std::ostream& operator<<(std::ostream& strm, Map &map) {
    return strm << map.to_string();
}