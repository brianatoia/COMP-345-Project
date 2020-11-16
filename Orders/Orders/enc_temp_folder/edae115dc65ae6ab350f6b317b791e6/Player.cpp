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
	playerCount++;
	string tempName = "Player" + ::to_string(playerCount);
	this->name = tempName;
	this->playerID = playerCount;
	this->territoryList = list<shared_ptr<Territory>>();    //Create a list of pointers pointing to territory objects
	this->hand = new Hand();    //Creates a pointer to a Hand object which contains cards
	this->orderList = new OrderList();  //Creates a pointer to an orderlist object containing pointers to order objects
	*(this->capturedTerritory) = false;
}

//Destructor which clears all parameters of pointer type
Player::~Player()
{
	for (shared_ptr<Territory> t : this->territoryList)
	{
		t.reset();      //Go through list and reset before clearing
	}
	territoryList.clear();


	//delete hand;    //Delete pointer to hand stucture
	//hand = nullptr;     //Resolve dangling pointer

	delete orderList;    //Delete pointer to orderList structure 
	orderList = nullptr;    //Resolve dangling pointer
}

//Parameterized constructor
Player::Player(string playerName)
{
	playerCount++;
	this->name = playerName;
	this->playerID = playerCount;
	this->territoryList = list<shared_ptr<Territory>>();
	this->hand = new Hand();
	this->orderList = new OrderList();
	*(this->capturedTerritory) = false;
}

//Copy constructor enables deep copy of pointer attributes
Player::Player(const Player& aPlayer)
{
	this->name = aPlayer.name;
	this->playerID = playerCount;
	this->territoryList = aPlayer.territoryList;
	this->hand = aPlayer.hand;
	this->orderList = aPlayer.orderList;
	this->capturedTerritory = aPlayer.capturedTerritory;
	this->playersNegotiated = aPlayer.playersNegotiated;
}

//Assignment operator
Player& Player::operator=(const Player& aPlayer)
{
	this->territoryList.clear();

	this->name = aPlayer.name;
	this->playerID = playerCount;
	this->territoryList = aPlayer.territoryList;
	this->hand = aPlayer.hand;
	this->orderList = aPlayer.orderList;
	this->capturedTerritory = aPlayer.capturedTerritory;
	this->playersNegotiated = aPlayer.playersNegotiated;

	return *this;
}

