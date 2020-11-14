
#include <iostream>
#include "GameEngine.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>

#include <filesystem>

//Default constructor
GameEngine::GameEngine()
{
	numOfPlayers = 0;
	this->deck = new Deck();
	activateObservers = true;
	this->players = vector<shared_ptr<Player>>();
	this->map = shared_ptr <Map>();
}

//Destructor
GameEngine::~GameEngine()
{
	delete deck;
	deck = nullptr;
	cout << "GameEngine was deleted" << endl;
}



//*************		PART I		**************//

void GameEngine::gameStart()
{
	//Part I: 1
	//Loading the map
	loadMap();

	//Part I: 2
	//Asking for player amount, rejecting non numerical inputs and numbers out of range 2-5
	while (true)
	{
		cout << "Select the number of players that will be participating (Must be between 2 and 5): ";
		while (!(cin >> numOfPlayers))
		{
			cout << "Only numerical values are accepted. Please select a number between 2 and 5: ";
			cin.clear();
			cin.ignore(123, '\n');
		}
		if (numOfPlayers >= 2 && numOfPlayers <= 5)
			break;
		else
			cout << "The numbers of players that you've selected has been deemed invalid. ";
	}
	
	//Ask for player names, create players and push into players vector list
	for (int i = 0; i < numOfPlayers; i++) 
	{
		string name;
		cout << "Enter the name for player #" << (i + 1) << ": ";
		cin >> name;
		shared_ptr<Player> player = shared_ptr<Player>(new Player(name));
		addPlayers(player);
	}

	//initialize a card deck with amount of players
	deck->initializeDeck(numOfPlayers);

	//Part I: 3
	//Turning on/off observers
	activateObservers = Observers();
}

//******     Map Methods    *****//

//Method to load map and store in map attribute of class GameEngine
void GameEngine::loadMap()
{
	//declaring map loader 
	shared_ptr<MapLoader> mapLoader(new MapLoader());

	//implement vector of maps, player can choose one
	vector <shared_ptr<Map>> listOfMaps;

	//List of Map names
	vector <string> mapNames;

	//Ask user for map names as input to store in map list to later select from
	while (true)
	{
		string userInput;
		cout << "Please enter name of the map (with .map or .txt) you would like to load and hit enter." << endl
			<< "If you are done selecting maps, enter 1\n" << endl;
		cin >> userInput;
		if (userInput == "1")
		{
			if (listOfMaps.size() != 0)
				break;
			else
				cout << "Cannot resume before at least 1 map was entered." << endl;
		}
		else
		{
			shared_ptr<Map> loadedMap = mapLoader->createMap(userInput);
			if (loadedMap != nullptr)
			{
				listOfMaps.push_back(loadedMap);
				mapNames.push_back(userInput);
			}
		}
	}

	//Iterating through list of vaild maps entered by the user
	if (listOfMaps.size() > 0)
	{
		//Printing maps in list as well as their names
		cout << "\nPrinting Maps:\n" << endl;
		for (int i = 0; i < listOfMaps.size(); i++)
		{
			cout << "Map " << i+1 << " " << mapNames[i] << ": \n" << listOfMaps[i]->to_string() << "\n" << endl;
		}
		
		//Loop to get user to pick a map from the previous printed maps. 
		//Reject input if its not numerical, smaller or greater than the list of maps size 
		while (true)
		{
			cout << "Please select the Map you would like to proceed with by entering its number : ";
			
			int mapNum;
			while (!(cin >> mapNum))
			{
				cout << "Invalid input, only numbers allowed. Please select the Map you would like to proceed with by entering its number: ";
				cin.clear();
				cin.ignore(123, '\n');
			}
			if (mapNum <= 0 || mapNum > listOfMaps.size())	
			{
				cout << "You entered Map number that does not exist. Please try again." << endl;
			}

			//If input is valid, create map and break out of loop
			else
			{
				map = listOfMaps[mapNum - 1];
				break;
			}
			//if value entered was not valid, clear user input and loop again
			cin.clear();
		}
	}

	//Resetting MapLoader
	mapLoader.reset();
	mapLoader = nullptr;
}

