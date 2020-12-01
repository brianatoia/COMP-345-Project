#include "../PlayerStrategies/PlayerStrategies.h"


HumanPlayerStrategy::HumanPlayerStrategy(): PlayerStrategy ()
{}

void HumanPlayerStrategy::issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck)
{
	//Deploying phase, force player to deploy first
	do
	{
		// player->issueOrder("Deploy", map);

		shared_ptr<Territory> territory;
		string territoryName;
		bool territoryAllowed = false;

		cout << player->getName() << "deploy your armies!" << endl;

		do {
			cout << "You have " << player->getArmies() << " deployable armies" << endl;
			cout << "\nCurrent territories you can deploy to: " << endl;
			cout << "--------------------------------------------" << endl;
			for (shared_ptr<Territory> t : *player->getTerritoryList())
			{
				cout << t->name << " (" << t->availableUnits << ")" << endl;
			}
			cout << "--------------------------------------------" << endl;
			cout << "\nWhich territory would you like to deploy to?" << endl;
			cin.clear();
			cin.ignore(123, '\n');
			cin >> territoryName;
			territory = map->findTerritory(territoryName);

			for (shared_ptr<Territory> t : *player->getTerritoryList())
			{
				if (t == territory)
				{
					territoryAllowed = true;
				}
			}

		} while (!territoryAllowed);

		cout << "\nHow many armies would you like to deploy? (Max " << player->getArmies() << ")" << endl;
		cout << "--------------------------------------------" << endl;
		int numberOfArmiesToDeploy;
		while (true)
		{
			cin >> numberOfArmiesToDeploy;
			if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > player->getArmies() || cin.fail())
			{
				cout << "\nInvalid input, please try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
			}
			else break;
		}

		territory->availableUnits += numberOfArmiesToDeploy;
		player->setArmies(player->getArmies() - numberOfArmiesToDeploy);

		shared_ptr<Order> order(new Deploy(numberOfArmiesToDeploy, territory, *player->getTerritoryList()));
		player->getOrderList()->addOrder(order);

	} while (player->getArmies() > 0);

	cout << "\n==================================================" << endl;
	cout << player->getName() << " place your orders!" << endl;
	cout << "==================================================" << endl;

	if (gameEngine->getObserverStatus(1))
	{
		string message = "wait";
		string time = std::to_string(1000);
		gameEngine->notify(message + " " + time);
	}


	//Player issues orders here
	string decision = "";
	while (true)
	{
		if (gameEngine->getObserverStatus(1))
		{
			string message = "order ";
			string id = std::to_string(player->getPlayerID());
			gameEngine->notify(message + id);
		}

		cin >> decision;

		if (_stricmp(decision.c_str(), "Advance") == 0)
		{
			if (player->canAdvance()) {
				// player->issueOrder("Advance", map);
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
					for (shared_ptr<Territory> t : *player->getTerritoryList())
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

				auto targetPlayer = gameEngine->getPlayer(targetTerritory->ownerID - 1);

				shared_ptr<Order> order(new Advance(numOfArmies, sourceTerritory, targetTerritory, player->getTerritoryList(), targetPlayer != nullptr ? targetPlayer->getTerritoryList() : nullptr, player->getCapturedTerritory(), &playersNegotiated));
				player->getOrderList()->addOrder(order);
			}
			else cout << "You no longer have any armies to advance!" << endl;
		}																				// === format followed for all order types === //
		else if (_stricmp(decision.c_str(), "Airlift") == 0)							//case where player chooses airlift
		{
			if (player->getHand()->findCardType("Airlift"))								//check if player has atleast 1 airlift card
			{
				// player->issueOrder("Airlift", map);									//issue the order (puts it in order list)

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
					for (shared_ptr<Territory> t : *player->getTerritoryList())
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


				auto targetPlayer = gameEngine->getPlayer(targetTerritory->ownerID - 1);

				shared_ptr<Order> order(new Airlift(numOfArmies, sourceTerritory, targetTerritory, player->getTerritoryList(), targetPlayer != nullptr ? targetPlayer->getTerritoryList() : nullptr, player->getCapturedTerritory(), &playersNegotiated));
				player->getOrderList()->addOrder(order);



				// 
				player->getHand()->play(player->getHand()->getCard("Airlift"), deck);	//play the card
			}
			else cout << "Not enough cards!" << endl;									//if not enough cards tell player
		}
		else if (_stricmp(decision.c_str(), "Blockade") == 0)
		{
			if (player->getHand()->findCardType("Blockade"))
			{
				// player->issueOrder("Blockade", map);

				shared_ptr<Territory> territory;
				string territoryName;
				bool territoryAllowed = false;

				do {
					cout << "\nCurrent territories you can blockade: " << endl;
					cout << "--------------------------------------------" << endl;
					for (shared_ptr<Territory> t : *player->getTerritoryList())
					{
						cout << t->name << " (" << t->availableUnits << ")" << endl;
					}
					cout << "--------------------------------------------" << endl;
					cout << "\nWhich territory would you like to blockade?" << endl;
					cin.clear();
					cin.ignore(123, '\n');
					cin >> territoryName;
					territory = map->findTerritory(territoryName);

					for (shared_ptr<Territory> t : *player->getTerritoryList())
					{
						if (t == territory)
						{
							territoryAllowed = true;
						}
					}

				} while (!territoryAllowed);


				shared_ptr<Order> order(new Blockade(territory, player->getTerritoryList()));
				player->getOrderList()->addOrder(order);





				//
				player->getHand()->play(player->getHand()->getCard("Blockade"), deck);
			}
			else cout << "Not enough cards!" << endl;
		}
		else if (_stricmp(decision.c_str(), "Reinforcement") == 0)
		{
			if (player->getHand()->findCardType("Reinforcement"))
			{
				player->getHand()->play(player->getHand()->getCard("Reinforcement"), deck);
				player->addArmies(20);	//Reinforcement is not an order so do it directly and treat it as a deploy
				do
				{
					shared_ptr<Territory> territory;
					string territoryName;
					bool territoryAllowed = false;

					cout << player->getName() << "deploy your armies!" << endl;

					do {
						cout << "You have " << player->getArmies() << " deployable armies" << endl;
						cout << "\nCurrent territories you can deploy to: " << endl;
						cout << "--------------------------------------------" << endl;
						for (shared_ptr<Territory> t : *player->getTerritoryList())
						{
							cout << t->name << " (" << t->availableUnits << ")" << endl;
						}
						cout << "--------------------------------------------" << endl;
						cout << "\nWhich territory would you like to deploy to?" << endl;
						cin.clear();
						cin.ignore(123, '\n');
						cin >> territoryName;
						territory = map->findTerritory(territoryName);

						for (shared_ptr<Territory> t : *player->getTerritoryList())
						{
							if (t == territory)
							{
								territoryAllowed = true;
							}
						}

					} while (!territoryAllowed);

					cout << "\nHow many armies would you like to deploy? (Max " << player->getArmies() << ")" << endl;
					cout << "--------------------------------------------" << endl;
					int numberOfArmiesToDeploy;
					while (true)
					{
						cin >> numberOfArmiesToDeploy;
						if (numberOfArmiesToDeploy < 0 || numberOfArmiesToDeploy > player->getArmies() || cin.fail())
						{
							cout << "\nInvalid input, please try again" << endl;
							cin.clear();
							cin.ignore(10000, '\n');
						}
						else break;
					}

					territory->availableUnits += numberOfArmiesToDeploy;
					player->setArmies(player->getArmies() - numberOfArmiesToDeploy);

					shared_ptr<Order> order(new Deploy(numberOfArmiesToDeploy, territory, *player->getTerritoryList()));
					player->getOrderList()->addOrder(order);
				} while (player->getArmies() > 0);
			}
			else cout << "Not enough cards!" << endl;
		}
		else if (_stricmp(decision.c_str(), "Negotiate") == 0)
		{
			if (player->getHand()->findCardType("Diplomacy"))	//Different names for the same thing, just following the assignment
			{
				// player->issueOrder("Negotiate");

				int targetPlayerID;
				do
				{
					cout << "Enter the ID of the player you would like to negotiate with: ";
					cin.clear();
					cin.ignore(123, '\n');
					cin >> targetPlayerID;
				} while (targetPlayerID > Player::getPlayerCount() || targetPlayerID < 0);


				shared_ptr<Order> order(new Negotiate(player->getPlayerID(), targetPlayerID, &playersNegotiated));
				player->getOrderList()->addOrder(order);






				//
				player->getHand()->play(player->getHand()->getCard("Diplomacy"), deck);
			}
			else cout << "Not enough cards!" << endl;
		}
		else if (_stricmp(decision.c_str(), "Bomb") == 0)
		{
			if (player->getHand()->findCardType("Bomb"))
			{
				// player->issueOrder("Bomb", map);

				shared_ptr<Territory> territory = nullptr;
				string territoryName;
				bool territoryAllowed = false;

				do {
					cout << "\nWhich territory would you like to bomb?" << endl;
					cout << "--------------------------------------------" << endl;
					for (shared_ptr<Territory> t : player->toAttack(map))
					{
						cout << t->name << endl;
					}
					cout << "--------------------------------------------" << endl;
					cin >> territoryName;

					territory = map->findTerritory(territoryName);

					for (shared_ptr<Territory> t : player->toAttack(map))
					{
						if (t == territory)
						{
							territoryAllowed = true;
						}
					}
				} while (!territoryAllowed);

				shared_ptr<Order> order(new Bomb(player->getPlayerID(), territory, player->getTerritoryList(), &playersNegotiated));
				player->getOrderList()->addOrder(order);



				//
				player->getHand()->play(player->getHand()->getCard("Bomb"), deck);
			}
			else cout << "Not enough cards!" << endl;
		}
		else if (_stricmp(decision.c_str(), "Finish") == 0)
		{
			break;
		}
		else
		{
			cout << "Invalid input" << endl;
		}
	}
}


HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& hps) : PlayerStrategy()
{
}
HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& hps) 
{
	PlayerStrategy::operator=(hps);
	return *this;
}

string HumanPlayerStrategy::to_string()
{
	return "HumanPlayerStrategy";
}

ostream& operator<<(ostream& strm, HumanPlayerStrategy hps)
{
	return strm << "HumanPlayerStrategy";
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
AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy()
{
}

void AggressivePlayerStrategy::issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck)
{
	//Deploying phase, force player to deploy first

	cout << player->getName() << " deploying their armies..." << endl;

	shared_ptr<Territory> territory = this->maxArmyTerritory(*player->getTerritoryList());

	int numberOfArmiesToDeploy = player->getArmies();

	territory->availableUnits += numberOfArmiesToDeploy;
	player->setArmies(player->getArmies() - numberOfArmiesToDeploy);

	shared_ptr<Order> order(new Deploy(numberOfArmiesToDeploy, territory, *player->getTerritoryList()));
	player->getOrderList()->addOrder(order);

	// Reinforcement cards
	while (player->getHand()->findCardType("Reinforcement"))
	{
		player->getHand()->play(player->getHand()->getCard("Reinforcement"), deck);
		player->addArmies(20);	//Reinforcement is not an order so do it directly and treat it as a deploy
		
		shared_ptr<Territory> territory = this->maxArmyTerritory(*player->getTerritoryList());

		int numberOfArmiesToDeploy = player->getArmies();

		territory->availableUnits += numberOfArmiesToDeploy;
		player->setArmies(player->getArmies() - numberOfArmiesToDeploy);

		shared_ptr<Order> order(new Deploy(numberOfArmiesToDeploy, territory, *player->getTerritoryList()));
		player->getOrderList()->addOrder(order);
	}

	while (player->getHand()->findCardType("Bomb"))
	{
		if (player->toAttack(map).size() == 0) break;
		else
		{
			shared_ptr<Territory> territory = this->maxArmyTerritory(player->toAttack(map));

			shared_ptr<Order> order(new Bomb(player->getPlayerID(), territory, player->getTerritoryList(), &playersNegotiated));
			player->getOrderList()->addOrder(order);
			player->getHand()->play(player->getHand()->getCard("Bomb"), deck);
		}
	}

	cout << player->canAdvance() << endl;

	if (player->canAdvance()) {
		shared_ptr<Territory> sourceTerritory = this->maxArmyTerritory(*player->getTerritoryList());
		shared_ptr<Territory> targetTerritory = this->findTargetTerritory(sourceTerritory, map);

		if (targetTerritory != nullptr)
		{
			int numOfArmies = sourceTerritory->availableUnits;
			sourceTerritory->availableUnits -= numOfArmies;

			auto targetPlayer = gameEngine->getPlayer(targetTerritory->ownerID - 1);

			shared_ptr<Order> order(new Advance(numOfArmies, sourceTerritory, targetTerritory, player->getTerritoryList(), targetPlayer != nullptr ? targetPlayer->getTerritoryList() : nullptr, player->getCapturedTerritory(), &playersNegotiated));
			player->getOrderList()->addOrder(order);
		}
	}

}

