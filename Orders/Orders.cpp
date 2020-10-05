#include "Orders.h";


//Order Constructors
Order::Order() 
{
	orderType = "Undefined";
	orderDescription = "Undefined";
	orderEffect = "";
}

Order::Order(string orderType)
{
	this->orderType = orderType;
	this->orderEffect = "";
	if (orderType == "Deploy")
	{
		this->orderDescription = "Place some armies on one of the current player's territories.";
	}
	else if (orderType == "Advance")
	{
		this->orderDescription = "Move some armies from one of the current player's territories (source) to an adjacent territory " 
		"(target). If the target territory belongs to the current player, the armies are moved to the target " 
			"territory. If the target territory belongs to another player, an attack happens between the two territories.";
	}
	else if (orderType == "Bomb")
	{
		this->orderDescription = "Destroy half of the armies located on an opponent's territory that is adjacent to one of the current " 
			"player's territories.";
	}
	else if (orderType == "Blockade")
	{
		this->orderDescription = "Triple the number of armies on one of the current player's territories and make it a neutral territory.";
	}
	else if (orderType == "Airlift")
	{
		this->orderDescription = "Advance some armies from one of the current player's territories to any another territory.";
	}
	else if (orderDescription == "Negotiate")
	{
		this->orderDescription = "Prevent attacks between the current player and another player until the end of the turn.";
	}
	else
	{
		cerr << "Invalid Order Type" << endl;
	}
}

//Order Getters
string Order::getOrderType()
{
	return orderType;
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
void Order::setOrderType(string orderType)
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

void Order::execute(){} //inherited & implemented in subclasses


//OrderList Default Constructor
OrderList::OrderList()
{
	orders.empty();
}

//OrderList Destructor
OrderList::~OrderList()
{
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
void OrderList::remove(shared_ptr<Order> order)
{ 
	list<shared_ptr<Order>>::iterator it = orders.begin();
	for (it = orders.begin(); it != orders.end(); advance(it, 1))
	{
		if (*it == order)
		{
			orders.remove(order);
			return;
		}
	}
}

//Moves an Order in the List: up, down, top or bottom
void OrderList::move(shared_ptr<Order> order, int moveOption)
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
	if (moveOption == 1)
	{
		orders.splice(orders.begin(), orders, it);
	}
	else if(moveOption == 2)
	{
		orders.splice(orders.end(), orders, it);
	}
	else if (moveOption == 3)
	{
		//check that it's not the first element
		if (it != orders.begin())
		{
			list<shared_ptr<Order>>::iterator itUp = it;
			advance(itUp, -1);
			orders.splice(itUp, orders, it);
		}
	}
	else if (moveOption == 4)
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

Deploy::Deploy() : Order("Deploy")
{
	//initialize attributes needed to perform this action
	//to be implemented 
}

bool Deploy::validate()
{
	//VALIDATE ACTION 
	cout << "Validating Deploy Order..." << endl;
	//to be implemented
	return true;
}

void Deploy::execute()
{
	if (validate())
	{
		//EXECUTE ACTION
		//to be implemented
		cout << "Deploying..." << endl;
		setOrderEffect("Effect of Deploying");
	}
}

Advance::Advance() : Order("Advance")
{
	//initialize attributes needed to perform this action
	//to be implemented 
}

bool Advance::validate()
{
	//VALIDATE ACTION 
	cout << "Validating Advance Order..." << endl;
	//to be implemented
	return true;
}

void Advance::execute()
{
	if (validate())
	{
		//EXECUTE ACTION
		//to be implemented
		cout << "Advancing..." << endl;
		setOrderEffect("Effect of Advancing");
	}
}

Bomb::Bomb() : Order("Bomb")
{
	//initialize attributes needed to perform this action
	//to be implemented 
}

bool Bomb::validate()
{
	//VALIDATE ACTION
	cout << "Validating Bomb Order..." << endl;
	//to be implemented
	return true;
}

void Bomb::execute()
{
	if (validate())
	{
		//EXECUTE ACTION
		//to be implemented
		cout << "Bombing..." << endl;
		setOrderEffect("Effect of Bombing");
	}
}

Blockade::Blockade() : Order("Blockade")
{
	//initialize attributes needed to perform this action
	//to be implemented
}

bool Blockade::validate()
{
	//VALIDATE ACTION 
	cout << "Validating Blockade Order..." << endl;
	//to be implemented
	return true;
}

void Blockade::execute()
{
	if (validate())
	{
		//EXECUTE ACTION
		//to be implemented
		cout << "Blockading..." << endl;
		setOrderEffect("Effect of Blockading");
	}
}

Airlift::Airlift() : Order("Airlift")
{
	//initialize attributes needed to perform this action
	//to be implemented
}

bool Airlift::validate()
{
	//VALIDATE ACTION 
	cout << "Validating Airlift Order..." << endl;
	//to be implemented
	return true;
}

void Airlift::execute()
{
	if (validate())
	{
		//EXECUTE ACTION
		//to be implemented
		cout << "Airlifting..." << endl;
		setOrderEffect("Effect of Airlifting");
	}

}

Negotiate::Negotiate() : Order("Negotiate")
{
	//initialize attributes needed to perform this action
	//to be implemented
}

bool Negotiate::validate()
{
	//VALIDATE ACTION 
	cout << "Validating Negotiate Order..." << endl;
	//to be implemented
	return true;
}

void Negotiate::execute()
{
	if (validate())
	{
		//EXECUTE ACTION
		//to be implemented
		cout << "Negotiating..." << endl;
		setOrderEffect("Effect of Negotiating");
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
	this->orders = orderList.orders;
}

Deploy::Deploy(const Deploy& order) : Order(order)
{
	//future attributes
}

Advance::Advance(const Advance& order) : Order(order)
{
	//future attributes
}

Bomb::Bomb(const Bomb& order) : Order(order)
{
	//future attributes
}

Blockade::Blockade(const Blockade& order) : Order(order)
{
	//future attributes
}

Airlift::Airlift(const Airlift& order) : Order(order)
{
	//future attrubutes
}

Negotiate::Negotiate(const Negotiate& order) : Order(order)
{
	//future attributes
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
	this->orders = rightSide.orders;
	return *this;
}

Deploy& Deploy::operator=(const Deploy& rightSide) 
{
	Order::operator=(rightSide);
	//future attibutes
	return *this;
}

Advance& Advance::operator=(const Advance& rightSide)
{
	Order::operator=(rightSide);
	//future attibutes
	return *this;
}

Bomb& Bomb::operator=(const Bomb& rightSide)
{
	Order::operator=(rightSide);
	//future attibutes
	return *this;
}

Blockade& Blockade::operator=(const Blockade& rightSide)
{
	Order::operator=(rightSide);
	//future attibutes
	return *this;
}

Airlift& Airlift::operator=(const Airlift& rightSide)
{
	Order::operator=(rightSide);
	//future attibutes
	return *this;
}

Negotiate& Negotiate::operator=(const Negotiate& rightSide)
{
	Order::operator=(rightSide);
	//future attibutes
	return *this;
}

//to_String functions
string Order::to_string()
{
	string str = "Order: " + orderType + "\nDescription: " + orderDescription;
	if (orderEffect != "")
	{
		str += "\nEffect: " + orderEffect;
	}
	return str;
}

string OrderList::to_string()
{
	string str = "";
	list<shared_ptr<Order>>::iterator it = orders.begin();
	for (it = orders.begin(); it != orders.end(); advance(it, 1))
	{
		shared_ptr<Order> o = *it;
		str += o->getOrderType() + "\n";
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
