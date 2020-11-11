#pragma once

#include "../player/Player.h"
#include "../cards/Cards.h"
//#include "Cards.h"//have to correct this with the correct folder structure after
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Orders/Orders.h"


#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

class GameEngine
{
public:
	GameEngine();
	//GameEngine(const GameEngine&);//All functions for Gamestart here with parameters
	//~GameEngine();
	//GameEngine& operator= (const GameEngine&);
	//friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);
	void GameStart();

	void addPlayers(shared_ptr<Player> player);
	vector <shared_ptr<Player>> getPlayers();
	string getPlayersInfo();
	string getPlayersNames();

	void setMap(shared_ptr<Map> newName);
	void loadMap();
	

	//Part 2
	void startupPhase();	

	//shared_ptr<GameEngine> createEngine();

	//bool mainGameLoop();

private:
	shared_ptr<Map> map;
	vector<shared_ptr<Player>> players;
	//bool reinforcementsPhase();
	//bool issueOrdersPhase();
	//bool executeOrdersPhase();

};
