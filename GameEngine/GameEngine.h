#pragma once

#include "../player/Player.h"
//#include "../Cards/Cards.h"
#include "Cards.h"//have to correct this with the correct folder structure after
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Orders/Orders.h"


#include <iostream>

using namespace std;

class GameEngine
{
public:
	GameEngine();//Error since parameters have to be provided
	GameEngine(const GameEngine&);//All functions for Gamestart here with parameters
	~GameEngine();
	GameEngine& operator= (const GameEngine&);
	friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);

	void addPlayers(Player* player);
	list <shared_ptr<Player*>> getPlayers();
	

	void startupPhase(Map* map, list<shared_ptr<Player*>> playerList);	//Part 2

	shared_ptr<GameEngine> createEngine();

	bool mainGameLoop();

private:
	list<shared_ptr<Player*>> players;
	bool reinforcementsPhase();
	bool issueOrdersPhase();
	bool executeOrdersPhase();

};
