//
//  Player.h
//  Player header, provides the declarations of functions contained in the Player.cpp file.
//
//  Created by Lina Kretzschmar on 2020-09-27.
//

#pragma once

#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <tuple>



using namespace std;

static list<tuple<int, int>> playersNegotiated;

class Player
{

private:
    static int playerCount; //Static counter, counting players of the game
    unsigned int playerID;
    string name;
    bool* capturedTerritory;
    unsigned int armies;    //number of armies player has to place

    list<shared_ptr<Territory>> territoryList;  //Territory list holding pointers to territories
    Hand* hand; //A pointer to a vector list holding ptrs to warzone cards
    OrderList* orderList;  //A pointer to a list holding ptrs to order objects

public:

    Player();
    ~Player();
    Player(string playerName);
    Player(const Player&);
    Player& operator=(const Player& aPlayer);
    string to_string();
    friend ostream& operator <<(ostream&, Player&);


    void setName(string playerName);
    string getName();
    static int getPlayerCount();
    void setPlayerID(unsigned int ID);
    unsigned int getPlayerID();
    void clearPlayersNegotiated();
    bool getCapturedTerritory();
    void resetCapturedTerritory();
    void setArmies(unsigned int armies);
    unsigned int getArmies() const;
    void Player::addArmies(unsigned int armies);
    void Player::removeArmies(unsigned int armies);

    //********* Territory methods **********//
    list<shared_ptr<Territory>> getTerritoryList();
    void addTerritory(shared_ptr<Territory> newTerritoryPtr);
    string printList(list<shared_ptr<Territory>> aList);
    list<shared_ptr<Territory>> toDefend(Map aMap);
    list<shared_ptr<Territory>> toAttack(Map* aMap);
    void updateAvailableUnits();

    //********* Card methods **********//
    Hand* getHand();

    //********** Order methods *************//
    OrderList* getOrderList();
    void issueOrder(string orderType, Map* map = NULL);
};

//*************      INSTRUCTIONS    ****************

// Player owns a collection of territories

// Players own a HandCards of Warzone cards

// Player contains methods toDefend() and toAttack() that return a list of territories that are to be defended and to be attaked, respectively

// Players contains an issueOrder() method that creates an order object and adds it to the list of orders

// Driver creates players and demonstrates that the above features are available
