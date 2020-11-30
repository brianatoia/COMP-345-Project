#include "../PlayerStrategies/PlayerStrategies.h"

PlayerStrategy::PlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList, StrategyType* strategyType)
{
	this->playerID = playerID;
	this->hand = hand;
	this->orderList = orderList;
	if (strategyType == nullptr)
	{
		this->strategyType = new StrategyType(NONE);
	}
	else
	{
		this->strategyType = strategyType;
	}
}

PlayerStrategy::~PlayerStrategy()
{
	delete this->strategyType;
}

StrategyType PlayerStrategy::getStrategyType()
{
	return *this->strategyType;
}

list<shared_ptr<Territory>> PlayerStrategy::toDefend(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	list<shared_ptr<Territory>> copyList;
	list<shared_ptr<Territory>>::iterator i = territoryList.begin();


	for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
	{
		vector <unsigned int> territoryIDs = (*i)->borders;
		for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
		{
			shared_ptr<Territory> t = map->getTerritory(*iD);

			if (t->ownerID == playerID) //if adjacent territory is owned by the player, add
			{
				copyList.push_back(t);
			}
		}
	}

	copyList.sort();
	copyList.unique();

	return copyList;
}

list<shared_ptr<Territory>> PlayerStrategy::toAttack(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	list<shared_ptr<Territory>> copyList;
	list<shared_ptr<Territory>>::iterator i = territoryList.begin();


	for (i = territoryList.begin(); i != territoryList.end(); advance(i, 1))
	{
		vector <unsigned int> territoryIDs = (*i)->borders; //loop through adjacent territories
		for (auto iD = territoryIDs.begin(); iD != territoryIDs.end(); iD++)
		{
			shared_ptr<Territory> t = map->getTerritory(*iD);

			if (t->ownerID != playerID) //if adjacent territory is not owned by player, add
			{
				copyList.push_back(t);
			}
		}
	}

	copyList.sort();
	copyList.unique();

	return copyList;
}

