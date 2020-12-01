#ifndef PLAYER_STRATEGIES_H

#define PLAYER_STRATEGIES_H 1

#pragma once

#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"

#include <list>
#include <memory>

using namespace std;

class GameEngine;
class Player;

class PlayerStrategy
{
public:
	virtual void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck) = 0;
	virtual list<shared_ptr<Territory>> toDefend(shared_ptr<Map> aMap) = 0;
	virtual list<shared_ptr<Territory>> toAttack(shared_ptr<Map> aMap) = 0;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
	HumanPlayerStrategy(const HumanPlayerStrategy& hps);
	HumanPlayerStrategy& operator=(const HumanPlayerStrategy& hps);
	friend ostream& operator<<(ostream& strm, HumanPlayerStrategy hps);
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
	AggressivePlayerStrategy(const AggressivePlayerStrategy& hps);
	AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& hps);
	friend ostream& operator<<(ostream& strm, AggressivePlayerStrategy hps);
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& hps);
	BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& hps);
	friend ostream& operator<<(ostream& strm, BenevolentPlayerStrategy hps);
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
	NeutralPlayerStrategy(const NeutralPlayerStrategy& hps);
	NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& hps);
	friend ostream& operator<<(ostream& strm, NeutralPlayerStrategy hps);
};

#endif