#include "GameEngine.h";
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

GameEngine::GameEngine() {
    numOfPlayers = 0;
    Deck deckCards;
    activateObservers = true;
}

bool GameEngine::equals(const string& a, const string& b) {
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

void GameEngine::gameStart()
{
    string map;
    string mapName = selectMap();
    while (mapName.compare("") == 0) {
        cout << "The map that you've selected does not exist in this current directory. You will be asked to select another one." << endl;
        mapName = selectMap();
    }

    //declaring map loader 
    shared_ptr<MapLoader> mapLoader(new MapLoader());

    //implement vector of maps, player can choose one
    vector <shared_ptr<Map>> listOfMaps;

            shared_ptr<Map> loadedMap = mapLoader->createMap(mapName);
            if (loadedMap != nullptr)
            {
                listOfMaps.push_back(loadedMap);
            }
            if (listOfMaps.size() > 0)
            {
                cout << "\nPrinting Maps:\n" << endl;
                for (int i = 0; i < listOfMaps.size(); i++)
                {
                    cout << listOfMaps[i]->to_string() << "\n" << endl;
                }
            }
   
    mapLoader.reset();
    mapLoader = nullptr;



    //setplayers
    setNumOfPlayers();

    activateObservers = Observers();

    cout << endl;
    for (int i = 0; i < numOfPlayers; i++) {
        string name;
        cout << "Enter the name for player #" << (i + 1) << ": ";
        cin >> name;
        Player newPlayer(name);
        players.push_back(newPlayer);
    }
    cout << endl;

    //initialize a card deck
    cout << "<Initial Card Deck Status>" << endl;
    Deck deck;
    deck.initializeDeck(getNumOfPlayers());
    cout << deck << endl;

}

string GameEngine::selectMap() {
    string map;
    cout << "What map would you like to play with ?: ";
    cin >> map;
    if (isMapInDirectory(map + ".map"))
        return map + ".map";
    else
        return "";
}

bool GameEngine::isMapInDirectory(string fileName) {
    ifstream file("MapDirectory/" + fileName);
    if (!file)
        return false;
    else
        return true;
}

void GameEngine::setNumOfPlayers()
{
    int count = 0;
    cout << "Select the number of players that will be participation (Must be between 2 and 5): ";
    cin >> count;
    while (!(count >= 2 && count <= 5)) {
        cout << "The numbers of players that you've selected has been deemed invalid. Please pick again: ";
        cin >> count;
    }
    numOfPlayers = count;
}

bool GameEngine::Observers() {
    string answer;
    bool loopAgain;
    cout << "activate the observers for this game? (Yes or No): ";
    cin >> answer;
    while (!equals(answer, "yes") && !equals(answer, "no")) {
        cout << "Your answer has been deeemd invalid. Please enter again: ";
        cin >> answer;
    }
    if (equals(answer, "yes"))
        return true;
    else if (equals(answer, "no"))
        return false;
    return false;
}

int GameEngine::getNumOfPlayers()
{
    return numOfPlayers;
}

vector<Player> GameEngine::getPlayersList() {
    return players;
}

bool GameEngine::getObserverStatus() {
    return activateObservers;
}

void GameEngine::setObserverStatus(bool status) {
    activateObservers = status;
}

Deck GameEngine::getDeckCards() {
    return deckCards;
}
