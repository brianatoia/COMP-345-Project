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

	//******     Constructor & Destructor    *****//
	GameEngine();
	GameEngine(const GameEngine&);//All functions for Gamestart here with parameters
	~GameEngine();
	GameEngine& operator=(const GameEngine&);
	friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);

	//******     PART I    *****//
	void gameStart();


	//******     Map Methods    *****//
	void loadMap();
	string selectMap();
	bool isMapInDirectory(string fileName);
	shared_ptr<Map> getMap();

	//******     Oberserver Methods    *****//
	bool Observers();
	bool getObserverStatus();
	void setObserverStatus(bool status);
	bool equals(const string& a, const string& b);


	//******     Deck Method    *****//
	Deck* getDeck();


	//******     Player Methods    *****//
	void addPlayers(shared_ptr<Player> player);
	vector<shared_ptr <Player>> getPlayers();
	string getPlayersInfo();
	string getPlayersNames();


	//******     PART II    *****//
	void startupPhase();

	//shared_ptr<GameEngine> createEngine();

	void mainGameLoop();
	void reinforcementsPhase();
	int findContinentBonusTotal(shared_ptr<Player> player);
	void deployLoop(shared_ptr<Player>);
	void issueOrdersPhase();
	void executeOrdersPhase();
	void checkForEliminatedPlayers();
	shared_ptr<Player> checkForWinner();


	//******     Private Members    *****//
private:
	int numOfPlayers;
	Deck* deck;
	shared_ptr<Map> map;
	vector<shared_ptr<Player>> players;

	bool activateObservers;
};