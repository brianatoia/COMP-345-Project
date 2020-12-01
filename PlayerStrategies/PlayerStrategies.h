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
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
	void issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck);
	list<shared_ptr<Territory>> toDefend(shared_ptr<Map> map);
	list<shared_ptr<Territory>> toAttack(shared_ptr<Map> map);
};

#endif