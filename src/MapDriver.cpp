#include <iostream>
#include <memory>
#include "Map.h"

int main() {
    Map hello = new Map();

    // Add a territory : with its id, its name and the continent's id to which it belongs
    // Doing so, you can keep a reference to via the object returned from the add()
    std::shared_ptr<Territory> t1 = hello.add(Territory(1, "Canada", 1));
    std::shared_ptr<Territory> t2 = hello.add(Territory(2, "U.S.", 1));

    // Add a Continent : id and name only
    // Same for keeping a reference
    std::shared_ptr<Continent> c1 = hello.add(Continent(1, "America", 2));
    std::shared_ptr<Continent> c2 = hello.add(Continent(2, "Europe", 4)); // Having a gap in Continent id's is not allowed.

    // You can add territories and Continents in any order, but you have to make sure that all territories map to an existing continent
    hello.add(Territory(3, "Mexico", 1));
    hello.add(Territory(4, "France", 2));
    hello.add(Territory(5, "Spain", 2));

    // Create a path from t1 to t2
    // this is a directed edge
    // Same for two continents
    hello.link(t1, t2);
    hello.link(t2, hello.findTerritory("Mexico"));
    hello.link(hello.getTerritory(4), hello.getTerritory(5));


    // Copy constructor with full deep copy
    Map hello2 = new Map(hello);

    hello.link(t2, hello.findTerritory("France"));

    for (int b : t1->borders) {
        std::cout << b << std::endl;
    }

    for (int b : t2->borders) {
        std::cout << b << std::endl;
    }

    // You can get the shared ptr to a territory or continent with
    // the methods findTerritory, findContinent, getTerritory, getContinent
    // the find ones look through all territories/continent and return
    // the first one with a corresponding name (linear)
    // the get ones return based on the id provided (constant time)
    std::shared_ptr<Territory> p = hello.findTerritory("Mexico");
    std::shared_ptr<Territory> p2 = hello.getTerritory(p->id);
    std::shared_ptr<Territory> p3 = std::shared_ptr<Territory>(p);
    p->units = 1;

    // Map has a to_string method
    // Same for Territory's and Continent's
    std::cout << hello.to_string() << std::endl;
    std::cout << hello << std::endl;
    std::cout << hello2 << std::endl;

    std::cout << p->to_string() << std::endl;
    std::cout << p2->to_string() << std::endl;
    std::cout << p3->to_string() << std::endl;

    // Release a shared ptr
    // Only releases the one you call it on
    p.reset();

    std::cout << p << std::endl;
    std::cout << p2 << std::endl;

    p = hello.findTerritory("Mexico");

    std::cout << p << std::endl;
    std::cout << p2 << std::endl;

    // Validate will fail and throw if:
    // some territories belong to a non-existing continent
    // some continents dont have any territories
    if (hello.validate()) {
        // start game
        std::cout << "hello is valid? " << hello.validate() << std::endl;
    }

    if (hello2.validate()) {
        std::cout << "hello2 (deep copy of hello without link from U.S. to France)?" << std::endl;
    }

    p.reset();
    p2.reset();
    p3.reset();
}