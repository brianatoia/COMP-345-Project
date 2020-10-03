#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <ostream>
// DEBUG
 #include <iostream>

struct Land
{
    Land(unsigned int id, std::string name);

    unsigned int id;
    std::string name;

    std::vector<unsigned int> borders;
};

struct Territory : Land {
    Territory(unsigned int id, std::string name, unsigned int continentID);

    unsigned int units;
    unsigned int ownerID;
    unsigned int continentID;

    std::string to_string ();
};

struct Continent : Land
{
    Continent(int id, std::string name, unsigned bonus);

    unsigned int bonus;
    std::vector<unsigned int> territoryIDs;

    std::string to_string ();
};

class Map
{
  private:
    std::vector<std::shared_ptr<Territory>> territories;
    std::vector<std::shared_ptr<Continent>> continents;

  public:
    std::shared_ptr<Territory> add (Territory territory);
    std::shared_ptr<Continent> add (Continent continent);

    // TODO: make this a template function instead
    bool link(std::shared_ptr<Territory> a, std::shared_ptr<Territory> b);
    bool link(std::shared_ptr<Continent> a, std::shared_ptr<Continent> b);

    std::shared_ptr<Territory> findTerritory(std::string name);
    std::shared_ptr<Territory> getTerritory(unsigned int id);

    std::shared_ptr<Continent> findContinent(std::string name);
    std::shared_ptr<Continent> getContinent(unsigned int id);

    // TODO: make this verify integrity of map
    bool validate();

    std::string to_string ();
    void operator = (Map map);
    friend std::ostream& operator<<(std::ostream& strm, Map &map);

    Map();
    Map(int numOfTerritories, int numOfContinents);
    Map(Map* map);
    ~Map();

};