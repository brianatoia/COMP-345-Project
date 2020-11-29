#pragma once

#include "../Player/Player.h"

class PlayerStrategies
{
public:
	virtual void issueOrder(string orderType, shared_ptr<Map> map) = 0;
	virtual list<shared_ptr<Territory>> toDefend(shared_ptr<Map> aMap) = 0;
	virtual  list<shared_ptr<Territory>> toAttack(shared_ptr<Map> aMap) = 0;
};

class HumanPlayerStrategy : public PlayerStrategies
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class AggressivePlayerStrategy : public PlayerStrategies
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class BenevolentPlayerStrategy : public PlayerStrategies
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class NeutralPlayerStrategy : public PlayerStrategies
{
public:
	void issueOrder(string orderType, shared_ptr<Map> map);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};