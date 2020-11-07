
#include <iostream>
#include "GameEngine.h"

//*************		PART 1		**************//



//*************		PART 2		**************//

GameEngine::GameEngine()
{
	this->players = list<shared_ptr<Player>>();
	this->map = shared_ptr<Map>();
}

void GameEngine::addPlayers(shared_ptr<Player> player)
{
	players.push_back(player);
}


list<shared_ptr<Player>> GameEngine::getPlayers()
{
	return players;
}

string GameEngine::getPlayerNames()
{
	string str = "";
	for (auto player : players)
		str += player->getName();
}

void GameEngine::startupPhase()//Part 2
{
	//1. The order of the players is determined randomly
	random_shuffle(players.begin(), players.end());

	//2. All territories in the map are randomly assigned to players one by one in round-robin fashion
	
	
	//3. Players are given a number of initial armies
}



int main(){
	////Declaring gameEngine
	//shared_ptr<GameEngine> gameEngine(new GameEngine());


	//shared_ptr<Player> p1 = shared_ptr<Player>();
	//cout << p1 << endl;

	////shared_ptr<Player> player2 = new Player("Sigmund");
	/////*players = new list <Player*>();*/
	////players->addPlayers(player1);



	//
	////main game loop
	////while (gameEngine->mainGameLoop())
	////{

	////}

	cout << "Hello" << endl;



	return 0;
}