#include "GameEngine.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>


//Default constructor

GameEngine::GameEngine() : Subject()
{
	numOfPlayers = 0;
	this->deck = new Deck();
	this->phaseObserver = false;
	this->mapObserver = false;
	this->players = vector<shared_ptr<Player>>();
	this->map = shared_ptr <Map>();
}


//*************		PART I		**************//

void GameEngine::gameStart()
{
	//Part I: 1
	//Loading the map
	loadMap();

	if (map->getTerritoriesCount() < Player::getPlayerCount())
	{
		cerr << "Not enough territories for all players. Quitting game..." << endl;
		exit(1);
	}

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

	//Ask for player strategies
	cout << "Please select a player stategy for each player by selecting either 1, 2 or 3: [1] Human, [2] Aggressive, [3] Benevolent." << endl;
	for (int i = 0; i < numOfPlayers; i++)
	{
		while (true)
		{
			int strategy;	//User input to decide 1 for Human, 2 for Aggressive, 3 for Benevolent
			cout << "Player strategy for player #" << (i + 1) << " with name " << players[i]->getName() << ": ";

			while (!(cin >> strategy))	//while cin is not a numerical value
			{
				cout << "Only numerical values are accepted. Please select a number between 1 and 3: ";
				cin.clear();
				cin.ignore(123, '\n');
			}
			//If numerical value was entered, verify it was between 1 - 3 and set strategy accordingly
			if (strategy == 1)
			{
				players[i]->setPlayerStrategy(new HumanPlayerStrategy());
				break;
			}
			else if (strategy == 2)
			{
				players[i]->setPlayerStrategy(new AggressivePlayerStrategy());
				break;
			}
			else if (strategy == 3)
			{
				players[i]->setPlayerStrategy(new BenevolentPlayerStrategy());
				break;
			}
			else
				cout << "The strategy number you have entered has been deemed invalid. ";
		}
	}

	//initialize a card deck with amount of players
	deck->initializeDeck(numOfPlayers);

	//Part I: 3
	//Turning on/off observers
	Observers();
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
	else if (isMapInDirectory(map + ".txt"))
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

void GameEngine::Observers()
{
	string answer;
	cout << "activate phase observer for this game? (Yes or No): ";
	cin >> answer;
	while (!equals(answer, "yes") && !equals(answer, "no")) {
		cout << "Your answer has been deemed invalid. Please enter again: ";
		cin >> answer;
	}
	if (equals(answer, "yes"))
	{
		phaseObserver = true;
	}
	else if (equals(answer, "no"))
	{
		phaseObserver = false;
	}

	answer = "";
	cout << "activate map observer for this game? (Yes or No): ";
	cin >> answer;
	while (!equals(answer, "yes") && !equals(answer, "no")) {
		cout << "Your answer has been deemed invalid. Please enter again: ";
		cin >> answer;
	}
	if (equals(answer, "yes"))
	{
		mapObserver = true;
	}
	else if (equals(answer, "no"))
	{
		mapObserver = false;
	}
}

bool GameEngine::getObserverStatus(int o)
{
	if (o == 1)
	{
		return phaseObserver;
	}
	else if (o == 2)
	{
		return mapObserver;
	}
}

void GameEngine::setObserverStatus(int o, bool status)
{
	if (o == 1)
	{
		phaseObserver = status;
	}
	else if (o == 2)
	{
		mapObserver = status;
	}
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

shared_ptr<Player> GameEngine::getPlayer(unsigned int id)
{
	for (shared_ptr<Player> p : players)
	{
		if (p->getPlayerID() == id)
		{
			return p;
		}
	}

	return nullptr;
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
		str += "Player ";
		str += std::to_string(player->getPlayerID());
		str += " ";
		str += player->getName();
		str += "\n";
	}
	return str;
}

shared_ptr<Map> GameEngine::getMap()
{
	return shared_ptr<Map>(map);
}

//*************		PART II		**************//

void GameEngine::startupPhase()
{
	//Part II: 1
	//The order of the players is determined randomly
	random_shuffle(players.begin(), players.end());

	//Reassign players id to match the new order
	for (int i = 0; i < players.size(); i++)
	{
		players.at(i)->setPlayerID(i + 1);
	}

	//Part II: 2
	//All territories in the map are randomly assigned to players one by one in round-robin fashion
	for (int i = 1; i != map->getTerritoriesCount(); i++)
	{
		//Circularly loop through players and assign territories in ascending order to each player
		players.at((i - 1) % players.size())->addTerritory(map->getTerritory(i));
	}

	//Part II: 3
	//Players are given a number of initial armies, 2 players = 40, 3 Player = 35, 4 players = 30, 5 players = 25
	int armies = 50 - (5 * players.size());
	for (auto player : players) {
		player->setArmies(armies);
	}


}

//*************		PART 3		**************//

//Give armies to each player and a card if they captured a territory in the last turn
void GameEngine::reinforcementsPhase()
{
	if (this->phaseObserver)
	{
		this->notify("reinforcement_phase_start");
	}
	for (auto player : players)
	{
		list<shared_ptr<Territory>> playerTerritories = *player->getTerritoryList();
		int baseArmiesGiven = playerTerritories.size() / 3;
		int continentBonus = findContinentBonusTotal(player);
		int armiesToGive = continentBonus +  baseArmiesGiven;

		if (armiesToGive < 3) armiesToGive = 3;

		if (this->phaseObserver)
		{
			string message = "give_armies";
			string armies = std::to_string(armiesToGive);
			string id = std::to_string(player->getPlayerID());
			string baseArmies = std::to_string(baseArmiesGiven);
			string continentB = std::to_string(continentBonus);
			this->notify(message + " " + armies + " " + id + " " + baseArmies + " " + continentB);
		}

		player->addArmies(armiesToGive); //Using add and not set because of the initial armies given from setup. Will always be 0 at start of a turn.

		if (*(player->getCapturedTerritory()))
		{
			player->resetCapturedTerritory();
			player->getHand()->addCard(deck->draw());
			string message = "draws_card";
			string id = std::to_string(player->getPlayerID());
			this->notify(message + " " + id);
		}
	}
}

int GameEngine::findContinentBonusTotal(shared_ptr<Player> player)
{
	int bonus = 0;
	vector<unsigned int> playerTerritoryIDs;

	for (auto territory : *player->getTerritoryList()) playerTerritoryIDs.push_back(territory->getID()); //Get all the territory IDs owned by player

	sort(playerTerritoryIDs.begin(), playerTerritoryIDs.end()); // sort for std::includes

	for (shared_ptr<Continent> continent : map->getContinents()) //Loop through all continents, could be simplified to only necessary continents but not necessary and complicates code
	{
		if (continent == nullptr) continue;

		sort(continent->territoryIDs.begin(), continent->territoryIDs.end());

		if (includes(playerTerritoryIDs.begin(), playerTerritoryIDs.end(), continent->territoryIDs.begin(), continent->territoryIDs.end())) //Check if continent's territory IDs list is a subset of Player's territory IDs list
		{
			bonus += continent->bonus;
		}
	}

	return bonus;
}

void GameEngine::issueOrdersPhase()
{
	if (this->phaseObserver)
	{
		this->notify("issue_order_start");
	}
	

	//Issuing all the rest of the orders
	for (auto player : players)
	{
		player->updateAvailableUnits();	//reset the temp display units (shows addition of subtraction of armies from deploy and advance before execution)

		player->issueOrder(this, map, deck);
	}
}

//Used to see if there is a certain order type left between all the players
//Indicates when it's time to switch to next order type for execution
bool GameEngine::atleastOneOfType(string type)
{
	for (auto player : players)
	{
		if (player->getOrderList()->hasOrderType(type)) return true;
	}
	return false;
}

void GameEngine::executeOrdersPhase()
{
	//Booleans to track current order type being executed.
	bool inDeploy = true;
	bool inAirlift = false;
	bool inBlockade = false;
	bool inDiplomacy = false;
	bool inBomb = false;
	bool inAdvance = false;
	bool done = false;

	if (this->mapObserver)
	{
		this->notify("map");
	}

	if (this->phaseObserver)
	{
		this->notify("execute_orders_phase_start");
	}

	while (!done)
	{
		for (auto player : players)
		{

			if (inDeploy)
			{
				shared_ptr<Order> order = player->getOrderList()->getOrder("Deploy");

				if (!(order == nullptr))
				{
					order->execute();
					//execute order
					
					if (this->phaseObserver)
					{
						this->notify("execute "+ order->to_string());
					}

					player->getOrderList()->remove(order);				//remove order from orderlist for that player
				}

				if (!atleastOneOfType("Deploy"))	//look through all players' orderlists for an order of this type, if none time for next order type
				{
					inDeploy = false;
					inAirlift = true;
					break;
				}
			}
			else if (inAirlift)
			{
				shared_ptr<Order> order = player->getOrderList()->getOrder("Airlift");

				if (!(order == nullptr))
				{
					order->execute();
					
					if (this->phaseObserver)
					{
						this->notify("execute " + order->to_string());
					}
					player->getOrderList()->remove(order);
				}

				if (!atleastOneOfType("Airlift"))
				{
					inAirlift = false;
					inBlockade = true;
					break;
				}
			}
			else if (inBlockade)
			{
				shared_ptr<Order> order = player->getOrderList()->getOrder("Blockade");

				if (!(order == nullptr))
				{
					order->execute();
					
					if (this->phaseObserver)
					{
						this->notify("execute " + order->to_string());
					}

					player->getOrderList()->remove(order);
				}

				if (!atleastOneOfType("Blockade"))
				{
					inBlockade = false;
					inDiplomacy = true;
					break;
				}
			}
			else if (inDiplomacy)
			{
				shared_ptr<Order> order = player->getOrderList()->getOrder("Negotiate");

				if (!(order == nullptr))
				{
					order->execute();
					
					if (this->phaseObserver)
					{
						this->notify("execute " + order->to_string());
					}

					player->getOrderList()->remove(order);
				}

				if (!atleastOneOfType("Negotiate"))
				{
					inDiplomacy = false;
					inBomb = true;
					break;
				}
			}
			else if (inBomb)
			{
				shared_ptr<Order> order = player->getOrderList()->getOrder("Bomb");

				if (!(order == nullptr))
				{
					order->execute();
					
					if (this->phaseObserver)
					{
						this->notify("execute " + order->to_string());
					}

					player->getOrderList()->remove(order);
				}

				if (!atleastOneOfType("Bomb"))
				{
					inBomb = false;
					inAdvance = true;
					break;
				}
			}
			else if (inAdvance)
			{
				shared_ptr<Order> order = player->getOrderList()->getOrder("Advance");

				if (!(order == nullptr))
				{
					order->execute();
					
					if (this->phaseObserver)
					{
						this->notify("execute " + order->to_string());
					}

					player->getOrderList()->remove(order);
				}

				if (!atleastOneOfType("Advance"))
				{
					inAdvance = false;
					done = true;
					break;
				}
			}


		}
	}

	reassignTerritories();

	if (this->mapObserver)
	{
		this->notify("map");
	}
}

void GameEngine::checkForEliminatedPlayers()
{
	for (int i = 0; i < players.size(); i++)
	{
		list<shared_ptr<Territory>> playerTerritories = *players[i]->getTerritoryList();
		if (playerTerritories.size() == 0)
		{
			cout << "\nEliminating player " << players[i]->getName() << ". Thank you for playing!" << endl;
			players.erase(players.begin() + i); //Because smart pointer, also calls destructor
			//Removing a player shifts the vector, thus invalidating the interator. Therefore use recursion to perform check again.
			checkForEliminatedPlayers();
			break;
		}
	}
}

shared_ptr<Player> GameEngine::checkForWinner()
{
	for (auto player : players)
	{
		if (players.size() == 1)
			return player;
	}
	return nullptr;
}

shared_ptr<Player> GameEngine::checkForLeader()
{
	int maxTerritories = 0;
	shared_ptr<Player> leader = nullptr;

	for (auto player : players)
	{
		if (player->getTerritoryList()->size() >= maxTerritories)
		{
			leader = player;

			maxTerritories = leader->getTerritoryList()->size();
		}
	}

	return leader;
}

void GameEngine::reassignTerritories()
{
	for (auto player : players)
	{
		player->getTerritoryList()->clear();
	}

	for (int i = 1; i < map->getTerritoriesCount(); i++)
	{
		shared_ptr<Territory> t = map->getTerritory(i);
		shared_ptr<Player> p = this->getPlayer(t->ownerID);

		if (p != nullptr)
			p->addTerritory(t);
	}
}

void GameEngine::mainGameLoop()
{
	reinforcementsPhase();
	issueOrdersPhase();
	executeOrdersPhase();
	checkForEliminatedPlayers();
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
	GameEngine* gameEngine = new GameEngine();

	cout << "Enter 1 for manual game\nEnter 2 for automatic game for demo purposes only" << endl;
	int decision = 0;
	cin >> decision;

	if (decision == 1)
	{
		//Testing Part I
		cout << "Calling gameStart(): Code from Part I" << endl;
		gameEngine->gameStart();

		StatisticsObserver* stats = new StatisticsObserver(gameEngine, gameEngine->getMap(), gameEngine->getPlayers());
		PhaseObserver* phases = new PhaseObserver(gameEngine, gameEngine->getPlayers());

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
			cout << "Start of main loop" << endl;
			gameEngine->mainGameLoop();
			cout << "end of main loop" << endl;

			//Check for winner, end game loop if true
			shared_ptr<Player> winner = gameEngine->checkForWinner();
			if (winner != nullptr)
			{
				break;
			}
		}

		delete phases;
		delete stats;
	}
	else if (decision == 2)
	{

		/*
		Logic for some points is hard to show using the driver but can clearly be seen in the manual game. For example,
		(2) a player will only issue deploy orders and no other kind of orders if they still have armies in their reinforcement pool. This
		can easily be seen above but is hard to show using just the driver.
		There is no ai either, so using the mainGameLoop() doesn't work with automatic gameplay, but works with manual gameplay as it should.
		I will be creating the orders in a way to demonstrate the requirements.
		Issue order asks for manual input, so to make it automatic I go straight to creating the order itself that I want to use to demonstrate.
		Execution will still happen corrently
		*/

		/*
		Visualization of Demo.map
		[1][2][3]
		[4][5][6]

		Because 3 players, each player has a collumn
		Border are bi directional through direct adjacency, no diagonals
		Continents are [1,4], [2], and [3,5,6]
		*/


		cout << "\nAutomatic playing for demo purposes. Orders will be hardcoded as opposed to entered by the player." << endl;
		cout << "Use map Demo.map, with 3 players to show all the functionality\n" << endl;
		Sleep(2000);

		//Testing Part I
		cout << "Calling gameStart(): Code from Part I" << endl;
		gameEngine->gameStart();

		StatisticsObserver* stats = new StatisticsObserver(gameEngine, gameEngine->getMap(), gameEngine->getPlayers());
		PhaseObserver* phases = new PhaseObserver(gameEngine, gameEngine->getPlayers());


		//A deck was created
		cout << *gameEngine->getDeck() << endl;

		//Players were created
		cout << gameEngine->getPlayersNames() << endl;

		//Testing Part II
		cout << "Calling startupPhase(): Code from Part II" << endl;
		gameEngine->startupPhase();

		//Player order was randomly changed, territories distributed and armies were assigned to each player
		cout << gameEngine->getPlayersInfo() << endl;

		gameEngine->reinforcementsPhase();

		for (auto player : gameEngine->getPlayers())
		{
			player->updateAvailableUnits();
		}

		cout << "Creating Deploy Orders" << endl;
		Sleep(2000);

		//getting the territories to make order creation easier
		shared_ptr<Territory> t1 = gameEngine->getMap()->getTerritory(1);
		shared_ptr<Territory> t2 = gameEngine->getMap()->getTerritory(2);
		shared_ptr<Territory> t3 = gameEngine->getMap()->getTerritory(3);
		shared_ptr<Territory> t4 = gameEngine->getMap()->getTerritory(4);
		shared_ptr<Territory> t5 = gameEngine->getMap()->getTerritory(5);
		shared_ptr<Territory> t6 = gameEngine->getMap()->getTerritory(6);

		//getting the players to make order creation easier
		//loop is to make sure no matter the order player 1 refers to the one that owns t1 and t4, etc...
		//only used for automatic gameplay
		shared_ptr<Player> player1;
		shared_ptr<Player> player2;
		shared_ptr<Player> player3;
		for (auto player : gameEngine->getPlayers())
		{
			if (player->getPlayerID() == 1) player1 = player;
			else if (player->getPlayerID() == 2) player2 = player;
			else if (player->getPlayerID() == 3) player3 = player;
		}

		//Deploying the armies for the players
		shared_ptr<Order> order1(new Deploy(player1->getArmies() - 10, t1, *player1->getTerritoryList()));
		player1->getOrderList()->addOrder(order1);

		shared_ptr<Order> order2(new Deploy(10, t4, *player1->getTerritoryList()));
		player1->getOrderList()->addOrder(order2);

		shared_ptr<Order> order3(new Deploy(player2->getArmies(), t2, *player2->getTerritoryList()));
		player2->getOrderList()->addOrder(order3);

		shared_ptr<Order> order4(new Deploy(player3->getArmies() - 1, t3, *player3->getTerritoryList()));
		player3->getOrderList()->addOrder(order4);

		shared_ptr<Order> order5(new Deploy(1, t6, *player3->getTerritoryList()));
		player3->getOrderList()->addOrder(order5);

		//For demonstration purposes since orders are hard coded, this would be used in the issue orders during manual gameplay
		for (auto player : gameEngine->getPlayers())
		{
			cout << "\n" << player->getName() << " can attack:" << endl;
			for (auto territory : player->toAttack(gameEngine->getMap()))
			{
				cout << territory->name << endl;
			}
			cout << "\nAnd can defend:" << endl;
			for (auto territory : player->toDefend(gameEngine->getMap()))
			{
				cout << territory->name << endl;
			}
			Sleep(2000);
		}

		//Player 1 has a huge continent bonus, he will eliminate player 2 to show eliminated players are removed and then eliminate player 3 to win.
		//Our card design can be seen in the manual gameplay, they don't issue the orders themselves but instead give the player permission to use that order
		//This may be hard to show here, so I will create a bomb order and use it on territory 2.

		//Player 1 will move into t2 from t1 and t5 from t4, eliminating player 2
		//player 2 will take no action
		//player 3 will advance some units to t6 to show that you can advance to friendly territories
		//shared_ptr<Order> order6(new Advance((player1->getArmies() - 10, t1, t2, player1->getTerritoryList(), player2->getTerritoryList(), player1->getCapturedTerritory(), playersNegotiated));

		shared_ptr<Order> order6(new Bomb(player1->getPlayerID(), t2, player1->getTerritoryList(), &playersNegotiated));
		player1->getOrderList()->addOrder(order6);

		shared_ptr<Order> order7(new Advance(player1->getArmies() - 10, t1, t2, player1->getTerritoryList(), player2->getTerritoryList(), player1->getCapturedTerritory(), &playersNegotiated));
		player1->getOrderList()->addOrder(order7);

		shared_ptr<Order> order8(new Advance(10, t4, t5, player1->getTerritoryList(), player2->getTerritoryList(), player1->getCapturedTerritory(), &playersNegotiated));
		player1->getOrderList()->addOrder(order8);

		shared_ptr<Order> order9(new Advance(5, t3, t6, player3->getTerritoryList(), player3->getTerritoryList(), player3->getCapturedTerritory(), &playersNegotiated));
		player3->getOrderList()->addOrder(order9);

		//execute the orders
		gameEngine->executeOrdersPhase();

		//This will eliminate player 2
		gameEngine->checkForEliminatedPlayers();
		Sleep(3000);
		cout << "\nStart of next turn!\n" << endl;
		Sleep(1000);

		//start of next turn ========================================================
		gameEngine->reinforcementsPhase();

		for (auto player : gameEngine->getPlayers())
		{
			player->updateAvailableUnits();
		}

		cout << "Creating deploy orders\n" << endl;

		//Will deploy player 1's units in t5 and player 3's units in t6
		shared_ptr<Order> order10(new Deploy(player1->getArmies(), t5, *player1->getTerritoryList()));
		player1->getOrderList()->addOrder(order10);

		shared_ptr<Order> order11(new Deploy(player3->getArmies(), t6, *player3->getTerritoryList()));
		player3->getOrderList()->addOrder(order11);

		for (auto player : gameEngine->getPlayers())
		{
			cout << "\n" << player->getName() << " can attack:" << endl;
			for (auto territory : player->toAttack(gameEngine->getMap()))
			{
				cout << territory->name << endl;
			}
			cout << "\nAnd can defend:" << endl;
			for (auto territory : player->toDefend(gameEngine->getMap()))
			{
				cout << territory->name << endl;
			}
			Sleep(2000);
		}

		cout << "\nCreating advance orders" << endl;
		//player 1 will advance into t3 from t2 and t6 from t5, eliminating player 3
		shared_ptr<Order> order12(new Advance(100, t2, t3, player1->getTerritoryList(), player3->getTerritoryList(), player1->getCapturedTerritory(), &playersNegotiated));
		player1->getOrderList()->addOrder(order12);

		shared_ptr<Order> order13(new Advance(200, t5, t6, player1->getTerritoryList(), player3->getTerritoryList(), player1->getCapturedTerritory(), &playersNegotiated));
		player1->getOrderList()->addOrder(order13);

		//execute the orders
		gameEngine->executeOrdersPhase();

		//This will eliminate player 3
		gameEngine->checkForEliminatedPlayers();
		Sleep(2000);

		cout << "\nEnd of Game!\n" << endl;

		delete phases;
		delete stats;
	}
	else
	{
		cout << "\nInvalid choice, ending program" << endl;
	}

	delete gameEngine;
	Sleep(5000);

	return 0;
}