void PlayerStrategy::createOrder(string orderType, shared_ptr<Map> map, int armies, list<shared_ptr<Territory>> territoryList)
{
	if (orderType == "Deploy")
	{
		cout << name << " deploy your armies!" << endl;

		auto deployInfo = this->deploy(armies, map, territoryList);

		shared_ptr<Territory> territory = deployInfo.first;
		int numberOfArmiesToDeploy = deployInfo.second;

		territory->availableUnits += numberOfArmiesToDeploy;
		armies -= numberOfArmiesToDeploy;

		shared_ptr<Order> order(new Deploy(numberOfArmiesToDeploy, territory, territoryList));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Advance")
	{
		string sourceTerritoryName;
		string targetTerritoryName;
		shared_ptr<Territory> sourceTerritory = nullptr;
		shared_ptr<Territory> targetTerritory = nullptr;
		list<shared_ptr<Territory>> playerTerritoryOptions;
		int numOfArmies = 0;
		bool territoryAllowed = false;

		do {
			cout << "Current territories you can advance from: " << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : territoryList)
			{
				if (t->availableUnits > 0)
				{
					playerTerritoryOptions.push_back(t);
					cout << t->name << " (" << t->availableUnits << ")" << endl;
				}
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nEnter territory you would like to advance from: " << endl;
			cin.clear();
			cin.ignore(123, '\n');
			cin >> sourceTerritoryName;
			sourceTerritory = map->findTerritory(sourceTerritoryName);

			for (shared_ptr<Territory> t : playerTerritoryOptions)
			{
				if (t == sourceTerritory)
				{
					territoryAllowed = true;
				}
			}

		} while (!territoryAllowed);

		territoryAllowed = false;
		do {
			cout << "Current territories you can move to: " << endl;
			cout << "--------------------------------------------" << endl;
			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory->ownerID == sourceTerritory->ownerID)
				{
					cout << territory->name << " (" << territory->availableUnits << ")" << endl;
				}
			}
			cout << "--------------------------------------------" << endl;

			cout << "Current territories you can attack: " << endl;
			cout << "--------------------------------------------" << endl;
			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory->ownerID != sourceTerritory->ownerID)
				{
					cout << territory->name << " (" << territory->units << ")" << endl;
				}
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nEnter territory you would like to advance to: " << endl;
			cin >> targetTerritoryName;
			targetTerritory = map->findTerritory(targetTerritoryName);

			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory == targetTerritory)
				{
					territoryAllowed = true;
				}
			}
		} while (!territoryAllowed);

		cout << "Enter number of armies to advance. Should be > 0 and <= " << sourceTerritory->availableUnits << endl;
		while (true)
		{
			cin >> numOfArmies;
			if (numOfArmies < 0 || sourceTerritory->availableUnits < numOfArmies || cin.fail())
			{
				cout << "\nInvalid input, please try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
			}
			else break;
		}

		sourceTerritory->availableUnits -= numOfArmies;



		shared_ptr<Order> order(new Advance(numOfArmies, sourceTerritory, targetTerritory, &territoryList, playerTerritories[targetTerritory->ownerID - 1], capturedTerritory, &playersNegotiated));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Bomb")
	{
		shared_ptr<Territory> territory = nullptr;
		string territoryName;
		bool territoryAllowed = false;

		do {
			cout << "\nWhich territory would you like to bomb?" << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : toAttack(map))
			{
				cout << t->name << endl;
			}
			cout << "--------------------------------------------" << endl;
			cin >> territoryName;

			territory = map->findTerritory(territoryName);

			for (shared_ptr<Territory> t : toAttack(map))
			{
				if (t == territory)
				{
					territoryAllowed = true;
				}
			}
		} while (!territoryAllowed);

		shared_ptr<Order> order(new Bomb(playerID, territory, &territoryList, &playersNegotiated));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Blockade")
	{
		shared_ptr<Territory> territory;
		string territoryName;
		bool territoryAllowed = false;

		do {
			cout << "\nCurrent territories you can blockade: " << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : territoryList)
			{
				cout << t->name << " (" << t->availableUnits << ")" << endl;
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nWhich territory would you like to blockade?" << endl;
			cin.clear();
			cin.ignore(123, '\n');
			cin >> territoryName;
			territory = map->findTerritory(territoryName);

			for (shared_ptr<Territory> t : territoryList)
			{
				if (t == territory)
				{
					territoryAllowed = true;
				}
			}

		} while (!territoryAllowed);


		shared_ptr<Order> order(new Blockade(territory, &territoryList));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Airlift")
	{
		string sourceTerritoryName;
		string targetTerritoryName;
		shared_ptr<Territory> sourceTerritory = nullptr;
		shared_ptr<Territory> targetTerritory = nullptr;
		list<shared_ptr<Territory>> playerTerritoryOptions;
		int numOfArmies = 0;
		bool territoryAllowed = false;
		int numOfTargets = 0;

		do {
			cout << "Current territories you can airlift from: " << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : territoryList)
			{
				if (t->availableUnits > 0)
				{
					playerTerritoryOptions.push_back(t);
					cout << t->name << " (" << t->availableUnits << ")" << endl;
				}
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nEnter territory you would like to airlift from: " << endl;
			cin.clear();
			cin.ignore(123, '\n');
			cin >> sourceTerritoryName;
			sourceTerritory = map->findTerritory(sourceTerritoryName);

			for (shared_ptr<Territory> t : playerTerritoryOptions)
			{
				if (t == sourceTerritory)
				{
					territoryAllowed = true;
				}
			}

		} while (!territoryAllowed);

		territoryAllowed = false;
		do {
			cout << "Current territories you can move to: " << endl;
			cout << "--------------------------------------------" << endl;
			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory->ownerID == sourceTerritory->ownerID)
				{
					cout << territory->name << " (" << territory->availableUnits << ")" << endl;
					numOfTargets++;
				}
			}
			if (numOfTargets == 0)
			{
				cout << "No available territories to move to. Cancelling Airlift." << endl;
				return;
			}
			cout << "--------------------------------------------" << endl;

			cout << "\nEnter territory you would like to airlift to: " << endl;
			cin.clear();
			cin.ignore(123, '\n');
			cin >> targetTerritoryName;
			targetTerritory = map->findTerritory(targetTerritoryName);

			for (int border : sourceTerritory->borders)
			{
				shared_ptr<Territory> territory = map->getTerritory(border);
				if (territory == targetTerritory)
				{
					territoryAllowed = true;
				}
			}
		} while (!territoryAllowed);

		do
		{
			cout << "Enter number of armies to airlift. Should be <= " + std::to_string(sourceTerritory->units) + " and >= 0: ";
			cin.clear();
			cin.ignore(123, '\n');
			cin >> numOfArmies;
		} while (numOfArmies >= sourceTerritory->units && numOfArmies < 0);

		targetTerritory->availableUnits += numOfArmies;

		shared_ptr<Order> order(new Airlift(numOfArmies, sourceTerritory, targetTerritory, &territoryList, playerTerritories[targetTerritory->ownerID - 1], capturedTerritory, &playersNegotiated));
		this->orderList->addOrder(order);
	}
	else if (orderType == "Negotiate")
	{
		int targetPlayerID;
		do
		{
			cout << "Enter the ID of the player you would like to negotiate with: ";
			cin.clear();
			cin.ignore(123, '\n');
			cin >> targetPlayerID;
		} while (targetPlayerID > playerCount || targetPlayerID < 0);


		shared_ptr<Order> order(new Negotiate(playerID, targetPlayerID, &playersNegotiated));
		this->orderList->addOrder(order);
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}

