#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <memory>
#include <ostream>
// DEBUG
#include <iostream>

 struct Land
{
    Land(unsigned int id, std::string name);
    Land(const Land& land);
    ~Land();

    std::string name;

public:
    unsigned int getID();

private:
    unsigned int id;
};

struct Territory : Land
{
    Territory(unsigned int id, std::string name, unsigned int continentID);
    Territory(const Territory& territory);
    ~Territory();

    unsigned int units;
    unsigned int ownerID;
    unsigned int continentID;

    std::vector<unsigned int> borders;

    std::string to_string();
    Territory& operator=(const Territory& territory);
    friend std::ostream& operator<<(std::ostream& strm, Territory &territory);
};

struct Continent : Land
{
    unsigned int bonus;
    std::vector<unsigned int> territoryIDs;

    std::string to_string ();
    Continent& operator=(const Continent& continent);
    friend std::ostream& operator<<(std::ostream& strm, Continent &continent);

    Continent(int id, std::string name, unsigned bonus);
    Continent(const Continent& continent);
    ~Continent();
};

class Map
{
  private:
    std::vector<std::shared_ptr<Territory>> territories;
    std::vector<std::shared_ptr<Continent>> continents;

  public:
    std::shared_ptr<Territory> add(Territory territory);
    std::shared_ptr<Continent> add(Continent continent);

    bool link(std::shared_ptr<Territory> a, std::shared_ptr<Territory> b);

    std::shared_ptr<Territory> findTerritory(std::string name);
    std::shared_ptr<Territory> getTerritory(unsigned int id);

    std::shared_ptr<Continent> findContinent(std::string name);
    std::shared_ptr<Continent> getContinent(unsigned int id);

    bool validate();

    std::string to_string ();
    Map& operator=(const Map& map);
    friend std::ostream& operator<<(std::ostream& strm, Map &map);

    Map();
    Map(int numOfTerritories, int numOfContinents);
    Map(const Map& map);
    ~Map();

};