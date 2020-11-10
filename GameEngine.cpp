#include "GameEngine.h";
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;


void GameEngine::gameStart()
{
   
  

    //maploader

    //declaring map loader 
    shared_ptr<MapLoader> mapLoader(new MapLoader());

    //implement vector of maps, player can choose one
    vector <shared_ptr<Map>> listOfMaps;

    while (true)
    {
        string userInput;
        cout << "Please enter name of the map (with .map or .txt) you would like to load and hit enter." << endl
            << "If you are done selecting maps, enter 1\n" << endl;
        cin >> userInput;
        if (userInput == "1") {
            break;
        }
        else
        {
            shared_ptr<Map> loadedMap = mapLoader->createMap(userInput);
            if (loadedMap != nullptr)
            {
                listOfMaps.push_back(loadedMap);
            }
        }
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
    ifstream file(fileName.c_str());
    return file.good();
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


Map GameEngine::getMap() {
    std::vector<std::string> maps;
    std::cout << "Let's Pick a map!\n\n";
    try {
        for (auto& p : std::filesystem::directory_iterator("mapdir"))
            maps.push_back(p.path().string());
    }
    catch (int e) { std::cout << "I/O error"; }

    //need to remove the first 8 characters to show only map and not path when asking for choice
    for (int i = 0; i < maps.size(); i++)
    {
        std::cout << "Map #" << i << " -> " << maps[i].substr(8) << "\n";
    }
    std::cout << "Please Enter the number of the map which you would like to select: ";
    int map_number;
    std::cin >> map_number;
    std::string selected_map = maps[map_number];

    //empty vectors to avoid memory leak
    maps.clear();

    return selected_map;
}


int main() {

    GameEngine firstGame;
    firstGame.gameStart();

}