HumanPlayerStrategy::HumanPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(HUMAN)) {}

string HumanPlayerStrategy::issueOrder(shared_ptr<Map> map, Deck* deck)
{
	string decision = "";
	cin >> decision;
	return decision;
}

pair<shared_ptr<Territory>, int> HumanPlayerStrategy::deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	shared_ptr<Territory> territory;
	string territoryName;

	bool territoryAllowed = false;

	do {
		cout << "You have " << armies << " deployable armies" << endl;
		cout << "\nCurrent territories you can deploy to: " << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << " (" << t->availableUnits << ")" << endl;
		}
		cout << "--------------------------------------------" << endl;
		cout << "\nWhich territory would you like to deploy to?" << endl;
		cin.clear();
		cin.ignore(123, '\n');
		cin >> territoryName;
		territory = map->findTerritory(territoryName);

		for (shared_ptr<Territory> t : territoryList)
		{
			if (t == territory)
			{
				territoryAllowed = true;
			}
		}

	} while (!territoryAllowed);

	cout << "\nHow many armies would you like to deploy? (Max " << armies << ")" << endl;
	cout << "--------------------------------------------" << endl;
	int numberOfArmiesToDeploy;
	while (true)
	{
		cin >> numberOfArmiesToDeploy;
		if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > armies || cin.fail())
		{
			cout << "\nInvalid input, please try again" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else break;
	}

	return pair<shared_ptr<Territory>, int>(territory, numberOfArmiesToDeploy);
}

//*********  AggressivePlayerStrategy  **********//
AggressivePlayerStrategy::AggressivePlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(AGGRESSIVE)) {}

string AggressivePlayerStrategy::issueOrder(shared_ptr<Map> map, Deck* deck)
{
	return "Finish";
}

