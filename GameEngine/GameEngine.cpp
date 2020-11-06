
#include "GameEngine.h"

//*************		PART 1		**************//



//*************		PART 2		**************//

void addPlayers(Player* player)
{
	this->players->push_back(player);
}

list <Player*> getPlayers()
{

}


void startupPhase(Map* map, list<Player*> players)//Part 2
{
	//1. The order of the players is determined randomly
	players.random_shuffle();

	//2. All territories in the map are randomly assigned to players one by one in round-robin fashion
	//3. Players are given a number of initial armies
}


int main(){
	//Declaring gameEngine
	shared_ptr<GameEngine> gameEngine(new GameEngine());


	Player* player1 = new Player("Bettina");
	Player* player2 = new Player("Sigmund");
	players = new list <Player*>();
	players->addPlayers(player1);



	
	//main game loop
	//while (gameEngine->mainGameLoop())
	//{

	//}





	return 0;
}