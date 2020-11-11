#include <iostream>
#include "GameEngine.h"
using namespace std;

int engine_main() {
    GameEngine ge;
    
    ge.gameStart();
    cout << "Number of players that were created: " << ge.getPlayersList().size() << endl;
    cout << "Size of deck: ";
    ge.getDeckCards().printDeckSize();
    cout << endl;

    // You set the initial observer status using a prompt from the GameStart() method
    /** After the user sets the status of the observers, we will change the status two times
     * to show the effectiveness of our methods**/
    if (ge.getObserverStatus()) {
        cout << "Observers are on" << endl;
        ge.setObserverStatus(false);
    }
    else {
        cout << "Observers are off" << endl;
        ge.setObserverStatus(true);
    }

    if (ge.getObserverStatus()) {
        cout << "Observers are on" << endl;
        ge.setObserverStatus(false);
    }
    else {
        cout << "Observers are off" << endl;
        ge.setObserverStatus(true);
    }

    if (ge.getObserverStatus())
        cout << "Observers are on" << endl;
    else
        cout << "Observers are off" << endl;
    return 0;
}