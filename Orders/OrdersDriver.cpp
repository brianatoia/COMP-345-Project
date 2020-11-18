#include "Orders.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"

int main()
{
    //Creating two players
    Player* player1(new Player("Berta"));   
    Player* player2(new Player("Rob"));

    //Creating a map
    shared_ptr<Map> testMap(new Map());

    //Creating two continents
    shared_ptr<Continent> c1 = testMap->add(Continent(1, "America", 1));
    shared_ptr<Continent> c2 = testMap->add(Continent(2, "Europe", 5));

    //Creating new territories of type ptr and adding them to the testMap
    shared_ptr<Territory> t1 = testMap->add(Territory(1, "Canada", 1));
    shared_ptr<Territory> t2 = testMap->add(Territory(2, "Iceland", 2));
    shared_ptr<Territory> t3 = testMap->add(Territory(3, "United Kingdom", 2));
    shared_ptr<Territory> t4 = testMap->add(Territory(4, "France", 2));
    shared_ptr<Territory> t5 = testMap->add(Territory(5, "Spain", 2));
    shared_ptr<Territory> t6 = testMap->add(Territory(6, "Italy", 2));

    //Connecting territories via directed graphs
    testMap->link(t1, t2);
    testMap->link(t2, t3);
    testMap->link(t3, t4);
    testMap->link(t4, t5);
    testMap->link(t4, t1);
    testMap->link(t5, t6);
    testMap->link(t6, t3);
    testMap->link(t6, t4);

    //Adding two territories to players
    player1->addTerritory(t1);
    player1->addTerritory(t4);

    player2->addTerritory(t3);
    player2->addTerritory(t5);

    //Creating Deck
    Deck* deck = new Deck();
    deck->initializeDeck(Player::getPlayerCount());


    cout << "PLAYER1: " << endl;
    for (shared_ptr<Territory> t : *player1->getTerritoryList())
    {
        cout << t->name << endl;
    }    
    //Number of Cards in Player1 hand
    cout << "Player 1 Cards: " << player1->getHand()->getHandSize() << endl;
    cout << "Player 2 Cards: " << player2->getHand()->getHandSize() << endl;

    cout << "\nPLAYER2: " << endl;
    for (shared_ptr<Territory> t : *player2->getTerritoryList())
    {
        cout << t->name << endl;
    }
    
    //Each order is validated before being executed
    

    cout << "\nTESTING DEPLOY" << endl;

    //Invalid Deploy - Deploying to a territory you do not own
    shared_ptr<Order> order1(new Deploy(10, t5, *player1->getTerritoryList()));
    order1->execute();
    cout << "\n" << *order1 << endl;

    //Valid Deploy - Deploying to a territory you do own
    shared_ptr<Order> order2(new Deploy(10, t4, *player1->getTerritoryList()));
    order2->execute();
    cout << "\n" << *order2 << endl;
    
    shared_ptr<Order> order3(new Deploy(10, t3, *player2->getTerritoryList()));
    order3->execute();
    cout << "\n" << *order3<< endl;


    
    cout << "\nTESTING ADVANCE" << endl;

    //Invalid Advance - If the source territory does not belong to the player that issued the order
    shared_ptr<Order> order4(new Advance(5, t5, t4, player1->getTerritoryList(), Player::getPlayerTerritories(t4->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order4->execute();
    cout << "\n" << *order4 << endl;

    //Invalid Advance - If the target territory is not adjacent to the source territory
    shared_ptr<Order> order5(new Advance(5, t4, t6, player1->getTerritoryList(), Player::getPlayerTerritories(t6->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order5->execute();
    cout << "\n" << *order5 << endl;

    //If the source and target territory both belong to the player that issued the order, the army units are moved from the source to the target territory.
    shared_ptr<Order> order6(new Advance(2, t4, t1, player1->getTerritoryList(), Player::getPlayerTerritories(t1->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order6->execute();
    cout << "\n" << *order6 << endl;

    //If the source belongs to the player but the target does not
    shared_ptr<Order> order7(new Advance(2, t4, t5, player1->getTerritoryList(), Player::getPlayerTerritories(t5->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order7->execute();
    cout << "\n" << *order7 << endl;

    //If the source belongs to the player, but the target territory belongs to the Neutral player
    shared_ptr<Order> order8(new Advance(1, t5, t6, player1->getTerritoryList(), Player::getPlayerTerritories(t5->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order8->execute();
    cout << "\n" << *order8 << endl;

    cout << "\nPLAYER1: " << endl;
    for (shared_ptr<Territory> t : *player1->getTerritoryList())
    {
        cout << t->name + " units: "<< t->units << endl;
    }

    cout << "\nPLAYER2: " << endl;
    for (shared_ptr<Territory> t : *player2->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }



    cout << "\nTESTING BLOCKADE" << endl;
    
    //Invalid Blockade - If the target territory belongs to an enemy player
    shared_ptr<Order> order9(new Blockade(t2, *player1->getTerritoryList()));
    order9->execute();
    cout << "\n" << *order9 << endl;
    
    //Invalid Blockade - If the target territory belongs to an enemy player
    shared_ptr<Order> order10(new Blockade(t6, *player1->getTerritoryList()));
    order10->execute();
    cout << "\n" << *order10 << endl;

    cout << "\nPLAYER1: " << endl;
    for (shared_ptr<Territory> t : *player1->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }



    cout << "\nTESTING AIRLIFT" << endl;

    //Invalid Airlift - If the source or target does not belong to the player that issued the order
    shared_ptr<Order> order11(new Airlift(2, t3, t5, player1->getTerritoryList(), Player::getPlayerTerritories(t5->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order11->execute();
    cout << "\n" << *order11 << endl;

    //Valid Airlift - If both the source and target territories belong to the player that issue the airlift order
    shared_ptr<Order> order12(new Airlift(2, t4, t5, player1->getTerritoryList(), Player::getPlayerTerritories(t5->ownerID), player1->getCapturedTerritory(), &playersNegotiated));
    order12->execute();
    cout << "\n" << *order12 << endl;

    //If the target territory does not belong to the player that issued the airlift order, the selected number of armies is attacking that territory(see “advance order”)
    shared_ptr<Order> order13(new Airlift(3, t3, t4, player2->getTerritoryList(), Player::getPlayerTerritories(t4->ownerID), player2->getCapturedTerritory(), &playersNegotiated));
    order13->execute();
    cout << "\n" << *order13 << endl;

    cout << "\nPLAYER1: " << endl;
    for (shared_ptr<Territory> t : *player1->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }

    cout << "\nPLAYER2: " << endl;
    for (shared_ptr<Territory> t : *player2->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }



    cout << "\nTESTING BOMB" << endl;

    //Invalid Bomb - Cannot bomb your own territory
    shared_ptr<Order> order14(new Bomb(player2->getPlayerID(), t3, player2->getTerritoryList(), &playersNegotiated));
    order14->execute();
    cout << "\n" << *order14 << endl;

    //Valid Bomb - Bomb territory you do not own
    shared_ptr<Order> order15(new Bomb(player1->getPlayerID(), t3, player1->getTerritoryList(), &playersNegotiated));
    order15->execute();
    cout << "\n" << *order15 << endl;

    cout << "\nPLAYER1: " << endl;
    for (shared_ptr<Territory> t : *player1->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }

    cout << "\nPLAYER2: " << endl;
    for (shared_ptr<Territory> t : *player2->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }



    cout << "\nTESTING NEGOTIATE" << endl;
    
    //Invalid Negotiate - Cannot negotiate with yourself
    shared_ptr<Order> order16(new Negotiate(player1->getPlayerID(), player1->getPlayerID(), &playersNegotiated));
    order16->execute();
    cout << "\n" << *order16 << endl;

    //Valid Negotiate
    shared_ptr<Order> order17(new Negotiate(player1->getPlayerID(), player2->getPlayerID(), &playersNegotiated));
    order17->execute();
    cout << "\n" << *order17 << endl;

    //Test Negotiate
    shared_ptr<Order> order18(new Advance(4, t3, t4, player2->getTerritoryList(), Player::getPlayerTerritories(t4->ownerID), player2->getCapturedTerritory(), &playersNegotiated));
    order18->execute();
    cout << "\n" << *order18 << endl;

    cout << "\nPLAYER1: " << endl;
    for (shared_ptr<Territory> t : *player1->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }

    cout << "\nPLAYER2: " << endl;
    for (shared_ptr<Territory> t : *player2->getTerritoryList())
    {
        cout << t->name + " units: " << t->units << endl;
    }

    if (player1->getCapturedTerritory())
    {
        player1->getHand()->addCard(deck->draw());
    }
    if (player2->getCapturedTerritory())
    {
        player2->getHand()->addCard(deck->draw());
    }

    cout << "Player 1 Cards: " << player1->getHand()->getHandSize() << endl;
    cout << "Player 2 Cards: " << player2->getHand()->getHandSize() << endl;

    GameEngine* game(new GameEngine());
    game->executeOrdersPhase();

	return 0;
}