pair<shared_ptr<Territory>, int> AggressivePlayerStrategy::deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	shared_ptr<Territory> territory;
	string territoryName;

	bool territoryAllowed = false;

	do {
		cout << "You have " << armies << " deployable armies" << endl;
		cout << "\nCurrent territories you can deploy to: " << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << " (" << t->availableUnits << ")" << endl;
		}
		cout << "--------------------------------------------" << endl;
		cout << "\nWhich territory would you like to deploy to?" << endl;
		cin.clear();
		cin.ignore(123, '\n');
		cin >> territoryName;
		territory = map->findTerritory(territoryName);

		for (shared_ptr<Territory> t : territoryList)
		{
			if (t == territory)
			{
				territoryAllowed = true;
			}
		}

	} while (!territoryAllowed);

	cout << "\nHow many armies would you like to deploy? (Max " << armies << ")" << endl;
	cout << "--------------------------------------------" << endl;
	int numberOfArmiesToDeploy;
	while (true)
	{
		cin >> numberOfArmiesToDeploy;
		if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > armies || cin.fail())
		{
			cout << "\nInvalid input, please try again" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else break;
	}

	return pair<shared_ptr<Territory>, int>(territory, numberOfArmiesToDeploy);
}

//********* BenevolentPlayerStrategy  **********//
BenevolentPlayerStrategy::BenevolentPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(BENEVOLENT)) {}

string BenevolentPlayerStrategy::issueOrder(shared_ptr<Map> map, Deck* deck)
{
	return "Finish";
}

pair<shared_ptr<Territory>, int> BenevolentPlayerStrategy::deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	shared_ptr<Territory> territory;
	string territoryName;

	bool territoryAllowed = false;

	do {
		cout << "You have " << armies << " deployable armies" << endl;
		cout << "\nCurrent territories you can deploy to: " << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << " (" << t->availableUnits << ")" << endl;
		}
		cout << "--------------------------------------------" << endl;
		cout << "\nWhich territory would you like to deploy to?" << endl;
		cin.clear();
		cin.ignore(123, '\n');
		cin >> territoryName;
		territory = map->findTerritory(territoryName);

		for (shared_ptr<Territory> t : territoryList)
		{
			if (t == territory)
			{
				territoryAllowed = true;
			}
		}

	} while (!territoryAllowed);

	cout << "\nHow many armies would you like to deploy? (Max " << armies << ")" << endl;
	cout << "--------------------------------------------" << endl;
	int numberOfArmiesToDeploy;
	while (true)
	{
		cin >> numberOfArmiesToDeploy;
		if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > armies || cin.fail())
		{
			cout << "\nInvalid input, please try again" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else break;
	}

	return pair<shared_ptr<Territory>, int>(territory, numberOfArmiesToDeploy);
}

//********* NeutralPlayerStrategy  **********//
NeutralPlayerStrategy::NeutralPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(NEUTRAL)) {}

string NeutralPlayerStrategy::issueOrder(shared_ptr<Map> map, Deck* deck)
{
	return "Finish";
}

pair<shared_ptr<Territory>, int> NeutralPlayerStrategy::deploy(int armies, shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	shared_ptr<Territory> territory;
	string territoryName;

	bool territoryAllowed = false;

	do {
		cout << "You have " << armies << " deployable armies" << endl;
		cout << "\nCurrent territories you can deploy to: " << endl;
		cout << "--------------------------------------------" << endl;
		for (shared_ptr<Territory> t : territoryList)
		{
			cout << t->name << " (" << t->availableUnits << ")" << endl;
		}
		cout << "--------------------------------------------" << endl;
		cout << "\nWhich territory would you like to deploy to?" << endl;
		cin.clear();
		cin.ignore(123, '\n');
		cin >> territoryName;
		territory = map->findTerritory(territoryName);

		for (shared_ptr<Territory> t : territoryList)
		{
			if (t == territory)
			{
				territoryAllowed = true;
			}
		}

	} while (!territoryAllowed);

	cout << "\nHow many armies would you like to deploy? (Max " << armies << ")" << endl;
	cout << "--------------------------------------------" << endl;
	int numberOfArmiesToDeploy;
	while (true)
	{
		cin >> numberOfArmiesToDeploy;
		if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > armies || cin.fail())
		{
			cout << "\nInvalid input, please try again" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else break;
	}

	return pair<shared_ptr<Territory>, int>(territory, numberOfArmiesToDeploy);
}

list<shared_ptr<Territory>> NeutralPlayerStrategy::toDefend(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> NeutralPlayerStrategy::toAttack(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	return list<shared_ptr<Territory>>();
}