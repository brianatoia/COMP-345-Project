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
	this->capturedTerritory = new bool(aPlayer.capturedTerritory);

    return *this;
}

//ToString method of Player
string Player::to_string()
{
    string str = "\n\nPlayer " + name + " with ID " + ::to_string(playerID) + " has:";
    str += "\nList of Territories--------\n";
    str += printList(getTerritoryList());
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

//Resets the available units to units at beginning of turn
void Player::updateAvailableUnits()
{
	for (auto territory : territoryList)
	{
		territory->availableUnits = territory->units;
	}
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
		shared_ptr<Territory> territory;
		string territoryName;

		cout << "You have " << armies << " deployable armies" << endl;

		//add check for correct territoy name
		cout << "\nWhich territory would you like to deploy to?" << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << " ("<< t->availableUnits << ")"<< endl;
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

		cout << "\nHow many armies would you like to deploy? (Max " << armies << ")" << endl;
		cout << "--------------------------------------------" << endl;
		int numberOfArmiesToDeploy;
		while (true)
		{
			cin >> numberOfArmiesToDeploy;
			if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > armies || cin.fail())
			{
				cout << "\nInvalid input, please try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
			}
			else break;
		}

		territory->availableUnits += numberOfArmiesToDeploy;
		armies -= numberOfArmiesToDeploy;

		shared_ptr<Order> order(new Deploy(numberOfArmiesToDeploy, territory, territoryList));
		this->orderList->addOrder(order);

		order->execute();
		cout << *order << endl;
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
				if (t->availableUnits > 0)
				{
					playerTerritoryOptions.push_back(t);
					cout << t->name << " (" <<t->availableUnits << ")" << endl;
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
					cout << territory->name << " (" << territory->availableUnits << ")" << endl;
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
					cout << territory->name << " (" << territory->units << ")" << endl;
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
			cout << "Enter number of armies to advance. Should be <= " << sourceTerritory->availableUnits << " and >= 0: ";
			cin >> numOfArmies;
		} while (numOfArmies >= sourceTerritory->availableUnits || numOfArmies < 0);

		sourceTerritory->availableUnits -= numOfArmies;

		list<shared_ptr<Territory>>* temp = &territoryList;
		shared_ptr<Order> order(new Advance(numOfArmies, sourceTerritory, targetTerritory, temp, playerTerritories[targetTerritory->ownerID - 1],capturedTerritory, playersNegotiated));
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
			cout << t->name << " (" <<t->availableUnits << ")" << endl;
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
				if (t->availableUnits > 0)
				{
					playerTerritoryOptions.push_back(t);
					cout << t->name << " (" << t->availableUnits << ")" << endl;
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
					cout << territory->name << " (" << territory->availableUnits << ")" << endl;
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
    
		targetTerritory->availableUnits += numOfArmies;

		shared_ptr<Order> order(new Airlift(numOfArmies, sourceTerritory, targetTerritory, temp, playerTerritories[targetTerritory->ownerID - 1], capturedTerritory, playersNegotiated));
		this->orderList->addOrder(order);

		order->execute();
		cout << *order << endl;
	}
	else if (orderType == "Negotiate")
	{
		int targetPlayerID;
		do
		{
			cout << "Enter the ID of the player you would like to negotiate with: ";
			cin >> targetPlayerID;
		} while (targetPlayerID > playerCount && targetPlayerID < 0);


		list<tuple<int, int>>* temp = &playersNegotiated;
		shared_ptr<Order> order(new Negotiate(playerID, targetPlayerID, temp));
		this->orderList->addOrder(order);
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}
