
#include "GameEngine.h"

int main(){

	//Declaring gameEngine
	shared_ptr<GameEngine> gameEngine(new GameEngine());

	
	//main game loop
	while (gameEngine->mainGameLoop())
	{

	}





	return 0;
}