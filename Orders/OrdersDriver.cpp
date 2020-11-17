#include "Orders.h"
#include "../Player/Player.h"

int main6()
{
    //Creating two players
    Player* player1(new Player("Berta"));    //Created player1 using the paramterized constructor
    Player* player2(new Player("Rob"));

    //Creating a map
    shared_ptr<Map> testMap(new Map());

    //Creating two continents
    shared_ptr<Continent> c1 = testMap->add(Continent(1, "America", 1));
    shared_ptr<Continent> c2 = testMap->add(Continent(2, "Europe", 5));

    //Creating new territories of type ptr and adding them to the testMap
    shared_ptr<Territory> t1 = testMap->add(Territory(1, "Canada", 1));
    shared_ptr<Territory> t2 = testMap->add(Territory(2, "Iceland", 2));
    shared_ptr<Territory> t3 = testMap->add(Territory(3, "United Kingdom", 2));
    shared_ptr<Territory> t4 = testMap->add(Territory(4, "France", 2));
    shared_ptr<Territory> t5 = testMap->add(Territory(5, "Spain", 2));
    shared_ptr<Territory> t6 = testMap->add(Territory(6, "Italy", 2));

    //Connecting territories via directed graphs
    testMap->link(t1, t2);
    testMap->link(t2, t3);
    testMap->link(t3, t4);
    testMap->link(t4, t5);
    testMap->link(t4, t1);
    testMap->link(t5, t6);
    testMap->link(t6, t3);
    testMap->link(t6, t4);

    //Adding two territories to player1
    player1->addTerritory(t3);
    player1->addTerritory(t4);
    player2->addTerritory(t5);

    cout << "PLAYER1: " << endl;
    cout << *player1 << endl;
    cout << "\nPLAYER2: " << endl;
    cout << *player2 << endl;

    //Test issueOrder() creates an order objects and adds it to the OrderList
    cout << "\nTest issueing orders:\n";
    cout << t4->units << endl;
    player1->issueOrder("Deploy", testMap);
    cout << t4->units << endl;
        
    t5->units = 20;

    //player1->issueOrder("Negotiate");

    /*cout << "Spain: " << t5->units;
    player1->issueOrder("Bomb", testMap);
    cout << "Spain2: " << t5->units << endl;*/

    player1->issueOrder("Blockade", testMap);
    cout << t4->units << endl;
    for (shared_ptr<Territory> t : player1->getTerritoryList())
    {
        cout << t->name << endl;
    }

    /*cout << t5->units << endl;
    player1->issueOrder("Advance", testMap);
    cout << t5->units << endl;
    cout << t4->units << endl;*/

    
    //player1->issueOrder("Airlift", testMap);
    

    //player1->issueOrder("Negotiate");
   
    /*player1->issueOrder("Advance", testMap);
    cout << t5->units << endl;
    cout << t4->units << endl;

    cout << "PLAYER1: " << endl;
    cout << *player1 << endl;
    cout << "\nPLAYER2: " << endl;
    cout << *player2 << endl;*/

	return 0;
}