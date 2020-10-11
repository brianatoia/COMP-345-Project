#include "Map.h"

Map::Map() : Map(1024, 1024) {} // 1024 is big, just to make sure you don't run out of memory when you add a territory or continent

Map::Map(int numOfTerritories, int numOfContinents) {
    this->territories = std::vector<std::shared_ptr<Territory>>(numOfTerritories);
    this->continents = std::vector<std::shared_ptr<Continent>>(numOfContinents);
}

Map::Map(const Map& map) {
    this->territories = std::vector<std::shared_ptr<Territory>>(map.territories.size());
    this->continents = std::vector<std::shared_ptr<Continent>>(map.continents.size());

    // Creating deep copy of shared_ptr's
    for (std::shared_ptr<Territory> t : map.territories) {
        if (t == nullptr) continue;
        this->territories[t->getID()] = std::make_shared<Territory>(Territory(*t.get()));
    }

    // Creating deep copy of shared_ptr's
    for (std::shared_ptr<Continent> c : map.continents) {
        if (c == nullptr) continue;
        this->continents[c->getID()] = std::make_shared<Continent>(Continent(*c.get()));
    }
}

Map::~Map() {
    for (std::shared_ptr<Territory> t : this->territories) {
        t.reset();
    }

    for (std::shared_ptr<Continent> c : this->continents) {
        c.reset();
    }

    this->territories.clear();
    this->continents.clear();
}

std::string Map::to_string () {
    std::string s;

    for (std::shared_ptr<Continent> c : continents) {
        if (c == nullptr) continue;
        s += c->to_string();
        s += "\n";

        for (unsigned int index : c->territoryIDs) {
            s += this->getTerritory(index)->to_string();
            s += "\n";
        }
    }

    return s;
}

std::shared_ptr<Territory> Map::add(Territory territory) {
    // Error checking
    if (territory.getID() == 0) {
        throw std::logic_error("ID's should start at 1");
    }

    if (this->getTerritory(territory.getID()) != nullptr) {
        throw std::logic_error("duplicate ID: territory already exists");
    }

    if (this->findTerritory(territory.name) != nullptr) {
        throw std::logic_error("duplicate name: territory already exists");
    }

    // Putting the territory in the map
    this->territories[territory.getID()] = std::shared_ptr<Territory> (new Territory(territory));

    // Matching the owning continent with this territory if it exists already
    std::shared_ptr<Continent> c = getContinent(territory.continentID);
    if (c != nullptr) {
        if (!std::count(c->territoryIDs.begin(), c->territoryIDs.end(), territory.getID())) { // This prevents from adding the same territoryID twice
            c->territoryIDs.push_back(territory.getID());
        }
    }

    return this->territories[territory.getID()];
}

std::shared_ptr<Continent> Map::add(Continent continent) {
    // Error checking
    if (continent.getID() == 0) {
        throw std::logic_error("ID's should start at 1");
    }

    if (this->getContinent(continent.getID()) != nullptr) {
        throw std::logic_error("duplicate ID: continent already exists");
    }

    if (this->findContinent(continent.name) != nullptr) {
        throw std::logic_error("duplicate name: continent already exists");
    }

    // Putting the continent in the map
    this->continents[continent.getID()] = std::shared_ptr<Continent> (new Continent(continent));

    // Matching its territories to itself
    for (std::shared_ptr<Territory> t : this->territories) {
        if (t == nullptr) continue;
        if (t->continentID == continent.getID()) {
            if (!std::count(this->continents[continent.getID()]->territoryIDs.begin(), this->continents[continent.getID()]->territoryIDs.end(), t->getID())) {
                this->continents[continent.getID()]->territoryIDs.push_back(t->getID());
            }
        }
    }

    return this->continents[continent.getID()];
}

