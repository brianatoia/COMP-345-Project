#include "../PlayerStrategies/PlayerStrategies.h"

PlayerStrategy::PlayerStrategy(unsigned int playerID, Hand* hand, OrderList* orderList)
{
	this->playerID = playerID;
	this->hand = hand;
	this->orderList = orderList;
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


string HumanPlayerStrategy::issueOrder(shared_ptr<Map> map)
{
	string decision = "";
	cin >> decision;
	return decision;
}


//*********  AggressivePlayerStrategy  **********//
void AggressivePlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
{

}

//********* BenevolentPlayerStrategy  **********//

void BenevolentPlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
{

}

//********* NeutralPlayerStrategy  **********//

void NeutralPlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
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