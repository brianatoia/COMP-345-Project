#pragma once
#include <iostream>
#include <list>
#include "../Map/Map.h"

using namespace std;

class Order
{
public:
	enum OrderType
	{
		UNDEFINED,
		DEPLOY,
		ADVANCE,
		BOMB,
		BLOCKADE,
		AIRLIFT,
		NEGOTIATE
	};
	Order();
	~Order();
	enum OrderType* getOrderType();
	string getOrderTypeString();
	string getOrderDescription();
	string getOrderEffect();
	void setOrderType(OrderType*);
	void setOrderDescription(string);
	virtual bool validate() = 0;
	virtual void execute() = 0;
	string to_string();
	Order(const Order&);
	Order& operator=(const Order&);
	friend ostream& operator<<(ostream&, Order&);
private:
	OrderType* orderType;
	string orderDescription;
	string orderEffect;
protected:
	void setOrderEffect(string);
	Order(OrderType*);
};

class OrderList
{
public:
	enum MoveOption
	{
		BEGINNING,
		END,
		UP,
		DOWN
	};
	OrderList();
	~OrderList();
	list<shared_ptr<Order>> getOrders();
	void setOrders(list<shared_ptr<Order>>);
	void addOrder(shared_ptr<Order>);
	void remove(shared_ptr<Order>);
	void move(shared_ptr<Order>, MoveOption*);
	bool hasOrderType(string type);
	shared_ptr<Order> getOrder(string type);
	string to_string();
	OrderList(const OrderList&);
	OrderList& operator=(const OrderList&);
	friend ostream& operator<<(ostream&, OrderList&);
private:
	list<shared_ptr<Order>> orders;
};


class Deploy : public Order
{
public:
	Deploy(int, shared_ptr<Territory>, list<shared_ptr<Territory>>);
	bool validate();
	void execute();
	Deploy(const Deploy&);
	Deploy& operator=(const Deploy&);
	~Deploy();

private:
	int numOfArmies;
	shared_ptr<Territory> territory;
	list<shared_ptr<Territory>> playerTerritories;
};

class Advance : public Order
{
public:
	Advance(int, shared_ptr<Territory>, shared_ptr<Territory>, list<shared_ptr<Territory>>*, list<shared_ptr<Territory>>*, bool*, list<tuple<int, int>>*);
	bool validate();
	void execute();
	Advance(const Advance&);
	Advance& operator=(const Advance&);
	~Advance();

private:
	int numOfArmies;
	shared_ptr<Territory> sourceTerritory;
	shared_ptr<Territory> targetTerritory;
	list<shared_ptr<Territory>>* playerTerritories;
	list<shared_ptr<Territory>>* targetPlayerTerritories;
	bool* capturedTerritory;
	list<tuple<int, int>>* playersNegotiated;
};

class Bomb : public Order
{
public:
	Bomb(unsigned int, shared_ptr<Territory>, list<shared_ptr<Territory>>*, list<tuple<int, int>>*);
	bool validate();
	void execute();
	Bomb(const Bomb&);
	Bomb& operator=(const Bomb&);
	~Bomb();
private:
	unsigned int attackerID;
	shared_ptr<Territory> targetTerritory;
	list<shared_ptr<Territory>>* playerTerritories;
	list<tuple<int, int>>* playersNegotiated;
};

class Blockade : public Order
{
public:
	Blockade(shared_ptr<Territory>, list<shared_ptr<Territory>>*);
	bool validate();
	void execute();
	Blockade(const Blockade&);
	Blockade& operator=(const Blockade&);
	~Blockade();
private:
	shared_ptr<Territory> targetTerritory;
	list<shared_ptr<Territory>>* playerTerritories;
};

class Airlift : public Order
{
public:
	Airlift(int, shared_ptr<Territory>, shared_ptr<Territory>, list<shared_ptr<Territory>>*, list<shared_ptr<Territory>>*, bool*, list<tuple<int, int>>*);
	bool validate();
	void execute();
	Airlift(const Airlift&);
	Airlift& operator=(const Airlift&);
	~Airlift();
private:
	int numOfArmies;
	shared_ptr<Territory> sourceTerritory;
	shared_ptr<Territory> targetTerritory;
	list<shared_ptr<Territory>>* playerTerritories;
	list<shared_ptr<Territory>>* targetPlayerTerritories;
	bool* capturedTerritory;
	list<tuple<int, int>>* playersNegotiated;
};

class Negotiate : public Order
{
public:
	Negotiate(int, int, list<tuple<int, int>>*);
	bool validate();
	void execute();
	Negotiate(const Negotiate&);
	Negotiate& operator=(const Negotiate&);
	~Negotiate();
private:
	int sourcePlayerID;
	int targetPlayerID;
	list<tuple<int, int>>* playersNegotiated;
};
