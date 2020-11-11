
#include <iostream>
#include "GameEngine.h"

#include <filesystem>


//*************		PART 1		**************//

GameEngine::GameEngine()
{
	this->players = vector<shared_ptr<Player>>();
	this->map = shared_ptr<Map>();
}

bool GameEngine::equals(const string& a, const string& b) {
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}


void GameEngine::gameStart()
{
	loadMap();

	setNumOfPlayers();

	activateObservers = Observers();

	std::cout << endl;
	for (int i = 0; i < numOfPlayers; i++) {
		string name;
		std::cout << "Enter the name for player #" << (i + 1) << ": ";
		cin >> name;
		shared_ptr<Player> player = shared_ptr<Player>(new Player(name));
		addPlayers(player);
	}
	std::cout << endl;

	//initialize a card deck
	std::cout << "<Initial Card Deck Status>" << endl;
	Deck deck;
	deck.initializeDeck(getNumOfPlayers());
	std::cout << deck << endl;

}



void GameEngine::setNumOfPlayers()
{
	int count = 0;
	cout << "Select the number of players that will be participation (Must be between 2 and 5): ";
	cin >> count;
	while (!(count >= 2 && count <= 5)) {
		cout << "The numbers of players that you've selected has been deemed invalid. Please pick again: ";
		cin >> count;
	}
	numOfPlayers = count;
}

bool GameEngine::Observers() {
	string answer;
	bool loopAgain;
	cout << "activate the observers for this game? (Yes or No): ";
	cin >> answer;
	while (!equals(answer, "yes") && !equals(answer, "no")) {
		cout << "Your answer has been deeemd invalid. Please enter again: ";
		cin >> answer;
	}
	if (equals(answer, "yes"))
		return true;
	else if (equals(answer, "no"))
		return false;
	return false;
}

int GameEngine::getNumOfPlayers()
{
	return numOfPlayers;
}

bool GameEngine::getObserverStatus() {
	return activateObservers;
}

void GameEngine::setObserverStatus(bool status) {
	activateObservers = status;
}

Deck GameEngine::getDeckCards() {
	return deckCards;
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
	//declaring map loader 
	shared_ptr<MapLoader> mapLoader(new MapLoader());

	//implement vector of maps, player can choose one
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
		else
		{
			shared_ptr<Map> loadedMap = mapLoader->createMap(userInput);
			if (loadedMap != nullptr)
			{
				listOfMaps.push_back(loadedMap);
			}
		}
	}

	if (listOfMaps.size() > 0)
	{
		cout << "\nPrinting Maps:\n" << endl;
		for (int i = 0; i < listOfMaps.size(); i++)
		{
			cout << listOfMaps[i]->to_string() << "\n" << endl;
		}
	}

	mapLoader.reset();
	mapLoader = nullptr;
}

int main() {
	////Declaring gameEngine
	shared_ptr<GameEngine> gameEngine(new GameEngine());

	gameEngine->gameStart();

	cout << "There are " << gameEngine->getNumOfPlayers() << " Players in this game of Warzone" << endl;
	cout << "Their names are: \n" << gameEngine->getPlayersNames() << endl;
	cout << gameEngine->getPlayersInfo() << endl;


	////Load Map
	//gameEngine->loadMap();

	////2. 
	//gameEngine->startupPhase();
	//std::cout << "\nPlayers in random order: \n" << gameEngine->getPlayersInfo();


	return 0;
}