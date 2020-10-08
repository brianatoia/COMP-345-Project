//
//  main.cpp
//  Player
//
//
//
//  Created by Lina Kretzschmar on 2020-09-27.
//

//

#include "Player.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>

using namespace std;
    
    //***********  Constructors ************

    //Default constructor
    Player::Player(){
        playerCount++;
        string tempName = "Player" + to_string(playerCount);
        cout << "Generic Name: "  << tempName << endl;
        this->name = tempName;
        
        //add all three lists
        
        this->orderList = new OrderList();
    }
    
    //Initialized constructor
    Player::Player(string playerName){
        playerCount++;
        this->name = playerName;
     
    //territory List = new Territory List(); list with territory objects
    //CollectionOfCards = new CollectionOfCards(); List with card objects
        this->orderList = new OrderList();
    };


    //Copy Constructor
    //String insertion operator
    
    //********** Mutators and accessors *************
    void Player::setName(string playerName){
        this->name = playerName;
    };
    
   
        
  
    
    int Player::getPlayerCount(){
        return playerCount;
    };

    string Player::getName (){
        return name;
    };

    OrderList * Player::getOrderList(){
        return orderList;
    }
    
//********* Special methods **********
    
/**
    territory <list> toDefend(){
 return territory list including every territory the player owns}
 
    territory <llist> toAttack(){
 return territory list including ever territory but the players}
 
 */
 

    //Check the string and create order according to that
    //If print out cerror and say invalid error
    void Player::issueOrder(string orderType){
        shared_ptr<Order> order (new Order(orderType));
        
        
        
        this->orderList->addOrder(order);
        
//        cout << *orderList << endl;
////        OrderList testList ();
//        cout << orderList->getOrders().size() << endl;
    }
 

    


