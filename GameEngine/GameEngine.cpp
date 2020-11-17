#include "GameEngine.h"
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
	vector <shared_ptr<Map>> loadedMaps;
	vector <string> loadedMapNames;

	//List of Map names
	vector <string> mapNames;

	//Ask user for map names as input to store in map list to later select from
	while (true)
	{
		// List available maps
		mapNames = findMapNames();

		cout << "Here are the available maps:" << endl;
		for (string s : mapNames) {
			cout << s << endl;
		}

		string userInput;
		cout << "Please enter name of the map (with .map or .txt) you would like to load and hit enter to validate it." << endl
			<< "If you are done selecting maps, enter 1\n" << endl;
		cin >> userInput;
		if (userInput == "1")
		{
			if (loadedMaps.size() != 0)
				break;
			else
				cout << "Cannot resume before at least 1 map was entered." << endl;
		}
		else
		{
			shared_ptr<Map> loadedMap = mapLoader->createMap(userInput, "MapDirectory/");
			if (loadedMap != nullptr)
			{
				loadedMaps.push_back(loadedMap);
				loadedMapNames.push_back(userInput);
			}
		}
	}

	//Iterating through list of vaild maps entered by the user
	if (loadedMaps.size() > 0)
	{
		//Printing maps in list as well as their names
		cout << "\nPrinting Maps:\n" << endl;
		for (int i = 0; i < loadedMaps.size(); i++)
		{
			cout << "Map " << i+1 << " " << loadedMapNames[i] << ": \n" << loadedMaps[i]->to_string() << "\n" << endl;
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
			if (mapNum <= 0 || mapNum > loadedMaps.size())	
			{
				cout << "You entered Map number that does not exist. Please try again." << endl;
			}

			//If input is valid, create map and break out of loop
			else
			{
				map = loadedMaps[mapNum - 1];
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

vector<string> GameEngine::findMapNames() {
	vector<string> mapNames = vector<string>();

	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath[] = "MapDirectory\\*.map";

	if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("Error loading map files. Make sure there is a MapDirectory folder.");
		exit(1);
	}

	do
	{
		//Find first file will always return "."
		//    and ".." as the first two directories.
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			mapNames.push_back(fdFile.cFileName);

		}
	} while (FindNextFile(hFind, &fdFile)); //Find the next file.

	FindClose(hFind); //Always, Always, clean things up!

	return mapNames;
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

	return 0;
}
