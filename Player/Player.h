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
#include "../PlayerStrategies/PlayerStrategies.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <tuple>



using namespace std;

static list<tuple<int, int>> playersNegotiated;
static vector<list<shared_ptr<Territory>>*> playerTerritories;

class Player
{
private:
    static int playerCount; //Static counter, counting players of the game
    unsigned int playerID;
    string name;
    bool* capturedTerritory;
    unsigned int armies;    //number of armies player has to place
    PlayerStrategy* playerStrategy;   //pointer to different concrete player strategies

    list<shared_ptr<Territory>> territoryList;  //Territory list holding pointers to territories
    Hand* hand; //A pointer to a vector list holding ptrs to warzone cards
    OrderList* orderList;  //A pointer to a list holding ptrs to order objects

public:
    static list<shared_ptr<Territory>>* getPlayerTerritories(int);

    Player();
    ~Player();
    Player(string playerName);
    Player(const Player&);
    Player& operator=(const Player& aPlayer);
    string to_string();
    friend ostream& operator <<(ostream&, Player&);

    //********* Player Methods **********//
    void setName(string playerName);
    string getName();
    static int getPlayerCount();
    void setPlayerID(unsigned int ID);
    unsigned int getPlayerID();

    //********* Game Engine Player Methods **********//
    void clearPlayersNegotiated();
    bool* getCapturedTerritory();
    void resetCapturedTerritory();
    void setArmies(unsigned int armies);
    unsigned int getArmies() const;
    void addArmies(unsigned int armies);
    void removeArmies(unsigned int armies);

    //********* Territory methods **********//
    list<shared_ptr<Territory>>* getTerritoryList();
    void addTerritory(shared_ptr<Territory> newTerritoryPtr);
    string printList(list<shared_ptr<Territory>> aList);
    list<shared_ptr<Territory>> toDefend(shared_ptr<Map> aMap);
    list<shared_ptr<Territory>> toAttack(shared_ptr<Map> aMap);
    void updateAvailableUnits();
    bool canAdvance();

    //********* Card methods **********//
    Hand* getHand();

    //********** Order methods *************//
    OrderList* getOrderList();
    void issueOrder(string orderType, shared_ptr<Map> map = NULL);

    //********** PlayerStrategy methods *************//
    void setPlayerStrategy(PlayerStrategy* newPS);
};

