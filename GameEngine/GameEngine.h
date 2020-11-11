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
	//GameEngine(const GameEngine&);//All functions for Gamestart here with parameters
	//~GameEngine();
	//GameEngine& operator= (const GameEngine&);
	//friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);
	~GameEngine();

	//******     PART I    *****//
	void gameStart();


	//******     Map Method    *****//
	void loadMap();


	//******     Oberserver Methods    *****//
	bool getObserverStatus();
	void setObserverStatus(bool status);


	//******     Deck Method    *****//
	Deck* getDeck();


	//******     Player Methods    *****//
	void addPlayers(shared_ptr<Player> player);
	vector<shared_ptr <Player>> getPlayers();
	string getPlayersInfo();
	string getPlayersNames();
	

	//******     PART II    *****//
	void startupPhase();	


	//bool mainGameLoop();

//******     Private Members    *****//
private:
	int numOfPlayers;
	Deck* deck;
	shared_ptr<Map> map;
	vector<shared_ptr<Player>> players;
	bool activateObservers;
	bool isMapInDirectory(string fileName);
	bool equals(const string& a, const string& b);
	void setNumOfPlayers();
	bool Observers();
	string selectMap();

	//bool reinforcementsPhase();
	//bool issueOrdersPhase();
	//bool executeOrdersPhase();

};
