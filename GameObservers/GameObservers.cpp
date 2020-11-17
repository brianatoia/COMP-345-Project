#include "GameObservers.h"

Subject::Subject()
{
	observers = new list<Observer*>();
}

Subject::~Subject()
{
	delete observers;
}

void Subject::attach(Observer* observer)
{
	observers->push_back(observer);
}

void Subject::detach(Observer* observer)
{
	observers->remove(observer);
}

void Subject::notify()
{
	for (auto i = observers->begin(); i != observers->end(); ++i)
	{
		(*i)->update();
	}
}

Observer::Observer(Subject* model)
{
	this->model = model;
	model->attach(this);
}

Observer::~Observer()
{
	model->detach(this);
}

StatisticsObserver::StatisticsObserver(Subject* model, shared_ptr<Map> map, vector<shared_ptr<Player>> players) : Observer(model) {
	this->map = map;
	this->players = players;
}
StatisticsObserver::~StatisticsObserver() {}

void StatisticsObserver::update()
{
	show();
}

void StatisticsObserver::show()
{
	// model
	int numOfTerritories = map->getTerritoriesCount() - 1;
	
	printf("%-20s %-20s\n", "Player", "Domination (%)");
	printf("==================== ====================\n");
	for (shared_ptr<Player> p : this->players)
	{
		printf("%-20s %.2f %%\n", p->getName().c_str(), 100.0f * (float)p->getTerritoryList().size() / (float)numOfTerritories);
	};

	int numOfContinents = map->getContinentsCount();

	for (int c = 1; c < numOfContinents; c++)
	{
		shared_ptr<Continent> continent = map->getContinent(c);

		printf("\n%-20s %-20s %-20s\n", continent->name.c_str(), "Owner", "Units");
		printf("==================== ==================== ====================\n");

		for (int t = 0; t < continent->territoryIDs.size(); t++)
		{
			shared_ptr<Territory> territory = map->getTerritory(continent->territoryIDs[t]);
			printf("%-20s %-20s %-20d\n", territory->name.c_str(), players[territory->ownerID - 1]->getName().c_str(), territory->units);
		}
	}

	/* TODO:
	* Put a notify call in the methods that run the game loop
	* Make a celebretory message once a player controls all territories
	* Hookup Observers creation
	* */

}