shared_ptr<Territory> AggressivePlayerStrategy::maxArmyTerritory(list<shared_ptr<Territory>> territoryList)
{
	shared_ptr<Territory> max = territoryList.front();

	for (shared_ptr<Territory> t : territoryList)
	{
		if (t->availableUnits > max->availableUnits)
		{
			max = t;
		}
	}

	return max;
}


shared_ptr<Territory> AggressivePlayerStrategy::findTargetTerritory(shared_ptr<Territory> territory, shared_ptr<Map> map)
{
	list<shared_ptr<Territory>> targets = list<shared_ptr<Territory>>();
	list<shared_ptr<Territory>> friends = list<shared_ptr<Territory>>();

	for (unsigned int i : territory->borders)
	{
		shared_ptr<Territory> t = map->getTerritory(i);
	
		if (t->ownerID != territory->ownerID)
		{
			targets.push_back(t);
		}
		else
		{
			friends.push_back(t);
		}
	}

	if (targets.size() > 0)
		return this->maxArmyTerritory(targets);
	else
	{
		for (shared_ptr<Territory> t : friends)
		{
			for (unsigned int i : t->borders)
			{
				shared_ptr<Territory> other = map->getTerritory(i);

				if (t->ownerID != other->ownerID)
				{
					return t;
				}
			}
		}

		if (friends.size() > 0) return friends.front();
	}

	cout << "targeting no one" << endl;

	return nullptr;
}

