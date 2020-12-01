//
//  Player.cpp
//  Player
//  Defines player class and its methods. A player has a list of territories, a hand of warzone cards and a list of orders. A player can call the issueOrder method to create a new order. Additionally the player can see all territories that are either to be attacked or to be defended with the toAttack ad toDefend methods.
//
//
//  Created by Lina Kretzschmar on 2020-09-27.
//

#include "Player.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <tuple>

using namespace std;


int Player::playerCount = 0;

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
	this->capturedTerritory = new bool(false);
	playerTerritories.push_back(&territoryList);
	this->playerStrategy = new NeutralPlayerStrategy();
}

//Destructor which clears all parameters of pointer type
Player::~Player()
{
	for (auto i = this->territoryList.begin(); i != this->territoryList.end(); advance(i, 1))
	{
		i->reset();
	}
	territoryList.clear();

	// This fails (we fixed it in a later commit
	delete playerStrategy;
	playerStrategy = nullptr;

	delete hand;    //Delete pointer to hand stucture
	hand = nullptr;     //Resolve dangling pointer

	delete orderList;    //Delete pointer to orderList structure
	orderList = nullptr;    //Resolve dangling pointer

	delete capturedTerritory;    //Delete pointer to orderList structure
	capturedTerritory = nullptr;	//Resolve dangling pointer

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
	this->capturedTerritory = new bool(false);
	playerTerritories.push_back(&territoryList);
	this->playerStrategy = new NeutralPlayerStrategy();
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
	this->capturedTerritory = aPlayer.capturedTerritory;
	playerTerritories.push_back(&territoryList);
	this->playerStrategy = copyPlayerStrategy(aPlayer);
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
	this->capturedTerritory = aPlayer.capturedTerritory;
	
	delete this->playerStrategy;
	this->playerStrategy = nullptr;
	this->playerStrategy = copyPlayerStrategy(aPlayer);

	return *this;
}

//ToString method of Player
string Player::to_string()
{
	string str = "\n\nPlayer " + name + " with ID " + ::to_string(playerID) + " and player strategy " + this->playerStrategy->to_string() + ":";
	str += "\nList of Territories--------\n";
	str += printList(*getTerritoryList());
	str += "\nArmies to deploy--------\n";
	str += ::to_string(armies);
	str += "\nHand of Warzone cards--------\n";
	Hand* h = getHand();
	str += h->to_string();
	str += "\nList of Orders--------\n";
	OrderList* o = getOrderList();
	str += o->to_string();
	str += "\nHas Captured at least one territory this turn--------\n";
	str += ::to_string(*capturedTerritory);
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

void Player::setPlayerID(unsigned int ID)
{
	this->playerID = ID;
}

unsigned int Player::getPlayerID()
{
	return playerID;
}

void Player::clearPlayersNegotiated()
{
	playersNegotiated.clear();
}

bool* Player::getCapturedTerritory()
{
	return capturedTerritory;
}

void Player::resetCapturedTerritory()
{
	*(this->capturedTerritory) = false;
}

 list<shared_ptr<Territory>>* Player::getPlayerTerritories(int playerID)
{
	if (playerID > 0 && playerID <= playerTerritories.size())
	{
		return playerTerritories[playerID - 1];
	}
	else
	{
		return nullptr;
	}
}


//********* Territory methods **********//

//Returning TerritoryList
list<shared_ptr<Territory>>* Player::getTerritoryList()
{
	return &territoryList;
}

//Adding a territory to the TerritoryList and assigning the ownerId to the playerID
void Player::addTerritory(shared_ptr<Territory> newTerritoryPtr)
{
	territoryList.push_back(newTerritoryPtr);
	newTerritoryPtr->ownerID = playerID;
}

//Resets the available units to units at beginning of turn
void Player::updateAvailableUnits()
{
	for (auto territory : territoryList)
	{
		territory->availableUnits = territory->units;
	}
}

bool Player::canAdvance()
{
	for (auto territory : territoryList)
	{
		if (territory->availableUnits > 0) return true;
	}
	return false;
}

//Method toAttack - returns list of pointers to territory objects having adjacent territory not owned by the player
list<shared_ptr<Territory>> Player::toAttack(shared_ptr<Map> aMap)
{
	list<shared_ptr<Territory>> copyList;
	list<shared_ptr<Territory>>::iterator i = territoryList.begin();


	for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
	{
		vector <unsigned int> territoryIDs = (*i)->borders; //loop through adjacent territories
		for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
		{
			shared_ptr<Territory> t = aMap->getTerritory(*iD);

			if (t->ownerID != playerID) //if adjacent territory is not owned by player, add
			{
				copyList.push_back(t);
			}
		}
	}

	copyList.sort();
	copyList.unique();

	return copyList;
}

//Method toDefend - returns list of pointers to territory objects having adjacent territory owned by the player
list<shared_ptr<Territory>> Player::toDefend(shared_ptr<Map> aMap)
{
	list<shared_ptr<Territory>> copyList;
	list<shared_ptr<Territory>>::iterator i = territoryList.begin();


	for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
	{
		vector <unsigned int> territoryIDs = (*i)->borders;
		for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
		{
			shared_ptr<Territory> t = aMap->getTerritory(*iD);

			if (t->ownerID == playerID) //if adjacent territory is owned by the player, add
			{
				copyList.push_back(t);
			}
		}
	}

	copyList.sort();
	copyList.unique();

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
void Player::issueOrder(string orderType, shared_ptr<Map> map)
{
	if (orderType == "Deploy")
	{
		

	}
	else if (orderType == "Advance")
	{
		
	}
	else if (orderType == "Bomb")
	{
		
	}
	else if (orderType == "Blockade")
	{
		
	}
	else if (orderType == "Airlift")
	{
		
	}
	else if (orderType == "Negotiate")
	{
		
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}

void Player::issueOrder2(GameEngine* gameEngine, shared_ptr<Map> map, Deck* deck)
{
	this->playerStrategy->issueOrder(gameEngine, this, map, deck);
}

//********** PlayerStrategy methods *************//

void Player::setPlayerStrategy(PlayerStrategy* newPS)
{
	this->playerStrategy = newPS;
}

//Returns the concrete strategy type of the player as a string
PlayerStrategy* Player::copyPlayerStrategy(const Player& aPlayer)
{
	if (aPlayer.playerStrategy->to_string() == "HumanPlayerStrategy")
	{
		HumanPlayerStrategy* hPS = new HumanPlayerStrategy();
		return hPS;
	}
	else if (aPlayer.playerStrategy->to_string() == "AggressivePlayerStrategy")
	{
		AggressivePlayerStrategy* aPS = new AggressivePlayerStrategy();
		return aPS;
	}
	else if (aPlayer.playerStrategy->to_string() == "BenevolentPlayerStrategy")
	{
		BenevolentPlayerStrategy* bPS = new BenevolentPlayerStrategy();
		return bPS;
	}
	else if (aPlayer.playerStrategy->to_string() == "NeutralPlayerStrategy")
	{
		NeutralPlayerStrategy* nPS = new NeutralPlayerStrategy();
		return nPS;
	}
}
