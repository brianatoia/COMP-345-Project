//
//  Player.h
//  Player
//
//  Created by Lina Kretzschmar on 2020-09-27.
//

#pragma once

#include "Map.h"
#include "Orders.h"
#include "Cards.h"

#include <stdio.h>
#include <iostream>
#include <string>


using namespace std;

 /* Header_h */

class Player
{
    
private:
    static int playerCount;
    unsigned int playerID;
    string name;
    
    OrderList * orderList;  //Orderlist holding pointers to orders
    list<shared_ptr<Territory>> territoryList;  //Territory list holding pointers to territories
    
public:
    Hand * handOfCards;
    
    Player();
    ~Player();
    Player(string playerName);
    Player(const Player&);
    Player& operator=(const Player&);
    string to_string();
    friend ostream& operator <<(ostream&, Player&);
    
    void setName (string playerName);
    string getName();
    static int getPlayerCount();
    unsigned int getPlayerID();
    
    OrderList * getOrderList();
    void issueOrder(string orderType);
    
    
    list<shared_ptr<Territory>> getTerritoryList();
    void addTerritory(shared_ptr<Territory> newTerritoryPtr);
    string printList(list<shared_ptr<Territory>> aList);
    list<shared_ptr<Territory>> toDefend(Map aMap);
    list<shared_ptr<Territory>> toAttack(Map aMap);
    
};

//*************      INSTRUCTIONS    ****************

// Player owns a collection of territories

// Players own a hand of Warzone cards

// Player contains methods toDefend() and toAttack() that return a list of territories that are to be defended and to be attaked, respectively

// Players contains an issueOrder() method that creates an order object and adds it to the list of orders

// Driver creates players and demonstrates that the above features are available


