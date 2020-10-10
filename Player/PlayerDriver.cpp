//
//  PlayerDriver.cpp
//  Player
//
//  Created by Lina Kretzschmar on 2020-09-27.
//



#include <stdio.h>
#include <iostream>
#include "Player.h"

using namespace std;

int Player::playerCount = 0;


int main(){
    
    
    //Creating two players
    Player player1("Berta");
    Player player2;

    //Creating a map
    Map testMap = new Map();
    
    //Creating two continents
    shared_ptr<Continent> c1 = testMap.add(Continent(1, "America", 1));
    shared_ptr<Continent> c2 = testMap.add(Continent(2, "Europe", 5));
    
    //Creating new territories of type ptr and adding them to the testMap
    shared_ptr<Territory> t1 = testMap.add(Territory(1, "Canada", 1));
    shared_ptr<Territory> t2 = testMap.add(Territory(2, "Iceland", 2));
    shared_ptr<Territory> t3 = testMap.add(Territory(3, "United Kingdom", 2));
    shared_ptr<Territory> t4 = testMap.add(Territory(4, "France", 2));
    shared_ptr<Territory> t5 = testMap.add(Territory(5, "Spain", 2));
    shared_ptr<Territory> t6 = testMap.add(Territory(6, "Italy", 2));
    
    //Connecting territories via directed graphs
    testMap.link(t1, t2);
    testMap.link(t2, t3);
    testMap.link(t3, t4);
    testMap.link(t4, t5);
    testMap.link(t4, t1);
    testMap.link(t5, t6);
    testMap.link(t6, t3);
    testMap.link(t6, t4);
    
    //Printing the map
    cout << "Testing map: \n" <<  testMap.to_string() << "\n";
    
    //Adding two territories to player1
    player1.addTerritory(t3);
    player1.addTerritory(t4);

    //Displaying territoryList containing territories currently owned by player2
    cout << "Player " << player1.getName() << " owns the following territories:\n" + player1.printList(player1.getTerritoryList());
//    player1.printList(player1.getTerritoryList());


    //Test issueOrder() creates an order objects and adds it to the OrderList
    cout << "\nIssueing Orders:\n";
    
    player1.issueOrder("Deploy");
    player1.issueOrder("Advance");
    player1.issueOrder("Bomb");
    player1.issueOrder("Blockade");
    player1.issueOrder("Airlift");
    player1.issueOrder("Negotiate");
    
    cout << "\nOrderList from player " << player1.getName() << ":\n" << *(player1.getOrderList()) << endl;
    
    
    //test toAttack()
    cout << "Territories to Attack:\n" + player1.printList(player1.toAttack(testMap));
    
    
    //test toDefend()
    cout << "\nTerritories to Defend:\n" + player1.printList(player1.toDefend(testMap));
    
    
    //Creating, initializing a deck of cards
    cout << "Testing Hand of Cards: \n";
    Deck testDeck;
    testDeck.initialize_vec_deck();
    cout << testDeck;
    

    //drawing the card returns a card pointer for now
    cout << "Testing hand of cards:\n";
    player1.handOfCards->set_vec_hand(testDeck.draw());
    cout << *(player1.handOfCards);
    
    //Testing players to_string method
    //    cout << "\nTesting toString() method:" << player1.to_string() << endl;

    
    return 0;
};