//ToString method of Player
string Player::to_string()
{
	string str = "\n\nPlayer " + name + " has ID " + ::to_string(playerID) + " and owns:\n";
	str += "\nList of Territories:\n";
	str += printList(getTerritoryList());
	str += "\nHand of Warzone cards:\n";
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

int Player::getPlayerCount()
{
	return playerCount;
}

unsigned int Player::getPlayerID()
{
	return playerID;
}

void Player::clearPlayersNegotiated()
{
	playersNegotiated.clear();
}

bool Player::getCapturedTerritory()
{
	return *capturedTerritory;
}

void Player::resetCapturedTerritory()
{
	*(this->capturedTerritory) = false;
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
list<shared_ptr<Territory>> Player::toAttack(Map* aMap)
{
	list<shared_ptr<Territory>> copyList;
	list<shared_ptr<Territory>>::iterator i = territoryList.begin();


	for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
	{
		vector <unsigned int> territoryIDs = (*i)->borders;
		for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
		{
			shared_ptr<Territory> t = aMap->getTerritory(*iD);

			if (t->ownerID != playerID)
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
			if (t->ownerID == playerID)
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
void Player::issueOrder(string orderType, Map* map)
{
	if (orderType == "Deploy")
	{
		int numOfArmies = 10; //change to get from reinforcement pool
		shared_ptr<Territory> territory;
		string territoryName;

		cout << "\nWhich territory would you like to deploy to?" << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << endl;
		}
		cout << "--------------------------------------------" << endl;
		cin >> territoryName;

		for (shared_ptr<Territory> t : territoryList)
		{
			string name = t->name;
			transform(territoryName.begin(), territoryName.end(), territoryName.begin(), ::tolower);
			transform(name.begin(), name.end(), name.begin(), ::tolower);
			if (territoryName == name)
			{
				territory = t;
				break;
			}
		}

		shared_ptr<Order> order(new Deploy(numOfArmies, territory, territoryList));
		this->orderList->addOrder(order);

		order->execute();
	}
	else if (orderType == "Advance")
	{
		string sourceTerritoryName;
		string targetTerritoryName;
		shared_ptr<Territory> sourceTerritory = nullptr;
		shared_ptr<Territory> targetTerritory = nullptr;
		list<shared_ptr<Territory>> playerTerritoryOptions;
		int numOfArmies = 0;
		bool territoryAllowed = false;

		do {
			cout << "Current territories you can advance from: " << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : territoryList)
			{
				if (t->units > 0)
				{
					playerTerritoryOptions.push_back(t);
					cout << t->name + " " + std::to_string(t->units) + " units." << endl;
				}
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nEnter territory you would like to advance from: " << endl;
			cin >> sourceTerritoryName;
			sourceTerritory = map->findTerritory(sourceTerritoryName);

			for (shared_ptr<Territory> t : playerTerritoryOptions)
			{
				if (t == sourceTerritory)
				{
					territoryAllowed = true;
				}
			}

		} while (!territoryAllowed);

		territoryAllowed = false;
		do {
			cout << "Current territories you can move to: " << endl;
			cout << "--------------------------------------------" << endl;
			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory->ownerID == sourceTerritory->ownerID)
				{
					cout << territory->name + " " + std::to_string(territory->units) + " units." << endl;
				}
			}
			cout << "--------------------------------------------" << endl;

			cout << "Current territories you can attack: " << endl;
			cout << "--------------------------------------------" << endl;
			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory->ownerID != sourceTerritory->ownerID)
				{
					cout << territory->name + " " + std::to_string(territory->units) + " units." << endl;
				}
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nEnter territory you would like to advance to: " << endl;
			cin >> targetTerritoryName;
			targetTerritory = map->findTerritory(targetTerritoryName);

			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory == targetTerritory)
				{
					territoryAllowed = true;
				}
			}
		} while (!territoryAllowed);

		do
		{
			cout << "Enter number of armies to advance. Should be <= " + std::to_string(sourceTerritory->units) + " and >= 0: ";
			cin >> numOfArmies;
		} while (numOfArmies >= sourceTerritory->units && numOfArmies < 0);

		shared_ptr<Order> order(new Advance(numOfArmies, sourceTerritory, targetTerritory, territoryList, capturedTerritory, playersNegotiated));
		this->orderList->addOrder(order);

		order->execute();
		cout << *order << endl;
	}
	else if (orderType == "Bomb")
	{
		shared_ptr<Territory> territory = nullptr;
		string territoryName;

		do {
			cout << "\nWhich territory would you like to bomb?" << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : toAttack(map))
			{
				cout << t->name << endl;
			}
			cout << "--------------------------------------------" << endl;
			cin >> territoryName;

			for (shared_ptr<Territory> t : toAttack(map))
			{
				string name = t->name;
				transform(territoryName.begin(), territoryName.end(), territoryName.begin(), ::tolower);
				transform(name.begin(), name.end(), name.begin(), ::tolower);
				if (territoryName == name)
				{
					territory = t;
					break;
				}
			}
		} while (territory == nullptr);

		shared_ptr<Order> order(new Bomb(territory, territoryList));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Blockade")
	{
		shared_ptr<Territory> territory;
		string territoryName;

		cout << "\nWhich territory would you like to blockade?" << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << endl;
		}
		cout << "--------------------------------------------" << endl;
		cin >> territoryName;

		for (shared_ptr<Territory> t : territoryList)
		{
			string name = t->name;
			transform(territoryName.begin(), territoryName.end(), territoryName.begin(), ::tolower);
			transform(name.begin(), name.end(), name.begin(), ::tolower);
			if (territoryName == name)
			{
				territory = t;
				break;
			}
		}

		shared_ptr<Order> order(new Blockade(territory, territoryList));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Airlift")
	{
		string sourceTerritoryName;
		string targetTerritoryName;
		shared_ptr<Territory> sourceTerritory = nullptr;
		shared_ptr<Territory> targetTerritory = nullptr;
		list<shared_ptr<Territory>> playerTerritoryOptions;
		int numOfArmies = 0;
		bool territoryAllowed = false;
		int numOfTargets = 0;

		do {
			cout << "Current territories you can airlift from: " << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : territoryList)
			{
				if (t->units > 0)
				{
					playerTerritoryOptions.push_back(t);
					cout << t->name + " " + std::to_string(t->units) + " units." << endl;
				}
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nEnter territory you would like to airlift from: " << endl;
			cin >> sourceTerritoryName;
			sourceTerritory = map->findTerritory(sourceTerritoryName);

			for (shared_ptr<Territory> t : playerTerritoryOptions)
			{
				if (t == sourceTerritory)
				{
					territoryAllowed = true;
				}
			}

		} while (!territoryAllowed);

		territoryAllowed = false;
		do {
			cout << "Current territories you can move to: " << endl;
			cout << "--------------------------------------------" << endl;
			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory->ownerID == sourceTerritory->ownerID)
				{
					cout << territory->name + " " + std::to_string(territory->units) + " units." << endl;
					numOfTargets++;
				}
			}
			if (numOfTargets == 0)
			{
				cout << "No available territories to move to. Cancelling Airlift." << endl;
				return;
			}
			cout << "--------------------------------------------" << endl;

			cout << "\nEnter territory you would like to airlift to: " << endl;
			cin >> targetTerritoryName;
			targetTerritory = map->findTerritory(targetTerritoryName);

			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory == targetTerritory)
				{
					territoryAllowed = true;
				}
			}
		} while (!territoryAllowed);

		do
		{
			cout << "Enter number of armies to airlift. Should be <= " + std::to_string(sourceTerritory->units) + " and >= 0: ";
			cin >> numOfArmies;
		} while (numOfArmies >= sourceTerritory->units && numOfArmies < 0);

		shared_ptr<Order> order(new Airlift(numOfArmies, sourceTerritory, targetTerritory, territoryList, capturedTerritory, playersNegotiated));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Negotiate")
	{
		int targetPlayerID;
		do
		{
			cout << "Enter the ID of the player you would like to negotiate with: ";
			cin >> targetPlayerID;
		} while (targetPlayerID > playerCount && targetPlayerID < 0);

		shared_ptr<Order> order(new Negotiate(playerID, targetPlayerID, playersNegotiated));
		this->orderList->addOrder(order);

		order->execute();
		cout << *order << endl;
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}





