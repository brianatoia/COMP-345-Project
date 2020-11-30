#pragma once

#include "../Player/Player.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Orders/Orders.h"
#include "../GameObservers/GameObservers.h"
#include "../PlayerStrategies/PlayerStrategies.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <windows.h>

using namespace std;

class GameEngine : public Subject
{
public:

	//******     Constructor & Destructor    *****//
	GameEngine();
	GameEngine(const GameEngine&);//All functions for Gamestart here with parameters
	~GameEngine();
	GameEngine& operator=(const GameEngine&);
	friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);
	string to_string();

	//******     PART I    *****//
	void gameStart();


	//******     Map Methods    *****//
	void loadMap();
	string selectMap();
	bool isMapInDirectory(string fileName);
	shared_ptr<Map> getMap();
	vector<string> findMapNames();

	//******     Oberserver Methods    *****//
	void Observers();
	bool getObserverStatus(int);
	void setObserverStatus(int, bool);
	bool equals(const string& a, const string& b);


	//******     Deck Method    *****//
	Deck* getDeck();


	//******     Player Methods    *****//
	void addPlayers(shared_ptr<Player> player);
	vector<shared_ptr <Player>> getPlayers();
	string getPlayersInfo();
	string getPlayersNames();
	shared_ptr<Player>getNeutralPlayer();
	

	//******     PART II    *****//
	void startupPhase();

	//shared_ptr<GameEngine> createEngine();

	void mainGameLoop();
	void reinforcementsPhase();
	int findContinentBonusTotal(shared_ptr<Player> player);
	void deployLoop(shared_ptr<Player>);
	void issueOrdersPhase();
	bool atleastOneOfType(string type);
	void executeOrdersPhase();
	void checkForEliminatedPlayers();
	shared_ptr<Player> checkForWinner();


	//******     Private Members    *****//
private:
	int numOfPlayers;
	Deck* deck;
	shared_ptr<Map> map;
	vector<shared_ptr<Player>> players;
	shared_ptr<Player> neutralPlayer;

	bool phaseObserver;
	bool mapObserver;
};
