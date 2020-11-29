#pragma once

#include "../Map/Map.h"

#include <list>
#include <memory>

using namespace std;

class PlayerStrategy
{
public:
	virtual void issueOrder(string orderType, shared_ptr<Map> map) = 0;
	virtual list<shared_ptr<Territory>> toDefend(shared_ptr<Map> aMap) = 0;
	virtual list<shared_ptr<Territory>> toAttack(shared_ptr<Map> aMap) = 0;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};