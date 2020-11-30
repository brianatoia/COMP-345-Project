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

HumanPlayerStrategy::HumanPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(HUMAN)) {}

string HumanPlayerStrategy::issueOrder(shared_ptr<Map> map)
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

string AggressivePlayerStrategy::issueOrder(shared_ptr<Map> map)
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

string BenevolentPlayerStrategy::issueOrder(shared_ptr<Map> map)
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

string NeutralPlayerStrategy::issueOrder(shared_ptr<Map> map)
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