GameEngine::GameEngine(const GameEngine& gameEngine)
{
	this->numOfPlayers = gameEngine.numOfPlayers;
	this->deck = gameEngine.deck;
	this->map = gameEngine.map;
	this->players = gameEngine.players;
	this->phaseObserver = gameEngine.phaseObserver;
	this->mapObserver = gameEngine.mapObserver;
}

GameEngine::~GameEngine()
{
	delete deck;
	deck = nullptr;

	map.reset();

	for (shared_ptr<Player> p : players)
	{
		p.reset();
	}
	players.clear();
}

GameEngine& GameEngine::operator=(const GameEngine& gameEngine)
{
	this->numOfPlayers = gameEngine.numOfPlayers;
	this->deck = gameEngine.deck;
	this->map = gameEngine.map;
	this->players = gameEngine.players;
	this->phaseObserver = gameEngine.phaseObserver;
	this->mapObserver = gameEngine.mapObserver;
	return *this;
}

ostream& operator<<(ostream& strm, GameEngine& gameEngine)
{
	return strm << gameEngine.to_string();
}

string GameEngine::to_string()
{
	string s = "GameEngine:";
	s += "\nWith Map:\n";
	s += map->to_string();
	s += "\nWith Players:\n";
	for (auto player : players) s += player->to_string() + "\n";
	s += "\nWith Deck:\n";
	s += deck->to_string();
	s += "\nAnd with Observers:\n";
	s += "phase observer: ";
	s += getObserverStatus(1);
	s += "\n";
	s += "map observer: ";
	s += getObserverStatus(2);
	s += "\n";
	return s;
}
