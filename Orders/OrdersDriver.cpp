#include "Orders.h";

int main()
{
	shared_ptr<Order> order1 (new Deploy);
	shared_ptr<Order> order2 (new Advance);
	shared_ptr<Order> order3 (new Bomb);
	shared_ptr<Order> order4 (new Negotiate);
	shared_ptr<Order> order5 (new Order); //invalid order

	OrderList* orderList = new OrderList();

	//Adding to the order list
	orderList->addOrder(order1);
	orderList->addOrder(order2);
	orderList->addOrder(order3);
	orderList->addOrder(order4);
	orderList->addOrder(order5); //invalid order will not appear in list

	//Stream insertion example
	cout << "\nOrder List" << endl;
	cout << "------------------" << endl;
	cout << *orderList << endl;
	cout << "\nOrder Object" << endl;
	cout << "------------------" << endl;
	cout << *order5 << endl;
	cout << "\nOrder Subclass Object (Deploy)" << endl;
	cout << "------------------" << endl;
	cout << *order1 << endl;

	//Demonstrating remove
	cout << "\nDemonstrating remove()" << endl;
	cout << "------------------" << endl;
	orderList->remove(order2);
	orderList->remove(order5); //no change since not in list
	cout << *orderList << endl;

	//Demonstrating move
	cout << "Demonstrating move(): Move 'Negotiate' to the top" << endl;
	cout << "------------------" << endl;
	orderList->move(order4, 1);
	cout << *orderList << endl;

	cout << "Demonstrating move(): Move 'Negotiate' back to the bottom" << endl;
	cout << "------------------" << endl;
	orderList->move(order4, 2);
	cout << *orderList << endl;

	cout << "Demonstrating move(): Move 'Bomb' up" << endl;
	cout << "------------------" << endl;
	orderList->move(order3, 3);
	cout << *orderList << endl;
	
	cout << "Demonstrating move(): Move 'Bomb' down" << endl;
	cout << "------------------" << endl;
	orderList->move(order3, 4);
	cout << *orderList << endl;

	cout << "\nDemonstrating execute()" << endl;
	cout << "------------------" << endl;
	//Demonstrating execute() functions
	order1->execute();
	order3->execute();
	order4->execute();
	order5->execute(); //doesn't print since not valid 

	//Demonstrating assignment operator
	order5 = order1;
	order1 = order3;

	OrderList* orderList1 = new OrderList();
	cout << "\nNew Order List" << endl;
	cout << "------------------" << endl;
	cout << *orderList1 << endl;
	orderList1 = orderList;
	cout << "\nNew Order List after Assignment" << endl;
	cout << "------------------" << endl;
	cout << *orderList1 << endl;
	cout << "\nNew Order5" << endl;
	cout << "------------------" << endl;
	cout << *order5 << endl;
	cout << "\nNew Order1" << endl;
	cout << "------------------" << endl;
	cout << *order1 << endl;

	//Demonstrating copy constructor
	shared_ptr<Order> order6(new Order);
	shared_ptr<Order> order7 = order6;
	cout << "\nOrder Copy Constructor" << endl;
	cout << "------------------" << endl;
	cout << *order7 << endl;

	shared_ptr<Order> order8 = order5;
	cout << "\nOrder Subclass Copy Constructor" << endl;
	cout << "------------------" << endl;
	cout << *order8 << endl;

	OrderList* orderList2 = orderList;
	cout << "\nOrder List Copy Constructor" << endl;
	cout << "------------------" << endl;
	cout << *orderList2 << endl; 

	return 0;
}