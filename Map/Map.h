#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <memory>
#include <ostream>
#include <iostream>

using namespace std;

struct Land
{
    Land(unsigned int id, string name);
    Land(const Land& land);
    ~Land();

    string name; // Both Territory and Continent need a name

public:
    unsigned int getID();

private:
    unsigned int id; // Both Territory and Continent need an id
};

struct Territory : Land
{
    Territory(unsigned int id, string name, unsigned int continentID);
    Territory(const Territory& territory);
    ~Territory();

    unsigned int units;
    unsigned int ownerID;
    unsigned int continentID;

    vector<unsigned int> borders;

    string to_string();
    Territory& operator=(const Territory& territory);
    friend ostream& operator<<(ostream& strm, Territory &territory);
};

struct Continent : Land
{
    Continent(int id, string name, unsigned int bonus);
    Continent(const Continent& continent);
    ~Continent();

    unsigned int bonus;
    vector<unsigned int> territoryIDs;

    string to_string();
    Continent& operator=(const Continent& continent);
    friend ostream& operator<<(ostream& strm, Continent &continent);
};

class Map
{
private:
    vector<shared_ptr<Territory>> territories; // these are vectors because they are accessed using ID's
    vector<shared_ptr<Continent>> continents; // ID's =:= index

public:
    int getTerritoriesCount();
    int getContinentsCount();

    shared_ptr<Territory> add(Territory territory);
    shared_ptr<Continent> add(Continent continent);

    bool link(shared_ptr<Territory> a, shared_ptr<Territory> b);

    shared_ptr<Territory> findTerritory(string name);
    shared_ptr<Territory> getTerritory(unsigned int id);

    shared_ptr<Continent> findContinent(string name);
    shared_ptr<Continent> getContinent(unsigned int id);

    bool validate();

    string to_string ();
    Map& operator=(const Map& map);
    friend ostream& operator<<(ostream& strm, Map &map);

    Map();
    Map(int numOfTerritories, int numOfContinents);
    Map(const Map& map);
    ~Map();

};