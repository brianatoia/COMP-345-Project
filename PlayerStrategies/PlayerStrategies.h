#pragma once

#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Orders/Orders.h"

#include <list>
#include <memory>
#include <utility>
#include <iostream>

#ifndef NEUTRAL_PLAYER_ID
#define NEUTRAL_PLAYER_ID 0
#endif

using namespace std;

enum StrategyType {
	NONE, NEUTRAL, HUMAN, AGGRESSIVE, BENEVOLENT
};

class PlayerStrategy
{
	unsigned int playerID;
	Hand* hand; //A pointer to a vector list holding ptrs to warzone cards
	OrderList* orderList;  //A pointer to a list holding ptrs to order objects
	StrategyType* strategyType;
public:
	PlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList, StrategyType* strategyType = nullptr);
	~PlayerStrategy();
	virtual string issueOrder(shared_ptr<Map> map) = 0;
	virtual pair<shared_ptr<Territory>, int> deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList) = 0;
	virtual list<shared_ptr<Territory>> toDefend(shared_ptr<Map> aMap, list<shared_ptr<Territory>> territoryList);
	virtual list<shared_ptr<Territory>> toAttack(shared_ptr<Map> aMap, list<shared_ptr<Territory>> territoryList);
	StrategyType getStrategyType();
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
	HumanPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList);
	string issueOrder(shared_ptr<Map> map);
	pair<shared_ptr<Territory>, int> deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList);
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
	AggressivePlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList);
	string issueOrder(shared_ptr<Map> map);
	pair<shared_ptr<Territory>, int> deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList);
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
	BenevolentPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList);
	string issueOrder(shared_ptr<Map> map);
	pair<shared_ptr<Territory>, int> deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList);
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
	NeutralPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList);
	string issueOrder(shared_ptr<Map> map);
	pair<shared_ptr<Territory>, int> deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList);
};