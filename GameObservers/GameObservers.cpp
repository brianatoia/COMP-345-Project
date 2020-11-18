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
	
	list<shared_ptr<Player>> activePlayers = list<shared_ptr<Player>>();

	for (shared_ptr<Player> p : this->players)
	{
		if (p->getTerritoryList().size() > 0)
		{
			activePlayers.push_back(p);
		}
	};

	if (activePlayers.size() > 1)
	{
		printf("\n%-2s %-20s %-20s\n", "ID","Player", "Domination (%)");
		printf("== ==================== ====================\n");
		for (shared_ptr<Player> p : activePlayers)
		{

			printf("%2d %-20s %.2f %%\n", p->getPlayerID(), p->getName().c_str(), 100.0f * (float)p->getTerritoryList().size() / (float)numOfTerritories);
		};

		int numOfContinents = map->getContinentsCount();

		for (int c = 1; c < numOfContinents; c++)
		{
			shared_ptr<Continent> continent = map->getContinent(c);

			printf("\n   %-20s\n", continent->name.c_str());
			printf("== ==================== ==================== ==================== ====================\n");

			printf("%-2s %-20s %-20s %-20s %-20s\n", "ID", "Name", "Owner", "Units", "Borders");

			printf("== ==================== ==================== ==================== ====================\n");
			
			for (int t = 0; t < continent->territoryIDs.size(); t++)
			{
				shared_ptr<Territory> territory = map->getTerritory(continent->territoryIDs[t]);
				string playerName = territory->ownerID - 1 >= 0 ? players[territory->ownerID - 1]->getName() : "None";
				string borderList = "";

				list<unsigned int> borders = list<unsigned int>(territory->borders.begin(), territory->borders.end());

				borders.sort();

				for (int b : borders)
				{
					borderList += to_string(b) + (b < 10 ? "  " : " ");
				}

				printf("%2d %-20s %-20s %-20d %-20s\n", territory->getID(), territory->name.c_str(), playerName.c_str(), territory->units, borderList.c_str());
			}
		}
	}
	else
	{
		printf("=== Congratulations ===\n");
		printf("%s wins!\n", activePlayers.front()->getName().c_str());
	}

	/* TODO:
	* Put a notify call in the methods that run the game loop
	* Hookup Observers creation
	* */

}


