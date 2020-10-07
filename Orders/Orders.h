#pragma once
#include <iostream>;
#include <list>;
using namespace std;

class Order
{
private:
	string orderType;
	string orderDescription;
	string orderEffect;
protected:
	void setOrderEffect(string);
	Order(string);
public:
	Order();
	string getOrderType();
	string getOrderDescription();
	string getOrderEffect();
	void setOrderType(string);
	void setOrderDescription(string);
	virtual bool validate();
	virtual void execute();
	string to_string();
	Order(const Order&);
	Order& operator=(const Order&);
	friend ostream& operator<<(ostream&, Order&);
};

class OrderList
{
private:
	list<shared_ptr<Order>> orders;
public:
	OrderList();
	~OrderList();
	list<shared_ptr<Order>> getOrders();
	void setOrders(list<shared_ptr<Order>>);
	void addOrder(shared_ptr<Order>);
	void remove(shared_ptr<Order>);
	void move(shared_ptr<Order>, int);
	string to_string();
	OrderList(const OrderList&);
	OrderList& operator=(const OrderList&);
	friend ostream& operator<<(ostream&, OrderList&);
};


class Deploy : public Order
{
public:
	Deploy();
	bool validate();
	void execute();
	Deploy(const Deploy&);
	Deploy& operator=(const Deploy&);
};

class Advance : public Order
{
public:
	Advance();
	bool validate();
	void execute();
	Advance(const Advance&);
	Advance& operator=(const Advance&);
};

class Bomb : public Order
{
public:
	Bomb();
	bool validate();
	void execute();
	Bomb(const Bomb&);
	Bomb& operator=(const Bomb&);
};

class Blockade : public Order
{
public:
	Blockade();
	bool validate();
	void execute();
	Blockade(const Blockade&);
	Blockade& operator=(const Blockade&);
};

class Airlift : public Order
{
public:
	Airlift();
	bool validate();
	void execute();
	Airlift(const Airlift&);
	Airlift& operator=(const Airlift&);
};

class Negotiate : public Order
{
public:
	Negotiate();
	bool validate();
	void execute();
	Negotiate(const Negotiate&);
	Negotiate& operator=(const Negotiate&);
};

