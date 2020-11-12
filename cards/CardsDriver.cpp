#include "Cards.h"
#include <time.h>



int main() {

	srand(time(0));
	//Fill deck with cards example
	Deck Deck1;
	Deck1.initializeDeck(4);
	cout << Deck1;

	//draw() method example
	Hand aHandCard;
	Hand bHandCard;
	Hand cHandCard;
	Hand dHandCard;
	Deck1.printDeckSize();
	cout << " test to assign 5 random cards into hand \n" << endl;
	for (int i = 0; i < 5; i++) {
		aHandCard.addCard(Deck1.draw());
		bHandCard.addCard(Deck1.draw());
		cHandCard.addCard(Deck1.draw());
		dHandCard.addCard(Deck1.draw());
	}
	
	cout << aHandCard;
	cout << bHandCard;
	cout << cHandCard;
	cout << dHandCard;
	Deck1.printDeckSize();


	//test the play cards order of HandCard
	//traverse to set all HandCards will play
	cout << " test to play all Cards of this hand..put the cards into playCards vector \n" << endl;
	
	int handSize = aHandCard.getHand()->size();
	for (int k = 0; k < handSize ; k++) {
		aHandCard.play(aHandCard.getHand()->front(), &Deck1);
	}


	cout << aHandCard;

	Deck1.printDeckSize();

}