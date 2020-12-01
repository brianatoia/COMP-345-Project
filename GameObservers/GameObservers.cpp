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

void Subject::notify(string message)
{
	for (auto i = observers->begin(); i != observers->end(); ++i)
	{
		(*i)->update(message);
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


vector<string> Observer::tokenize(string s)
{
	string to_parse = s;

	vector<string> tokens = vector<string>();

	string delimiter = " ";

	std::size_t current, previous = 0;
	current = to_parse.find(delimiter);
	while (current != std::string::npos) {
		tokens.push_back(to_parse.substr(previous, current - previous));
		previous = current + 1;
		current = to_parse.find(delimiter, previous);
	}
	tokens.push_back(to_parse.substr(previous, current - previous));

	return tokens;
}

PhaseObserver::PhaseObserver(Subject* model, vector<shared_ptr<Player>> players) : Observer(model)
{
	this->players = players;
}

PhaseObserver::~PhaseObserver() {
	players.clear();
}

void PhaseObserver::update(string message)
{
	vector<string> tokens = tokenize(message);

	string command = tokens[0];

	if (command == "wait")
	{
		int time = stoi(tokens.at(1), nullptr, 0);
		Sleep(time);
	}

	if (command == "reinforcement_phase_start")
	{
		cout << "\nReinforcement Phase:\n" << endl;
	}

	if (command == "give_armies")
	{
		int armiesToGive = stoi(tokens.at(1), nullptr, 0);
		int playerIndex = stoi(tokens.at(2), nullptr, 0) - 1;
		int baseArmiesGiven = stoi(tokens.at(3), nullptr, 0);
		int continentBonus = stoi(tokens.at(4), nullptr, 0);

		cout << "Gave " << armiesToGive << " armies to Player " << players[playerIndex]->getName() << " (minimum 3)" << endl;
		cout << "(" << baseArmiesGiven << ") plus (" << continentBonus << ") from continent bonus(es)\n" << endl;

		Sleep(1000);
	}

	if (command == "draws_card")
	{
		int playerIndex = stoi(tokens.at(1), nullptr, 0) - 1;
		cout << players[playerIndex]->getName() << " also draws a card because they captured atleast one territory last turn" << endl;

		Sleep(1000);
	}

	if (command == "issue_order_start")
	{
		cout << "\nStart of issueOrderPhase()\n" << endl;
		Sleep(1000); //Small sleeps to make information readable to player (don't want to fill screen with text)

		cout << "Deploying Phase: \n" << endl;
		Sleep(1000);
	}

	if (command == "order")
	{
		int playerIndex = stoi(tokens.at(1), nullptr, 0) - 1;

		cout << "\nAirlift (" << players[playerIndex]->getHand()->findNumberOfType("Airlift") << ")" << endl;				Sleep(200);
		cout << "Blockade (" << players[playerIndex]->getHand()->findNumberOfType("Blockade") << ")" << endl;				Sleep(200);
		cout << "Reinforcement (" << players[playerIndex]->getHand()->findNumberOfType("Reinforcement") << ")" << endl;	Sleep(200);
		cout << "Negotiate (" << players[playerIndex]->getHand()->findNumberOfType("Diplomacy") << ")" << endl;			Sleep(200);
		cout << "Bomb (" << players[playerIndex]->getHand()->findNumberOfType("Bomb") << ")" << endl;						Sleep(200);
		cout << "Advance" << endl;																			Sleep(200);
		cout << "Finish" << endl;																			Sleep(200);
		cout << "\nOrders are listed top - down in order of execution priority, but all orders of one type must execute before the next can occur!" << endl;	Sleep(200);
		cout << "Only one order is played at a time for a player, unless they are the last one with that order type in queue." << endl;							Sleep(200);
		cout << "Orders of a type you create first happen first, so start with what you want to have happen right away!" << endl;
		cout << "Use Advance to move armies to another adjacent friendly territory or to attack enemy territories" << endl;
		cout << "==================================================" << endl;
	}

	if (command == "execute_orders_phase_start")
	{
		cout << "\nStart of executeOrdersPhase()" << endl;
		Sleep(2000);
	}

	if (command == "execute")
	{
		bool first = true;
		for (auto s : tokens)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				cout << s << " ";
			}
		}
		cout << endl << endl;
		Sleep(500);
	}
	
}

StatisticsObserver::StatisticsObserver(Subject* model, shared_ptr<Map> map, vector<shared_ptr<Player>> players) : Observer(model) 
{
	this->map = map;
	this->players = players;
}

StatisticsObserver::~StatisticsObserver() {
	players.clear();
	map.reset();
}

void StatisticsObserver::update(string message)
{
	vector<string> tokens = tokenize(message);

	string command = tokens[0];

	if (command == "map")
	{
		show();
	}
}

void StatisticsObserver::show()
{
	// model
	int numOfTerritories = map->getTerritoriesCount() - 1;
	
	list<shared_ptr<Player>> activePlayers = list<shared_ptr<Player>>();

	for (shared_ptr<Player> p : this->players)
	{
		if (p->getTerritoryList()->size() > 0)
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
			printf("%2d %-20s %.2f %%\n", p->getPlayerID(), p->getName().c_str(), 100.0f * (float)p->getTerritoryList()->size() / (float)numOfTerritories);
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

				string playerName = "None";

				for (shared_ptr<Player> p : activePlayers)
				{
					if (territory->ownerID == p->getPlayerID())
					{
						playerName = p->getName();
					}
				}

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


