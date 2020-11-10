#pragma once

#include <iostream>
#include "boost/filesystem.hpp"

using namespace std;

struct GameEngine
{
	GameEngine();
	GameEngine(const GameEngine&);
	~GameEngine();
	GameEngine& operator= (const GameEngine&);
	friend ostream& operator<<(ostream& strm, GameEngine& gameEngine);

	shared_ptr<GameEngine> createEngine();

	bool mainGameLoop();

private:
	bool startupPhase();	//Part 2
	bool reinforcementsPhase();
	bool issueOrdersPhase();
	bool executeOrdersPhase();

};
