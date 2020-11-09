
#include "GameEngine.h"



void GameEngine::reinforcementsPhase()
{

}

void GameEngine::executeOrdersPhase()
{

}

void GameEngine::issueOrdersPhase()
{

}



int main(){

	//Declaring gameEngine
	shared_ptr<GameEngine> gameEngine(new GameEngine());

	
	//main game loop
	while (gameEngine->mainGameLoop())
	{

	}





	return 0;
}