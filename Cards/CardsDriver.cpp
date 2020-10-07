#include "Cards.h"



int main() {

	//Fill deck with cards example
	Deck Deck1;
	Deck1.initialize_vec_deck();
	cout << Deck1;

	//draw() method example
	Hand aHandCard;
	Hand bHandCard;
	Hand cHandCard;
	Hand dHandCard;
	Deck1.print_vec_deck_size();
	cout << " test to assign 5 random cards into hand \n" << endl;
	for (int i = 0; i < 5; i++) {
		aHandCard.set_vec_hand(Deck1.draw());
		bHandCard.set_vec_hand(Deck1.draw());
		cHandCard.set_vec_hand(Deck1.draw());
		dHandCard.set_vec_hand(Deck1.draw());
	}
	
	cout << aHandCard;
	cout << bHandCard;
	cout << cHandCard;
	cout << dHandCard;
	Deck1.print_vec_deck_size();


	//test the play cards order of HandCard
	//traverse to set all HandCards will play
	cout << " test to play all Cards of the HandCards...put the cards into vec_play_cards \n" << endl;
	for (int k = 0; k < aHandCard.get_vec_hand()->size(); k++) {
		aHandCard.play(aHandCard.get_vec_hand()->at(k));
	}

	//print the vec_play_cards
	aHandCard.print_vec_play_cards();

	//test return played cards to deck
	aHandCard.return_played_card_to_deck(&Deck1);

	//test remove all played cards of hand cards
	aHandCard.remove_all_played_cards_of_hand();

	//then clear the play cards of vec_play_cards. Notice: clean play_cards at end.
	aHandCard.clear_play_cards();

	//print the vec_play_cards
	aHandCard.print_vec_play_cards();

	//print the current hand cards
	aHandCard.print_vec_hand();

	//test the size of current deck
	Deck1.print_vec_deck_size();

}