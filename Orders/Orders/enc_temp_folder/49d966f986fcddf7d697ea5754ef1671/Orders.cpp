#include "Orders.h"
#include <random>
#include <tuple>


//Order Constructors
Order::Order()
{
	orderType = Undefined;
	orderDescription = "Undefined";
	orderEffect = "Undefined";
}

Order::Order(OrderType orderType)
{
	this->orderType = orderType;
	this->orderEffect = "Undefined";
	if (orderType == Deploy)
	{
		this->orderDescription = "Place some armies on one of the current player's territories.";
	}
	else if (orderType == Advance)
	{
		this->orderDescription = "Move some armies from one of the current player's territories (source) to an adjacent territory " 
		"(target). If the target territory belongs to the current player, the armies are moved to the target " 
			"territory. If the target territory belongs to another player, an attack happens between the two territories.";
	}
	else if (orderType == Bomb)
	{
		this->orderDescription = "Destroy half of the armies located on an opponent's territory that is adjacent to one of the current " 
			"player's territories.";
	}
	else if (orderType == Blockade)
	{
		this->orderDescription = "Double the number of armies on one of the current player's territories and make it a neutral territory.";
	}
	else if (orderType == Airlift)
	{
		this->orderDescription = "Advance some armies from one of the current player's territories to any another territory.";
	}
	else if (orderType == Negotiate)
	{
		this->orderDescription = "Prevent attacks between the current player and another player until the end of the turn.";
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}

//Order Getters
Order::OrderType Order::getOrderType()
{
	return orderType;
}

string Order::getOrderTypeString()
{
	switch (this->orderType)
		{
			case Deploy: return "Deploy";
			case Advance: return "Advance";
			case Bomb: return "Bomb";
			case Blockade: return "Blockade";
			case Airlift: return "Airlift";
			case Negotiate: return "Negotiate";
			case Undefined: return "Undefined";
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
void Order::setOrderType(OrderType orderType)
{
	this->orderType = orderType;
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
	for (std::shared_ptr<Order> order : this->orders) 
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
void OrderList::move(shared_ptr<Order> order, MoveOption moveOption)
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
	if (moveOption == moveToBeginning)
	{
		orders.splice(orders.begin(), orders, it);
	}
	//Move order to end of list
	else if(moveOption == moveToEnd)
	{
		orders.splice(orders.end(), orders, it);
	}
	//Move order up one position
	else if (moveOption == moveUp)
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
	else if (moveOption == moveDown)
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

Deploy::Deploy(int numOfArmies, shared_ptr<Territory> territory, list<shared_ptr<Territory>> playerTerritories) : Order(Order::Deploy)
{
	this->numOfArmies = numOfArmies;
	this->territory = territory;
	this->playerTerritories = playerTerritories;
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

	cerr << "Invalid Order. Cannot deploy to a territory you do not control." << endl;
	
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

Advance::Advance(int numOfArmies, shared_ptr<Territory> sourceTerritory, shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>> playerTerritories, bool* capturedTerritory, 
								list<tuple<int, int>> playersNegotiated) : Order(Order::Advance)
{
	this->numOfArmies = numOfArmies;
	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;
	this->playerTerritories = playerTerritories;
	this->capturedTerritory = capturedTerritory;
	this->playersNegotiated = playersNegotiated;
}

bool Advance::validate()
{
	bool playerOwnsSource = false;
	bool sourceBordersTarget = false;

	for (shared_ptr<Territory> t : playerTerritories)
	{
		if (t == sourceTerritory)
		{
			playerOwnsSource = true;
			break;
		}
	}
	if (!playerOwnsSource)
	{
		cerr << "Invalid Order. Source territory does not belong to player that issued order." << endl;
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
		cerr << "Invalid Order. Source territory does not border target territory." << endl;
		return false;
	}

	return true;
}

string attack(int numOfArmies, shared_ptr<Territory> sourceTerritory, shared_ptr<Territory> targetTerritory, bool* capturedTerritory)
{
	string s = "";
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, 1);

	int sourceArmiesAttacking = 0, targetArmiesDefending = 0;
	for (int i = 0; i < numOfArmies; i++)
	{
		float chance = dist(mt);
		if (chance <= 0.6)
		{
			sourceArmiesAttacking += 1;
		}
	}

	for (int i = 0; i < targetTerritory->units; i++)
	{
		float chance = dist(mt);
		if (chance <= 0.7)
		{
			targetArmiesDefending += 1;
		}
	}

	targetTerritory->units -= sourceArmiesAttacking;
	sourceTerritory->units -= targetArmiesDefending;
	numOfArmies -= targetArmiesDefending;
	
	//Defending army defeated all of Attacking territoy armies
	if (sourceTerritory->units <= 0)
	{
		sourceTerritory->units = 0; //make sure it's 0 and not negative
		sourceTerritory->ownerID = NULL;

		s += sourceTerritory->name + " has no armies remaining. Player " + std::to_string(sourceTerritory->ownerID) + "no longer owns this territory.\n";
	}
	if (targetTerritory->units <= 0)
	{
		cout << "HI" << endl;
		targetTerritory->units = 0; //make sure it's 0 and not negative
		targetTerritory->ownerID = NULL;

		s += targetTerritory->name + " has been defeated.\n";

		if (sourceTerritory->units != 0)
		{
			cout << numOfArmies << endl;
			targetTerritory->units = numOfArmies;
			targetTerritory->ownerID = sourceTerritory->ownerID;
			cout << targetTerritory->units << endl;

			s += "Player " + std::to_string(sourceTerritory->ownerID) + " now has " + std::to_string(numOfArmies) + " armies in " + targetTerritory->name;
			*capturedTerritory = true;
		}

	}
	return(s);
}

void Advance::execute()
{
	if (validate())
	{
		string s;
		//if target territory does not have an owner, can take it 
		if (targetTerritory->ownerID == NULL)
		{
			targetTerritory->ownerID = sourceTerritory->ownerID;
			targetTerritory->units += numOfArmies;
			sourceTerritory->units -= numOfArmies;
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
			for (tuple<int, int> t : playersNegotiated)
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
				s = attack(numOfArmies, sourceTerritory, targetTerritory, capturedTerritory);
			}
		}
		setOrderEffect(s);
	}
}

Bomb::Bomb(shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>> playerTerritories) : Order(Order::Bomb)
{
	this->targetTerritory = targetTerritory;
	this->playerTerritories = playerTerritories;
}

bool Bomb::validate()
{
	for (shared_ptr<Territory> t : playerTerritories)
	{
		if (t == targetTerritory)
		{
			cerr << "Cannot bomb own territory." << endl;
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

Blockade::Blockade(shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>>& playerTerritories) : Order(Order::Blockade)
{
	this->targetTerritory = targetTerritory;
	this->playerTerritories = &playerTerritories;
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

	cerr << "Cannot blockade a territory player does not own." << endl;
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

Airlift::Airlift(int numOfArmies, shared_ptr<Territory> sourceTerritory, shared_ptr<Territory> targetTerritory, list<shared_ptr<Territory>> playerTerritories, bool* capturedTerritory,
						list<tuple<int, int>> playersNegotiated) : Order(Order::Airlift)
{
	this->numOfArmies = numOfArmies;
	this->sourceTerritory = sourceTerritory;
	this->targetTerritory = targetTerritory;
	this->playerTerritories = playerTerritories;
	this->capturedTerritory = capturedTerritory;
	this->playersNegotiated = playersNegotiated;
}

bool Airlift::validate()
{
	bool playerOwnsSource = false;
	bool playerOwnsTarget = false;

	for (shared_ptr<Territory> t : playerTerritories)
	{
		if (t == sourceTerritory)
		{
			playerOwnsSource = true;
			break;
		}
		if (t == targetTerritory)
		{
			playerOwnsTarget = true;
			break;
		}
	}
	if (!playerOwnsSource)
	{
		cerr << "Invalid Order. Player does not own source territory." << endl;
		return false;
	}
	if (!playerOwnsTarget)
	{
		cerr << "Invalid Order. Player does not own target territory." << endl;
		if (playerOwnsSource)
		{
			cerr << "Issuing Attack..." << endl;
			string s;
			bool negotiatedWith = false;
			for (tuple<int, int> t : playersNegotiated)
			{
				cout << get<0>(t) << endl;
				cout << get<1>(t) << endl;
				if (t == make_tuple(sourceTerritory->ownerID, targetTerritory->ownerID) || t == make_tuple(targetTerritory->ownerID, sourceTerritory->ownerID))
				{
					cout << "HI" << endl;
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
				s = attack(numOfArmies, sourceTerritory, targetTerritory, capturedTerritory);
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

Negotiate::Negotiate(int sourcePlayerID, int targetPlayerID, list<tuple<int, int>> playersNegotiated) : Order(Order::Negotiate)
{
	this->sourcePlayerID = sourcePlayerID;
	this->targetPlayerID = targetPlayerID;
	this->playerseNegotiated = playerseNegotiated;
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
		playerseNegotiated.push_back(std::make_tuple(sourcePlayerID, targetPlayerID));
				
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
	this->capturedTerritory = order.capturedTerritory;
	this->playersNegotiated = order.playersNegotiated;
}

Bomb::Bomb(const Bomb& order) : Order(order)
{
	this->targetTerritory = order.targetTerritory;
	this->playerTerritories = order.playerTerritories;
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
	this->capturedTerritory = order.capturedTerritory;
	this->playersNegotiated = order.playersNegotiated;
}

Negotiate::Negotiate(const Negotiate& order) : Order(order)
{
	this->sourcePlayerID = order.sourcePlayerID;
	this->targetPlayerID = order.targetPlayerID;
	this->playerseNegotiated = order.playerseNegotiated;
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
	this->capturedTerritory = rightSide.capturedTerritory;
	this->playersNegotiated = rightSide.playersNegotiated;
	return *this;
}

Bomb& Bomb::operator=(const Bomb& rightSide)
{
	Order::operator=(rightSide);
	this->targetTerritory = rightSide.targetTerritory;
	this->playerTerritories = rightSide.playerTerritories;
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
	this->capturedTerritory = rightSide.capturedTerritory;
	this->playersNegotiated = rightSide.playersNegotiated;
	return *this;
}

Negotiate& Negotiate::operator=(const Negotiate& rightSide)
{
	Order::operator=(rightSide);
	this->sourcePlayerID = rightSide.sourcePlayerID;
	this->targetPlayerID = rightSide.targetPlayerID;
	this->playerseNegotiated = rightSide.playerseNegotiated;
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
