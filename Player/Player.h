//
//  Header.h
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



using namespace std;

 /* Header_h */

class Player
{
    
private:
    static int playerCount;
    string name;
    Card * card;
    Map * map;
    Map * territory;
    OrderList * orderList;
    
public:
    
    Player();
    Player(string playerName);
    
    
    void setName (string playerName);
    
    
    static int getPlayerCount();
    
    
    string getName ();
    
    OrderList * getOrderList();
    
    
    
    
    
    /**
     territory  <list>  showTerritories()--> List of pointers to territories returns list of territories
     card  <list> showCards()
     order  <list> showOrder()
     
     territory <> toDefend() --> returns arbitrary list of territories
     territory <> toAttack()
     */
    
    void issueOrder(string orderType);
    
    
};

//*************      INSTRUCTIONS    ****************

// Player owns a collection of territories

// Players own a hand of Warzone cards

// Player contains methods toDefend() and toAttack() that return a list of territories that are to be defended and to be attaked, respectively

// Players contains a issueOrder() method that creates an order object and adds it to the list of orders

// Driver creates players and demonstrates that the above features are available

