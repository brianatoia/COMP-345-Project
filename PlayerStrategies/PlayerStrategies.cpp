#include "../PlayerStrategies/PlayerStrategies.h"

void HumanPlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
{

}

list<shared_ptr<Territory>> HumanPlayerStrategy::toDefend(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> HumanPlayerStrategy::toAttack(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

//*********  AggressivePlayerStrategy  **********//
void AggressivePlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
{

}

list<shared_ptr<Territory>> AggressivePlayerStrategy::toDefend(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> AggressivePlayerStrategy::toAttack(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

//********* BenevolentPlayerStrategy  **********//

void BenevolentPlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
{

}

list<shared_ptr<Territory>> BenevolentPlayerStrategy::toDefend(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> BenevolentPlayerStrategy::toAttack(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

//********* NeutralPlayerStrategy  **********//

void NeutralPlayerStrategy::issueOrder(string orderType, shared_ptr<Map> map)
{

}

list<shared_ptr<Territory>> NeutralPlayerStrategy::toDefend(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> NeutralPlayerStrategy::toAttack(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}