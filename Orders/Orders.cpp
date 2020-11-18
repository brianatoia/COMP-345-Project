#include "Orders.h"
#include <random>
#include <tuple>


//Order Constructors
Order::Order()
{
	*orderType = UNDEFINED;
	orderDescription = "Undefined";
	orderEffect = "Undefined";
}

Order::Order(OrderType* orderType)
{
	this->orderType = orderType;
	this->orderEffect = "Undefined";
	if (*orderType == DEPLOY)
	{
		this->orderDescription = "Place some armies on one of the current player's territories.";
	}
	else if (*orderType == ADVANCE)
	{
		this->orderDescription = "Move some armies from one of the current player's territories (source) to an adjacent territory " 
		"(target). If the target territory belongs to the current player, the armies are moved to the target " 
			"territory. If the target territory belongs to another player, an attack happens between the two territories.";
	}
	else if (*orderType == BOMB)
	{
		this->orderDescription = "Destroy half of the armies located on an opponent's territory that is adjacent to one of the current " 
			"player's territories.";
	}
	else if (*orderType == BLOCKADE)
	{
		this->orderDescription = "Double the number of armies on one of the current player's territories and make it a neutral territory.";
	}
	else if (*orderType == AIRLIFT)
	{
		this->orderDescription = "Advance some armies from one of the current player's territories to any another territory.";
	}
	else if (*orderType == NEGOTIATE)
	{
		this->orderDescription = "Prevent attacks between the current player and another player until the end of the turn.";
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}

Order::~Order()
{
	//delete orderType;
	//orderType = nullptr;
}

//Order Getters
Order::OrderType Order::getOrderType()
{
	return *orderType;
}

string Order::getOrderTypeString()
{
	switch (*(this->orderType))
		{
			case DEPLOY: return "Deploy";
			case ADVANCE: return "Advance";
			case BOMB: return "Bomb";
			case BLOCKADE: return "Blockade";
			case AIRLIFT: return "Airlift";
			case NEGOTIATE: return "Negotiate";
			case UNDEFINED: return "Undefined";
		}
}

string Order::getOrderDescription()
{
	return orderDescription;
}

string Order::getOrderEffect()
{
	return orderEffect;
}

//Order Setters
void Order::setOrderType(OrderType* orderType)
{
	*(this->orderType) = *orderType;
}

void Order::setOrderDescription(string orderDescription)
{
	this->orderDescription = orderDescription;
}

void Order::setOrderEffect(string orderEffect)
{
	this->orderEffect = orderEffect;
}


bool Order::validate() { return false; } //inherited & implemented in subclasses, false if invalid order type is created

void Order::execute() {} //inherited & implemented in subclasses


//OrderList Default Constructor
OrderList::OrderList()
{
	orders.empty();
}

//OrderList Destructor
OrderList::~OrderList()
{
	for (shared_ptr<Order> order : this->orders) 
	{
		order.reset();
	}
	//will clear the list, deleting all the shared_ptrs (which does so themselves)
	orders.clear();
}

//OrderList Getter (in case needed)
list<shared_ptr<Order>> OrderList::getOrders()
{
	return orders;
}

//OrderList Setter (in case needed)
void OrderList::setOrders(list<shared_ptr<Order>> orderList)
{
	this->orders = orderList;
}

void OrderList::addOrder(shared_ptr<Order> order)
{
	if (order->validate())
	{
		orders.push_back(order);
	}
}

//Removes an Order from the List
void OrderList::remove(shared_ptr<Order> orderToRemove)
{ 
	for (std::shared_ptr<Order> order : this->orders) 
	{
		if (order == orderToRemove)
		{
			orders.remove(orderToRemove);
			return;
		}
	}
}

//Moves an Order in the List: up, down, top or bottom
void OrderList::move(shared_ptr<Order> order, MoveOption* moveOption)
{
	bool orderExists = false;
	list<shared_ptr<Order>>::iterator it = orders.begin();
	for (it = orders.begin(); it != orders.end(); advance(it, 1))
	{
		if (*it == order)
		{
			orderExists = true;
			break;
		}
	}

	if (!orderExists)
	{
		return;
	}
	//Move order to beginnning of list
	if (*moveOption == BEGINNING)
	{
		orders.splice(orders.begin(), orders, it);
	}
	//Move order to end of list
	else if(*moveOption == END)
	{
		orders.splice(orders.end(), orders, it);
	}
	//Move order up one position
	else if (*moveOption == UP)
	{
		//check that it's not the first element
		if (it != orders.begin())
		{
			list<shared_ptr<Order>>::iterator itUp = it;
			advance(itUp, -1);
			orders.splice(itUp, orders, it);
		}
	}
	//Move order down one position
	else if (*moveOption == DOWN)
	{
		//check that it's not the last element
		list<shared_ptr<Order>>::iterator itEnd = orders.end();
		advance(itEnd, -1);
		if (it != itEnd)
		{
			list<shared_ptr<Order>>::iterator itDown = it;
			advance(itDown, 2);
			orders.splice(itDown, orders, it);
		}
	}
	else
	{
		cerr << "Invalid movement option" << endl;
	}
}

Order::OrderType deployO = Order::DEPLOY;
Deploy::Deploy(int numOfArmies, shared_ptr<Territory> territory, list<shared_ptr<Territory>> playerTerritories) : Order(&deployO)
{
	this->numOfArmies = numOfArmies;
	this->territory = territory;
	this->playerTerritories = playerTerritories;
}

Deploy::~Deploy()
{
	territory.reset();

	for (shared_ptr<Territory> t : playerTerritories)
	{
		t.reset();
	}
	playerTerritories.clear();
}

bool Deploy::validate()
{
	for (shared_ptr<Territory> t : playerTerritories)
	{
		if (t == territory) 
		{
			return true;
		}
	}

	cerr << "\nInvalid Order. Cannot deploy to a territory you do not control." << endl;
	
	return false;
}

void Deploy::execute()
{

	if (validate())
	{
		//add armies to territory 
		territory->units += numOfArmies;

		string s = "Deployed " + std::to_string(numOfArmies) + " armies to " + territory->name;
		setOrderEffect(s);
	}
}

Order::OrderType advanceO = Order::ADVANCE;
Advance::Advance(int numOfArmies, shared_ptr<Territory> sourceTerritory, shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>>* playerTerritories, list<shared_ptr<Territory>>* targetPlayerTerritories, 
							bool* capturedTerritory, list<tuple<int, int>>* playersNegotiated) : Order(&advanceO)
{
	this->numOfArmies = numOfArmies;
	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;
	this->playerTerritories = playerTerritories;
	this->targetPlayerTerritories = targetPlayerTerritories;
	this->capturedTerritory = capturedTerritory;
	this->playersNegotiated = playersNegotiated;
}

Advance::~Advance()
{
	sourceTerritory.reset();
	targetTerritory.reset();


	/*for (shared_ptr<Territory> t : *playerTerritories)
	{
		t.reset();
	}
	playerTerritories->clear();

	delete playerTerritories;
	playerTerritories = nullptr;

	for (shared_ptr<Territory> t : *targetPlayerTerritories)
	{
		t.reset();
	}
	targetPlayerTerritories->clear();

	delete targetPlayerTerritories;
	targetPlayerTerritories = nullptr;

	delete capturedTerritory;
	capturedTerritory = nullptr;

	delete playersNegotiated;
	playersNegotiated = nullptr;*/
}

bool Advance::validate()
{
	bool playerOwnsSource = false;
	bool sourceBordersTarget = false;

	for (shared_ptr<Territory> t : *playerTerritories)
	{
		if (t == sourceTerritory)
		{
			playerOwnsSource = true;
			break;
		}
	}
	if (!playerOwnsSource)
	{
		cerr << "\nInvalid Order. Source territory does not belong to player that issued order." << endl;
		return false;
	}

	for (int t : sourceTerritory->borders)
	{
		if (t == targetTerritory->getID())
		{
			sourceBordersTarget = true;
			break;
		}
	}
	if (!sourceBordersTarget)
	{
		cerr << "\nInvalid Order. Source territory does not border target territory." << endl;
		return false;
	}

	return true;
}

string attack(int numOfArmies, shared_ptr<Territory> sourceTerritory, shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>>* playerTerritories, 
					list<shared_ptr<Territory>>* targetPlayerTerritories, bool* capturedTerritory)
{
	int n;
	string s = "";
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<float> dist(0, 1);

	int sourceArmiesAttacking = 0, targetArmiesDefending = 0;
	//Each attacking army unit involved has 60% chances of killing one defending army. 
	for (int i = 0; i < numOfArmies; i++)
	{
		float chance = dist(mt);
		if (chance <= 0.6)
		{
			sourceArmiesAttacking += 1;
		}
	}
	//Each defending army unit has 70% chances of killing one attacking army unit.
	for (int i = 0; i < targetTerritory->units; i++)
	{
		float chance = dist(mt);
		if (chance <= 0.7)
		{
			targetArmiesDefending += 1;
		}
	}

	n = targetTerritory->units - sourceArmiesAttacking;
	targetTerritory->units = n < 0 ? 0 : n; //fixes issue with unsigned int
	
	n = sourceTerritory->units - targetArmiesDefending;
	sourceTerritory->units = n < 0 ? 0 : n; //fixes issue with unsigned int

	numOfArmies -= targetArmiesDefending;

	//Defending army defeated all of Attacking territoy armies
	if (sourceTerritory->units == 0 && numOfArmies == 0)
	{
		sourceTerritory->ownerID = NULL;
		playerTerritories->remove(sourceTerritory);

		s += sourceTerritory->name + " has no armies remaining. Player " + std::to_string(sourceTerritory->ownerID) + " no longer owns this territory.\n";
	}
	//Attacking army defeated all Defending territory armies
	if (targetTerritory->units == 0)
	{
		targetTerritory->ownerID = NULL;
		targetPlayerTerritories->remove(targetTerritory);

		s += targetTerritory->name + " has been defeated.\n";

		//If Attacking army still has units, can capture the defeated target territory 
		if (numOfArmies != 0)
		{
			targetTerritory->units = numOfArmies;
			sourceTerritory->units -= numOfArmies;
			targetTerritory->ownerID = sourceTerritory->ownerID;
			playerTerritories->push_back(targetTerritory);
			s += "Player "+ std::to_string(sourceTerritory->ownerID) + " now has " + std::to_string(numOfArmies) + " armies in " + targetTerritory->name + ". ";
			*capturedTerritory = true;
		}
	}
	if (targetTerritory->units != 0 && sourceTerritory->units != 0)
	{
		s += "Both territories remain standing.";
	}
	return(s);
}

void Advance::execute()
{
	if (validate())
	{
		string s;
		//if target territory does not have an owner, can take it 
		if (targetTerritory->ownerID == NULL && targetTerritory->units == 0)
		{
			targetTerritory->ownerID = sourceTerritory->ownerID;
			targetTerritory->units += numOfArmies;
			sourceTerritory->units -= numOfArmies;
			playerTerritories->push_back(targetTerritory);
			*capturedTerritory = true;

			s = "Target territory does not belong to a player. Deploying " + std::to_string(numOfArmies) + " armies from " + sourceTerritory->name + " to " + targetTerritory->name;
		}
		//if target territory is owned by the attacking territory owner
		else if (sourceTerritory->ownerID == targetTerritory->ownerID)
		{
			sourceTerritory->units -= numOfArmies;
			targetTerritory->units += numOfArmies;

			s = "Target territory belongs to player. Deploying " + std::to_string(numOfArmies) + " armies from " + sourceTerritory->name + " to " + targetTerritory->name;
		}
		//target territory is not owned by the attacking territory owner
		else
		{
			bool negotiatedWith = false;
			for (tuple<int, int> t : *playersNegotiated)
			{
				if (t == make_tuple(sourceTerritory->ownerID, targetTerritory->ownerID) || t == make_tuple(targetTerritory->ownerID, sourceTerritory->ownerID))
				{
					negotiatedWith = true;
					break;
				}
			}

			if (negotiatedWith)
			{
				s = "Source and Target territory owners have negotiated together. Cannot attack each other this turn.";
			}
			else
			{
				s = attack(numOfArmies, sourceTerritory, targetTerritory, playerTerritories, targetPlayerTerritories, capturedTerritory);
			}
		}
		setOrderEffect(s);
	}
}

Order::OrderType bombO = Order::BOMB;
Bomb::Bomb(unsigned int attackerID, shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>>* playerTerritories, list<tuple<int, int>>* playersNegotiated) : Order(&bombO)
{
	this->attackerID = attackerID;
	this->targetTerritory = targetTerritory;
	this->playerTerritories = playerTerritories;
	this->playersNegotiated = playersNegotiated;
}

Bomb::~Bomb()
{
	targetTerritory.reset();

	/*for (shared_ptr<Territory> t : *playerTerritories)
	{
		t.reset();
	}
	playerTerritories->clear();

	//delete playerTerritories;
	//playerTerritories = nullptr;

	playersNegotiated->clear();
	//delete playersNegotiated;
	//playersNegotiated = nullptr;*/
}

bool Bomb::validate()
{
	for (shared_ptr<Territory> t : *playerTerritories)
	{
		if (t == targetTerritory)
		{
			cerr << "\nCannot bomb own territory." << endl;
			return false;
		}
	}
	bool negotiatedWith = false;
	for (tuple<int, int> t : *playersNegotiated)
	{
		if (t == make_tuple(attackerID, targetTerritory->ownerID) || t == make_tuple(targetTerritory->ownerID, attackerID))
		{
			cerr << "Source and Target territory owners have negotiated together. Cannot attack each other this turn." << endl;
			return false;
		}
	}

	return true;
}

void Bomb::execute()
{
	if (validate())
	{
		int armiesToBomb = targetTerritory->units / 2;
		targetTerritory->units -= armiesToBomb;

		string s = "Player" + std::to_string(targetTerritory->ownerID + 1) + " bombed " + std::to_string(armiesToBomb) + " armies in " + targetTerritory->name;
		setOrderEffect(s);
	}
}

Order::OrderType blockadeO = Order::BLOCKADE;
Blockade::Blockade(shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>>& playerTerritories) : Order(&blockadeO)
{
	this->targetTerritory = targetTerritory;
	this->playerTerritories = &playerTerritories;
}

Blockade::~Blockade()
{
	targetTerritory.reset();

	/*for (shared_ptr<Territory> t : *playerTerritories)
	{
		t.reset();
	}
	playerTerritories->clear();*/
}

bool Blockade::validate()
{
	for (shared_ptr<Territory> t : *playerTerritories)
	{
		if (t == targetTerritory)
		{
			return true;
		}
	}

	cerr << "\nCannot blockade a territory player does not own." << endl;
	return false;
}

void Blockade::execute()
{
	if (validate())
	{
		targetTerritory->units *= 2;
		targetTerritory->ownerID = NULL;
		playerTerritories->remove(targetTerritory);

		//Separating string avoided an error
		string s = targetTerritory->name + " now has " + std::to_string(targetTerritory->units) + " armies. Ownership now transferred to Neutral player.";
		setOrderEffect(s);
	}
}

Order::OrderType airliftO = Order::AIRLIFT;
Airlift::Airlift(int numOfArmies, shared_ptr<Territory> sourceTerritory, shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>>* playerTerritories, list<shared_ptr<Territory>>* targetPlayerTerritories, 
						bool* capturedTerritory, list<tuple<int, int>>* playersNegotiated) : Order(&airliftO)
{
	this->numOfArmies = numOfArmies;
	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;
	this->playerTerritories = playerTerritories;
	this->targetPlayerTerritories = targetPlayerTerritories;
	this->capturedTerritory = capturedTerritory;
	this->playersNegotiated = playersNegotiated;
}

Airlift::~Airlift()
{
	/*sourceTerritory.reset();
	targetTerritory.reset();

	for (shared_ptr<Territory> t : *playerTerritories)
	{
		t.reset();
	}
	playerTerritories->clear();

	//delete playerTerritories;
	//playerTerritories = nullptr;

	for (shared_ptr<Territory> t : *targetPlayerTerritories)
	{
		t.reset();
	}
	targetPlayerTerritories->clear();

	//delete targetPlayerTerritories;
	//targetPlayerTerritories = nullptr;

	//delete capturedTerritory;
	//capturedTerritory = nullptr;

	playersNegotiated->clear();
	//delete playersNegotiated;
	//playersNegotiated = nullptr;*/
}

bool Airlift::validate()
{
	bool playerOwnsSource = false;
	bool playerOwnsTarget = false;

	for (shared_ptr<Territory> t : *playerTerritories)
	{
		if (t == sourceTerritory)
		{
			playerOwnsSource = true;
			continue;
		}
		if (t == targetTerritory)
		{
			playerOwnsTarget = true;
			continue;
		}
	}
	if (!playerOwnsSource)
	{
		cerr << "\nInvalid Order. Player does not own source territory." << endl;
		return false;
	}
	if (!playerOwnsTarget)
	{
		cerr << "\nInvalid Order. Player does not own target territory." << endl;
		if (playerOwnsSource)
		{
			cerr << "Issuing Attack..." << endl;
			string s;
			bool negotiatedWith = false;
			for (tuple<int, int> t : *playersNegotiated)
			{
				if (t == make_tuple(sourceTerritory->ownerID, targetTerritory->ownerID) || t == make_tuple(targetTerritory->ownerID, sourceTerritory->ownerID))
				{
					negotiatedWith = true;
					break;
				}
			}

			if (negotiatedWith)
			{
				s = "Source and Target territory owners have negotiated together. Cannot attack each other this turn.";
			}
			else
			{
				s = attack(numOfArmies, sourceTerritory, targetTerritory, playerTerritories, targetPlayerTerritories, capturedTerritory);
			}

			setOrderEffect(s);
		}
		return false;
	}

	return true;
}

void Airlift::execute()
{
	if (validate())
	{
		sourceTerritory->units -= numOfArmies;
		targetTerritory->units += numOfArmies;

		string s = "Airlifting " + std::to_string(numOfArmies) + " from " + sourceTerritory->name + " to " + targetTerritory->name;
		setOrderEffect(s);
	}

}

Order::OrderType negotiateO = Order::NEGOTIATE;
Negotiate::Negotiate(int sourcePlayerID, int targetPlayerID, list<tuple<int, int>>* playersNegotiated) : Order(&negotiateO)
{
	this->sourcePlayerID = sourcePlayerID;
	this->targetPlayerID = targetPlayerID;
	this->playersNegotiated = playersNegotiated;
}

Negotiate::~Negotiate()
{
	playersNegotiated->clear();
	//delete playersNegotiated;
	//playersNegotiated = nullptr;
}

bool Negotiate::validate()
{
	if (sourcePlayerID == targetPlayerID)
	{
		cerr << "Cannot negotiate with yourself." << endl;
		return false;
	}
	
	return true;
}

void Negotiate::execute()
{
	if (validate())
	{
		playersNegotiated->push_back(std::make_tuple(sourcePlayerID, targetPlayerID));

		string s = "Player" + std::to_string(sourcePlayerID) + " has negotiated with Player" + std::to_string(targetPlayerID);
		setOrderEffect(s);
	}
}

//Copy Constructors
Order::Order(const Order& order)
{
	this->orderType = order.orderType;
	this->orderDescription = order.orderDescription;
	this->orderEffect = order.orderEffect;
}

OrderList::OrderList(const OrderList& orderList)
{
	for (std::shared_ptr<Order> order : orderList.orders) {
		this->orders.push_back(order);
	}
}

Deploy::Deploy(const Deploy& order) : Order(order)
{
	this->numOfArmies = order.numOfArmies;
	this->territory = order.territory;
	this->playerTerritories = order.playerTerritories;
}

Advance::Advance(const Advance& order) : Order(order)
{
	this->numOfArmies = order.numOfArmies;
	this->sourceTerritory = order.sourceTerritory;
	this->targetTerritory = order.targetTerritory;
	this->playerTerritories = order.playerTerritories;
	this->targetPlayerTerritories = order.targetPlayerTerritories;
	this->capturedTerritory = order.capturedTerritory;
	this->playersNegotiated = order.playersNegotiated;
}

Bomb::Bomb(const Bomb& order) : Order(order)
{
	this->attackerID = order.attackerID;
	this->targetTerritory = order.targetTerritory;
	this->playerTerritories = order.playerTerritories;
	this->playersNegotiated = order.playersNegotiated;
}

Blockade::Blockade(const Blockade& order) : Order(order)
{
	this->targetTerritory = order.targetTerritory;
	this->playerTerritories = order.playerTerritories;
}

Airlift::Airlift(const Airlift& order) : Order(order)
{
	this->numOfArmies = order.numOfArmies;
	this->sourceTerritory = order.sourceTerritory;
	this->targetTerritory = order.targetTerritory;
	this->playerTerritories = order.playerTerritories;
	this->targetPlayerTerritories = order.targetPlayerTerritories;
	this->capturedTerritory = order.capturedTerritory;
	this->playersNegotiated = order.playersNegotiated;
}

Negotiate::Negotiate(const Negotiate& order) : Order(order)
{
	this->sourcePlayerID = order.sourcePlayerID;
	this->targetPlayerID = order.targetPlayerID;
	this->playersNegotiated = order.playersNegotiated;
}

//Assignment operator
Order& Order::operator=(const Order& rightSide)
{
	this->orderType = rightSide.orderType;
	this->orderDescription = rightSide.orderDescription;
	this->orderEffect = rightSide.orderEffect;
	return *this;
}

OrderList& OrderList::operator=(const OrderList& rightSide)
{
	this->orders.clear();
	this->orders = rightSide.orders;
	return *this;
}

Deploy& Deploy::operator=(const Deploy& rightSide) 
{
	Order::operator=(rightSide);
	this->numOfArmies = rightSide.numOfArmies;
	this->territory = rightSide.territory;
	this->playerTerritories = rightSide.playerTerritories;
	return *this;
}

Advance& Advance::operator=(const Advance& rightSide)
{
	Order::operator=(rightSide);
	this->numOfArmies = rightSide.numOfArmies;
	this->sourceTerritory = rightSide.sourceTerritory;
	this->targetTerritory = rightSide.targetTerritory;
	this->playerTerritories = rightSide.playerTerritories;
	this->targetPlayerTerritories = rightSide.targetPlayerTerritories;
	this->capturedTerritory = rightSide.capturedTerritory;
	this->playersNegotiated = rightSide.playersNegotiated;
	return *this;
}

Bomb& Bomb::operator=(const Bomb& rightSide)
{
	Order::operator=(rightSide);
	this->attackerID = rightSide.attackerID;
	this->targetTerritory = rightSide.targetTerritory;
	this->playerTerritories = rightSide.playerTerritories;
	this->playersNegotiated = rightSide.playersNegotiated;
	return *this;
}

Blockade& Blockade::operator=(const Blockade& rightSide)
{
	Order::operator=(rightSide);
	this->targetTerritory = rightSide.targetTerritory;
	this->playerTerritories = rightSide.playerTerritories;
	return *this;
}

Airlift& Airlift::operator=(const Airlift& rightSide)
{
	Order::operator=(rightSide);
	this->numOfArmies = rightSide.numOfArmies;
	this->sourceTerritory = rightSide.sourceTerritory;
	this->targetTerritory = rightSide.targetTerritory;
	this->playerTerritories = rightSide.playerTerritories;
	this->targetPlayerTerritories = rightSide.targetPlayerTerritories;
	this->capturedTerritory = rightSide.capturedTerritory;
	this->playersNegotiated = rightSide.playersNegotiated;
	return *this;
}

Negotiate& Negotiate::operator=(const Negotiate& rightSide)
{
	Order::operator=(rightSide);
	this->sourcePlayerID = rightSide.sourcePlayerID;
	this->targetPlayerID = rightSide.targetPlayerID;
	this->playersNegotiated = rightSide.playersNegotiated;
	return *this;
}

//to_String functions
string Order::to_string()
{
	string str = "Order: " + getOrderTypeString();
	str += "\nDescription: " + orderDescription;
	if (orderEffect != "Undefined")
	{
		str += "\nEffect: " + orderEffect;
	}
	return str;
}

string OrderList::to_string()
{
	string str = "";
	for (std::shared_ptr<Order> order : this->orders) 
	{
		str += order->getOrderTypeString() + "\n";
	}
	return str;
}

//Stream insertion operator
ostream& operator<<(ostream& strm, Order& order)
{
	return strm << order.to_string();
}

ostream& operator<<(ostream& strm, OrderList &orderList)
{
	return strm << orderList.to_string();
}

//Since stream insertion is inherited, no need to implement for Order subclasses