bool Map::validate() {
    // resize territories and continents vectors
    bool valid = true;

    // resize territories to check for an id that points to an empty territory
    for (int t = this->territories.size() - 1; t >= 0; t--) {
        if (this->territories[t] != nullptr) {
            this->territories.resize(t+1);
            this->territories.shrink_to_fit();
            break;
        }
    }

    // same but for continents
    for (int c = this->continents.size() - 1; c >= 0; c--) {
        if (this->continents[c] != nullptr) {
            this->continents.resize(c+1);
            this->continents.shrink_to_fit();
            break;
        }
    }

    std::list<unsigned int> unconnectedTerritories = std::list<unsigned int>(); // List of territories who have no paths to or from
    std::list<std::list<unsigned int>> connectedLists = std::list<std::list<unsigned int>>(); // List of subgraphs

    // multiple tests for
    for (int i = 1; i < this->territories.size(); i++) {
        std::shared_ptr<Territory> t = this->territories[i];

        // if t is nullptr, then it means that there is an empty territory in the map, which makes it invalid
        // Reason: id's have to be consecutive
        if (t == nullptr) {
            std::cout << "there is an empty territory object at index " << i << std::endl;
            valid = false;
        }

        // if t->continentID is nullptr, it means that it doesn't have a continent, which makes it invalid
        // Reason: territories must belong to a continent
        if (getContinent(t->continentID) == nullptr) {
            std::cout << t->to_string() << " doesn't have a valid continent. Either the continent is missing or the territory has a bad continentID." << std::endl;
        }

        // Computing the connections for testing
        if (t->borders.size() > 0) {
            std::list<unsigned int> connectedTerritories = std::list<unsigned int>();
            connectedTerritories.push_back(t->getID());

            // If a territory is pointed to, remove it from unconnected territories.
            for (unsigned int b : t->borders) {
                connectedTerritories.push_back(b);
                unconnectedTerritories.remove(b);
            }

            // Go through connectedLists and merge them if the current connectedTerritories list
            // contains elements present in different connectedLists lists
            std::list<std::list<unsigned int>> listsToMerge = std::list<std::list<unsigned int>>();
            for (unsigned int b : connectedTerritories) {
                if (connectedLists.size() == 0) break;
                for (std::list<unsigned int> listToCheck : connectedLists) {
                    for (unsigned int c : listToCheck) {
                        if (b == c) {
                            listsToMerge.push_back(listToCheck);
                            break;
                        }
                    }
                }
            }

            // Merging lists later because other wise foreach breaks
            for (std::list<unsigned int> listToMerge : listsToMerge) {
                connectedLists.remove(listToMerge);
                listToMerge.sort();
                connectedTerritories.sort();
                connectedTerritories.merge(listToMerge);
                connectedTerritories.unique();
            }

            // Push the current connectedTerritories list to connectedLists
            // It's very well possible that right before this step, connectedLists is empty
            connectedLists.push_back(connectedTerritories);
        } else {
            // our element is pointed to by one of the connectedLists list, then it is connected, otherwise it isn't
            // if it is, we can ignore it and keep checking the next territory
            // if it isn't, we put the territory in the unconnectedTerritories list
            bool isConnected = false;
            for (std::list<unsigned int> listToCheck : connectedLists) {
                for (unsigned int c : listToCheck) {
                    if (t->getID() == c) {
                        isConnected = true;
                        break;
                    }
                }
                if (isConnected) break;
            }

            if (!isConnected) {
                unconnectedTerritories.push_back(t->getID());
            }
        }
    }

    // Tests for continents
    for (int i = 1; i < this->continents.size(); i++) {
        std::shared_ptr<Continent> c = this->continents[i];
        if (c == nullptr) {
            std::cout << "there is an empty continent object at index " << i << std::endl;
            valid = false;
        }

        if (c->territoryIDs.size() == 0) {
            std::cout << c->name << " has no territories" << std::endl;
            valid = false;
        }
    }

    // If any territory is in the unconnectedTerritories, it means it's a lonely island, which makes the map invalid
    // Reason: Maps cannot accept lonely islands.
    if (unconnectedTerritories.size() > 0) {
        std::cout << "Map contains " << unconnectedTerritories.size() << " lonely island(s) at " << std::endl;

        for (unsigned int li : unconnectedTerritories) {
            std::cout << li << ", ";
        }
        std::cout << std::endl;

        valid = false;
    }

    // If the connectedLists list has more than one list, it means that the map is not fully connected, which makes it invalid
    // Reason: the map must be a connected graph
    if (connectedLists.size() > 1) {
        std::cout << "Map contains " << connectedLists.size() << " unconnected islands:" << std::endl;

        for (std::list<unsigned int> island : connectedLists) {
            std::cout << "[";
            for (unsigned int i : island) {
                std::cout << i << " ";
            }
            std::cout << "]" << std::endl;
        }

        valid = false;
    } // This also guarantees that the continents are connected subgraphs

    if (valid) {
        std::cout << "Map is valid!" << std::endl;
    } else {
        std::cout << "Map is not valid!" << std::endl;
    }

    return valid;
}

// Creating a path from territory a to territory b, this is a directed path
bool Map::link(std::shared_ptr<Territory> a, std::shared_ptr<Territory> b) {
    bool a2b = false;

    for (int i : a->borders) {
        if (i == b->getID()) {
            a2b = true;
            continue;
        }
    }

    if (!a2b) {
        a->borders.push_back(b->getID());
    }

    return a2b;
}

