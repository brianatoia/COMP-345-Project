#pragma once
#include <iostream>
#include <memory>
#include "Map.h"

<<<<<<< HEAD
using namespace std;

int main()
{
=======

int main() {
>>>>>>> master
    Map hello = Map();

    // Add a territory : with its id, its name and the continent's id to which it belongs
    // Doing so, you can keep a reference to via the object returned from the add()
    shared_ptr<Territory> t1 = hello.add(Territory(1, "Canada", 1));
    shared_ptr<Territory> t2 = hello.add(Territory(2, "U.S.", 1));

    // Add a Continent : id and name only
    // Same for keeping a reference
    shared_ptr<Continent> c1 = hello.add(Continent(1, "America", 2));
    shared_ptr<Continent> c2 = hello.add(Continent(2, "Europe", 4)); // Having a gap in Continent id's is not allowed.

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
    Map hello2 = Map(hello);
    Map* hello3 = new Map(hello2);
    cout << hello3 << endl;
    Map* hello4 = new Map(*hello3);
    cout << hello4 << endl;

    hello.link(t2, hello.findTerritory("France"));

    for (int b : t1->borders)
    {
        cout << b << endl;
    }

    for (int b : t2->borders)
    {
        cout << b << endl;
    }

    // You can get the shared ptr to a territory or continent with
    // the methods findTerritory, findContinent, getTerritory, getContinent
    // the find ones look through all territories/continent and return
    // the first one with a corresponding name (linear)
    // the get ones return based on the id provided (constant time)
    shared_ptr<Territory> p = hello.findTerritory("Mexico");
    shared_ptr<Territory> p2 = hello.getTerritory(p->getID());
    shared_ptr<Territory> p3 = shared_ptr<Territory>(p);
    p->units = 1;

    // Map has a to_string method
    // Same for Territory's and Continent's
    cout << hello.to_string() << endl;
    cout << hello << endl;
    cout << hello2 << endl;

    cout << p->to_string() << endl;
    cout << p2->to_string() << endl;
    cout << p3->to_string() << endl;

    // Release a shared ptr
    // Only releases the one you call it on
    p.reset();

    cout << p << endl;
    cout << p2 << endl;

    p = hello.findTerritory("Mexico");

    cout << p << endl;
    cout << p2 << endl;

    // Validate will fail and throw if:
    // some territories belong to a non-existing continent
    // some continents dont have any territories
    if (hello.validate())
    {
        // start game
        cout << "hello is valid? " << ((hello.validate() == true) ? "true" : "false") << endl;
    }

    cout << "What about hello2?" << endl;
    hello2.validate(); // will print error;

    hello3->add(Continent(3, "Asia", 7));
    hello3->add(Territory(6, "Japan", 3));
    cout << "And hello3?" << endl;
    hello3->validate();

    p.reset();
    p2.reset();
    p3.reset();

    delete hello3;
    delete hello4;
}
