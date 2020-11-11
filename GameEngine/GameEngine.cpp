
#include <iostream>
#include "GameEngine.h"

//*************		PART 1		**************//

GameEngine::GameEngine()
{
	this->players = vector<shared_ptr<Player>>();
	this->map = shared_ptr<Map>();
}

void GameEngine::addPlayers(shared_ptr<Player> player)
{
	players.push_back(player);
}

vector<shared_ptr<Player>> GameEngine::getPlayers()
{
	return players;
}

string GameEngine::getPlayersInfo()
{
	string str = "";
	for (auto player : players)
	{
		str += player->to_string();
	}
	return str;
}

string GameEngine::getPlayersNames()
{
	string str = "";
	for (auto player : players)
	{
		str += player->getName();
		str += "\n";
	}
	return str;
}

void GameEngine::setMap(shared_ptr<Map> newName)
{
	*map = *newName;
}

void GameEngine::loadMap()
{
	shared_ptr<MapLoader> mapLoader(new MapLoader());
	vector <shared_ptr<Map>> listOfMaps;

	while (true)
	{
		string userInput;
		cout << "Please enter name of the map (with .map or .txt) you would like to load and hit enter." << endl
			<< "If you are done selecting maps, enter 1\n" << endl;
		cin >> userInput;
		
		if (userInput == "1") {
			break;
		}

		map = mapLoader->createMap(userInput);
		break;

		//if (userInput == "1") {
		//	break;
		//}
		//else
		//{
		//	shared_ptr<Map> loadedMap = mapLoader->createMap(userInput);
		//	if (loadedMap != nullptr)
		//	{
		//		//listOfMaps.push_back(loadedMap);
		//	}
		//}
	}

	//if (listOfMaps.size() > 0)
	//{
	//	cout << "\nPrinting Maps:\n" << endl;
	//	for (int i = 0; i < listOfMaps.size(); i++)
	//	{
	//		cout << listOfMaps[i]->to_string() << "\n" << endl;
	//	}
	//}
	//mapLoader.reset();
	//mapLoader = nullptr;
}


//*************		PART 2		**************//

void GameEngine::startupPhase()
{
	//1. The order of the players is determined randomly
	random_shuffle(players.begin(), players.end());

	for (int i = 0; i<players.size(); i++)
	{
		players.at(i)->setPlayerID(i+1);
	}
	
	//2. All territories in the map are randomly assigned to players one by one in round-robin fashion
	//loop through map(territory ids) and players, assign territories in round robin to player : players > player->addTerritory(t)
	for (int i = 1; i != map->getTerritoriesSize() ; i++)
	{
		//Circularly loop through players and assign territories in ascending order to each player
		players.at((i-1)% players.size())->addTerritory(map->getTerritory(i));
	}
	
	//3. Players are given a number of initial armies, 2 players = 40, 3 Player = 35, 4 players = 30, 5 players = 25	
	int armies = 50 - (5 * Player::getPlayerCount());
	for (auto player : players)
		player->setArmies(armies);
}	

//*************		PART 3		**************//

void GameEngine::reinforcementsPhase()
{
	for (auto player : players)
	{
		int armiesToGive = player->getTerritoryList().size()/3;

		armiesToGive += findContinentBonusTotal(player);

		if (armiesToGive < 3) armiesToGive = 3;
		cout << "Gave " << armiesToGive << " armies to Player " << player->getName() << endl;
		player->addArmies(armiesToGive);
	}
		
}

int GameEngine::findContinentBonusTotal(shared_ptr<Player> player)
{
	int bonus = 0;

	vector<int> playerTerritoryIDs;
	for (auto territory : player->getTerritoryList()) playerTerritoryIDs.push_back(territory->getID()); //Get all the territory IDs owned by player
	
	sort(playerTerritoryIDs.begin(), playerTerritoryIDs.end()); // sort for std::includes

	for (auto continent : map->getContinents()) //Loop through all continents, could be simplified to only necessary continents but not necessary and complicates code
	{
		sort(continent->territoryIDs.begin(), continent->territoryIDs.end());

		if (includes(playerTerritoryIDs.begin(), playerTerritoryIDs.end(), continent->territoryIDs.begin(), continent->territoryIDs.end())) //Check if continent's territory IDs list is a subset of Player's territory IDs list
		{
			bonus += continent->bonus;
		}
	}

	return bonus;
}

void GameEngine::executeOrdersPhase()
{

}

void GameEngine::issueOrdersPhase()
{

}

void GameEngine::checkForEliminatedPlayers()
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->getTerritoryList().size() == 0)
		{
			players.erase(players.begin() + i); //Removing a player shifts the vector, thus invalidating the interator. Therefore use recursion to perform check again.
			checkForEliminatedPlayers();
			break;
		}
	}
}

shared_ptr<Player> GameEngine::checkForWinner()
{
	int numberOfTerritories = map->getTerritoriesSize();

	for (auto player : players)
	{
		if (player->getTerritoryList().size() == numberOfTerritories)
			return player;
	}
	
	return nullptr;

}

void GameEngine::mainGameLoop()
{
	reinforcementsPhase();
	executeOrdersPhase();
	issueOrdersPhase();

	//check for dead player

}


int main(){
	//Declaring gameEngine
	shared_ptr<GameEngine> gameEngine(new GameEngine());

	shared_ptr<Player> p1 = shared_ptr<Player>(new Player("Anna"));
	shared_ptr<Player> p2 = shared_ptr<Player>(new Player("Ben"));
	shared_ptr<Player> p3 = shared_ptr<Player>(new Player("Chris"));
	shared_ptr<Player> p4 = shared_ptr<Player>(new Player("Debby"));
	shared_ptr<Player> p5 = shared_ptr<Player>(new Player("Fynn"));

	gameEngine->addPlayers(p1);
	gameEngine->addPlayers(p2);
	gameEngine->addPlayers(p3);
	gameEngine->addPlayers(p4);
	gameEngine->addPlayers(p5);

	cout << "Players before shuffle: \n" << gameEngine->getPlayersNames();

	gameEngine->loadMap();


	gameEngine->startupPhase();
	cout << "\nPlayers in random order: \n" << gameEngine->getPlayersInfo();

	//Main Game Loop
	while (true)
	{
		gameEngine->mainGameLoop();
		
		shared_ptr<Player> winner = gameEngine->checkForWinner();
		if (winner != nullptr)
		{
			cout << winner->getName() << " is the Winner!" << endl;
			break;
		}

	}





	return 0;
}