// Querying a territory by name, linear
std::shared_ptr<Territory> Map::findTerritory(std::string name) {
    for (std::shared_ptr<Territory> t : this->territories) {
        if (t == nullptr) continue;
        if (t->name.compare(name) == 0) {
            return std::shared_ptr<Territory>(t);
        }
    }

    return nullptr;
}

// Getting a territory by id, constant
std::shared_ptr<Territory> Map::getTerritory(unsigned int id) {
    if (id >= this->territories.size()) {
        throw std::out_of_range ("id is out of bounds");
    }

    return std::shared_ptr<Territory>(this->territories[id]);
}

// Querying a continent by name, linear
std::shared_ptr<Continent> Map::findContinent(std::string name) {

    for (std::shared_ptr<Continent> c : this->continents) {
        if (c == nullptr) continue;
        if (c->name.compare(name) == 0) {
            return std::shared_ptr<Continent>(c);
        }
    }

    return nullptr;
}

// Getting a continent by id, constant
std::shared_ptr<Continent> Map::getContinent(unsigned int id) {
    if (id >= this->continents.size()) {
        throw std::out_of_range ("id is out of bounds");
    }

    return std::shared_ptr<Continent>(this->continents[id]);
}

// Assignment operator for map, copies over territories and continents
Map& Map::operator=(const Map& map) {
    this->territories.clear();
    this->continents.clear();

    this->territories = map.territories;
    this->continents = map.continents;

    return *this;
}

Territory::Territory(unsigned int id, std::string name, unsigned int continentID) : Land(id, name) {
    this->units = 0;
    this->continentID = continentID;
    this->borders = std::vector<unsigned int>();
    this->ownerID = 0;
}

Territory::Territory(const Territory &territory) : Land(territory) {
    this->units = territory.units;
    this->continentID = territory.continentID;
    this->borders = std::vector<unsigned int>();
    this->ownerID = territory.ownerID;

    for (int i = 0; i < territory.borders.size(); i++) {
        this->borders.push_back(territory.borders[i]);
    }
}

Territory::~Territory() {
    this->borders.clear();
}

std::string Territory::to_string() {
    char a[1024]; // big byte number just to be safe

#if __APPLE__
    sprintf(a, "Territory %d %s: %i unit(s), %li border(s)", this->getID(), this->name.c_str(), this->units, this->borders.size());
#else
    sprintf_s(a, "Territory %d %s: %i unit(s), %li border(s)", this->getID(), this->name.c_str(), this->units, this->borders.size());
#endif

    return std::string(a);
}

// Territory assignment operator throws an error because doing this could break the id system
Territory& Territory::operator=(const Territory& territory) {
    throw std::logic_error("You cannot do an assignment operator with another territory as this could break the id system");

    return *this;
}

Continent::Continent(int id, std::string name, unsigned int bonus) : Land(id, name) {
    this->bonus = bonus;
    this->territoryIDs = std::vector<unsigned int>();
}

Continent::Continent(const Continent &continent) : Land(continent) {
    this->bonus = continent.bonus;
    this->territoryIDs = std::vector<unsigned int>();

    for (int i = 0; i < continent.territoryIDs.size(); i++) {
        this->territoryIDs.push_back(continent.territoryIDs[i]);
    }
}

Continent::~Continent() {
    this->territoryIDs.clear();
}

std::string Continent::to_string() {
    char a[1024]; // big number for safety

#if __APPLE__
    sprintf(a, "Continent %d %s: %d bonus points, %li territories(s)", this->getID(), this->name.c_str(), this->bonus, this->territoryIDs.size());
#else
    sprintf_s(a, "Continent %d %s: %d bonus points, %li territories(s)", this->getID(), this->name.c_str(), this->bonus, this->territoryIDs.size());
#endif
    return std::string(a);
}

// Same as for territory
Continent& Continent::operator=(const Continent& continent) {
    throw std::logic_error("You cannot do an assignment operator with another continent as this could break the id system");

    return *this;
}

Land::Land(unsigned int id, std::string name) {
    this->id = id;
    this->name = name;
}

Land::Land(const Land &land) {
    this->id = land.id;
    this->name = std::string(land.name.c_str());
}

Land::~Land() {
    this->name.clear();
}

unsigned int Land::getID() {
    return this->id;
}

std::ostream& operator<<(std::ostream& strm, Map &map) {
    return strm << map.to_string();
}

std::ostream& operator<<(std::ostream& strm, Territory &territory) {
    return strm << territory.to_string();
}

std::ostream& operator<<(std::ostream& strm, Continent &continent) {
    return strm << continent.to_string();
}