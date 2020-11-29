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

//*********  AggressivePlayerStrategy  **********//
AggressivePlayerStrategy::AggressivePlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(AGGRESSIVE)) {}

string AggressivePlayerStrategy::issueOrder(shared_ptr<Map> map)
{

}

//********* BenevolentPlayerStrategy  **********//
BenevolentPlayerStrategy::BenevolentPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(BENEVOLENT)) {}

string BenevolentPlayerStrategy::issueOrder(shared_ptr<Map> map)
{

}

//********* NeutralPlayerStrategy  **********//
NeutralPlayerStrategy::NeutralPlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList) : PlayerStrategy(playerID, hand, orderList, new StrategyType(NEUTRAL)) {}

string NeutralPlayerStrategy::issueOrder(shared_ptr<Map> map)
{

}

list<shared_ptr<Territory>> NeutralPlayerStrategy::toDefend(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> NeutralPlayerStrategy::toAttack(shared_ptr<Map> map, list<shared_ptr<Territory>> territoryList)
{
	return list<shared_ptr<Territory>>();
}