string GameEngine::selectMap() 
{
	string map;
	cout << "What map would you like to play with ?: ";
	cin >> map;
	if (isMapInDirectory(map + ".map"))
		return map + ".map";
	else if(isMapInDirectory(map + ".txt"))
		return map + ".txt";
	else
		return "NO MAP FOUND";
}

bool GameEngine::isMapInDirectory(string fileName) 
{
	ifstream file("MapDirectory/" + fileName);
	if (!file)
		return false;
	else
		return true;
}

//******     Oberserver Methods    *****//

bool GameEngine::Observers()
{
	string answer;
	bool loopAgain;
	cout << "activate the observers for this game? (Yes or No): ";
	cin >> answer;
	while (!equals(answer, "yes") && !equals(answer, "no")) {
		cout << "Your answer has been deemed invalid. Please enter again: ";
		cin >> answer;
	}
	if (equals(answer, "yes"))
		return true;
	else if (equals(answer, "no"))
		return false;
	return false;
}

bool GameEngine::getObserverStatus() 
{
	return activateObservers;
}

void GameEngine::setObserverStatus(bool status) 
{
	activateObservers = status;
}

bool GameEngine::equals(const string& a, const string& b) {
	unsigned int size = a.size();
	if (b.size() != size)
		return false;
	for (unsigned int i = 0; i < size; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

//******     Deck Method    *****//

//Return Deck of cards
Deck* GameEngine::getDeck()
{
	return deck;
}

//******     Player Methods    *****//

//Adding players to vector list containing all players
void GameEngine::addPlayers(shared_ptr<Player> player)
{
	players.push_back(player);
}

//Return player vector list

vector<shared_ptr<Player>> GameEngine::getPlayers()
{
	return players;
}

//Return players stored in players vector list as string
string GameEngine::getPlayersInfo()
{
	string str = "";
	for (auto player : players)
	{
		str += player->to_string();
	}
	return str;
}

//Returns string with player names stored in players vector list
string GameEngine::getPlayersNames()
{
	string str = "";
	for (auto player : players)
	{
		str += "Player " ;
		str += to_string(player->getPlayerID());
		str += " ";
		str += player->getName();
		str += "\n";
	}
	return str;
}

//*************		PART II		**************//


void GameEngine::startupPhase()
{
	//Part II: 1
	//The order of the players is determined randomly
	random_shuffle(players.begin(), players.end());

	//Reassign players id to match the new order
	for (int i = 0; i<players.size(); i++)
	{
		players.at(i)->setPlayerID(i+1);
	}
	
	//Part II: 2
	//All territories in the map are randomly assigned to players one by one in round-robin fashion
	for (int i = 1; i != map->getTerritoriesCount() ; i++)
	{
		//Circularly loop through players and assign territories in ascending order to each player
		players.at((i-1)% players.size())->addTerritory(map->getTerritory(i));
	}
	
	//Part II: 3
	//Players are given a number of initial armies, 2 players = 40, 3 Player = 35, 4 players = 30, 5 players = 25	
	int armies = 50 - (5 * players.size());
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
	int numberOfTerritories = map->getTerritoriesCount();

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

	checkForEliminatedPlayers();

}


//*************		MAIN METHOD		**************//

int main(){
	//Declaring gameEngine
	shared_ptr<GameEngine> gameEngine(new GameEngine());

	//Testing Part I
	cout << "Calling gameStart(): Code from Part I" << endl;
	gameEngine->gameStart();

	//A deck was created
	cout << *gameEngine->getDeck() << endl;
	
	//Players were created
	cout << gameEngine->getPlayersNames() << endl;
		
	//Testing Part II
	cout << "Calling startupPhase(): Code from Part II" << endl;
	gameEngine->startupPhase();
	
	//Player order was randomly changed, territories distributed and armies were assigned to each player
	cout << gameEngine->getPlayersInfo() << endl;


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