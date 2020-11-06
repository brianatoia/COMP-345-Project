//
//  Player.cpp
//  Player
//  Defines player class and its methods. A player has a list of territories, a hand of warzone cards and a list of orders. A player can call the issueOrder method to create a new order. Additionally the player can see all territories that are either to be attacked or to be defended with the toAttack ad toDefend methods.
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

//Default constructor
Player::Player()
{
    Player::playerCount++;
    string tempName = "Player" + ::to_string(playerCount);
    this->name = tempName;
    this->playerID = playerCount;
    this->armies = 0;
    this->territoryList = list<shared_ptr<Territory>>();    //Create a list of pointers pointing to territory objects
    this->hand = new Hand();    //Creates a pointer to a Hand object which contains cards
    this->orderList = new OrderList();  //Creates a pointer to an orderlist object containing pointers to order objects
}

//Destructor which clears all parameters of pointer type
Player::~Player()
{
    for (auto i = this->territoryList.begin(); i != this->territoryList.end(); advance(i, 1))
    {
        i->reset();
    }
    territoryList.clear();

    delete hand;    //Delete pointer to hand stucture
    hand = nullptr;     //Resolve dangling pointer
    
    delete orderList;    //Delete pointer to orderList structure 
    orderList = nullptr;    //Resolve dangling pointer
    cout << "Player " << this->getName() << " with id " << this->getPlayerID() << " was deleted.\n";
}

//Parameterized constructor
Player::Player(string playerName)
{
    Player::playerCount++;
    this->name = playerName;
    this->playerID = playerCount;
    this->armies = 0;
    this->territoryList = list<shared_ptr<Territory>>();
    this->hand = new Hand();
    this->orderList = new OrderList();
}

//Copy constructor enables deep copy of pointer attributes
Player::Player(const Player& aPlayer)
{
    Player::playerCount++;
    this->name = aPlayer.name;
    this->playerID = playerCount;
    this->armies = aPlayer.armies;
  
    for (auto i = aPlayer.territoryList.begin(); i != aPlayer.territoryList.end(); advance(i, 1))
    {
        this->territoryList.push_back(*i);
    }

    this->hand = new Hand(*(aPlayer.hand));
    this->orderList = new OrderList(*(aPlayer.orderList));
}

//Assignment operator
Player& Player::operator=(const Player& aPlayer)
{
    this->name = aPlayer.name;
    this->armies = aPlayer.armies;


    for (auto i = this->territoryList.begin(); i != this->territoryList.end(); advance(i, 1))
    {
        i->reset();
    }
    territoryList.clear();

    for (auto i = aPlayer.territoryList.begin(); i != aPlayer.territoryList.end(); advance(i, 1))
    {
        this->territoryList.push_back(*i);
    }

    this->hand = new Hand(*(aPlayer.hand));
    this->orderList = new OrderList(*(aPlayer.orderList));
    return *this;
}

//ToString method of Player
string Player::to_string()
{
    string str = "\n\nPlayer " + name + " with ID " + ::to_string(playerID) + " has:\n";
    str += "\nList of Territories:\n";
    str += printList(getTerritoryList());
    str += "\nArmies to deploy: ";
    str += ::to_string(armies);
    str += "\n\nHand of Warzone cards:\n";
    Hand* h = getHand();
    str += h->to_string();
    str += "\nList of Orders:\n";
    OrderList* o = getOrderList();
    str += o->to_string();
    str += "\n";
    return str;
}

//Stream insertion operator
ostream& operator<<(ostream& strm, Player& player)
{
    return strm << player.to_string();
}

//********** Mutators and accessors *************//

void Player::setName(string playerName)
{
    this->name = playerName;
}

string Player::getName()
{
    return name;
}

void Player::setArmies(unsigned int armies)
{
    this->armies = armies;
}

unsigned int Player::getArmies() const
{
    return armies;
}

void Player::addArmies(unsigned int armies)
{
    this->armies = this->armies + armies;
}

void Player::removeArmies(unsigned int armies)
{
    this->armies = this->armies - armies;
}

int Player::getPlayerCount()
{
    return playerCount;
}

unsigned int Player::getPlayerID()
{
    return playerID;
}



//********* Territory methods **********//

//Returning TerritoryList
list<shared_ptr<Territory>> Player::getTerritoryList()
{
    return territoryList;
}

//Adding a territory to the TerritoryList and assigning the ownerId to the playerID
void Player::addTerritory(shared_ptr<Territory> newTerritoryPtr)
{
    territoryList.push_back(newTerritoryPtr);
    newTerritoryPtr->ownerID = playerID;
}

//Method toAttack - returns list of pointers to territory objects having adjacent territory not owned by the player
list<shared_ptr<Territory>> Player::toAttack(Map aMap)
{
    list<shared_ptr<Territory>> copyList;
    list<shared_ptr<Territory>>::iterator i = territoryList.begin();


    for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
    {
        vector <unsigned int> territoryIDs = (*i)->borders; //loop through adjacent territories
        for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
        {
            shared_ptr<Territory> t = aMap.getTerritory(*iD);

            if (t->ownerID != playerID) //if adjacent territory is not owned by player, add
            {
                copyList.push_back(t);
            }
        }
    }
    return copyList;
}

//Method toDefend - returns list of pointers to territory objects having adjacent territory owned by the player
list<shared_ptr<Territory>> Player::toDefend(Map aMap)
{
    list<shared_ptr<Territory>> copyList;
    list<shared_ptr<Territory>>::iterator i = territoryList.begin();


    for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
    {
        vector <unsigned int> territoryIDs = (*i)->borders;
        for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
        {
            shared_ptr<Territory> t = aMap.getTerritory(*iD);

            if (t->ownerID == playerID) //if adjacent territory is owned by the player, add
            {
                copyList.push_back(t);
            }
        }
    }
    return copyList;
}

//method that takes any list or pointers as input and returns it as string
string Player::printList(list<shared_ptr<Territory>> aList)
{
    string tList = "";
    list<shared_ptr<Territory>>::iterator it = aList.begin();
    for (it = aList.begin(); it != aList.end(); advance(it, 1))
    {
        shared_ptr<Territory> t = *it;
        tList += t->to_string() + "\n";
    }
    return tList;
}


//********* Card methods **********//

Hand* Player::getHand()
{
    return hand;
}


//********** Order methods *************//

OrderList* Player::getOrderList()
{
    return orderList;
}

//Method issueOrder - creates a new order objects according to orderType and adds it to the players OrderList
void Player::issueOrder(string orderType)
{
    if (orderType == "Deploy")
    {
        shared_ptr<Order> order(new Deploy);
        this->orderList->addOrder(order);
    }
    else if (orderType == "Advance")
    {
        shared_ptr<Order> order(new Advance);
        this->orderList->addOrder(order);
    }
    else if (orderType == "Bomb")
    {
        shared_ptr<Order> order(new Bomb);
        this->orderList->addOrder(order);
    }
    else if (orderType == "Blockade")
    {
        shared_ptr<Order> order(new Blockade);
        this->orderList->addOrder(order);
    }
    else if (orderType == "Airlift")
    {
        shared_ptr<Order> order(new Airlift);
        this->orderList->addOrder(order);
    }
    else if (orderType == "Negotiate")
    {
        shared_ptr<Order> order(new Negotiate);
        this->orderList->addOrder(order);
    }
    else
    {
        cerr << "Invalid Order Type" << endl;
    }
}