list<shared_ptr<Territory>> AggressivePlayerStrategy::toDefend(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

list<shared_ptr<Territory>> AggressivePlayerStrategy::toAttack(shared_ptr<Map> map)
{
	return list<shared_ptr<Territory>>();
}

AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& hps) : PlayerStrategy()
{
}
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& hps)
{
	PlayerStrategy::operator=(hps);
	return *this;
}

string AggressivePlayerStrategy::to_string()
{
	return "AggressivePlayerStrategy";
}

ostream& operator<<(ostream& strm, AggressivePlayerStrategy hps)
{
	return strm << "AggressivePlayerStrategy";
}

//********* BenevolentPlayerStrategy  **********//

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy()
{
}

void BenevolentPlayerStrategy::issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck)
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

BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& hps) : PlayerStrategy()
{
}
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& hps)
{
	PlayerStrategy::operator=(hps);
	return *this;
}

string BenevolentPlayerStrategy::to_string()
{
	return "BenevolentPlayerStrategy";
}

ostream& operator<<(ostream& strm, BenevolentPlayerStrategy hps)
{
	return strm << "BenevolentPlayerStrategy";
}

//********* NeutralPlayerStrategy  **********//

NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy()
{
}

void NeutralPlayerStrategy::issueOrder(GameEngine* gameEngine, Player* player, shared_ptr<Map> map, Deck* deck)
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

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& hps) : PlayerStrategy()
{
}
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& hps)
{
	PlayerStrategy::operator=(hps);
	return *this;
}

string NeutralPlayerStrategy::to_string()
{
	return "NeutralPlayerStrategy";
}

ostream& operator<<(ostream& strm, NeutralPlayerStrategy hps)
{
	return strm << "NeutralPlayerStrategy";
}