#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> //for sort, union, intersection method
#include <iterator> 
using namespace std;

class Card
{
public:
	Card();
	~Card();

	//initialize the type vec
	void initializeVecCardType();

	//get pointer to the type vec
	vector<string>* getPtrCardTypeArr();

	//for test to print vec_types_card
	void printVecCardType();

	//set card type id, from 0-4
	void setCardTypeId(int num);
	//get card type 
	string getCardType();

	string to_string();

	Card(const Card&);
	Card& operator=(const Card&);
	friend ostream& operator<<(ostream&, Card&);


private:
	//the vector stores the 5 types of card, 0->boomb, 1->reinforcement, 2->blockade, 3->airlift, 4->displomacy
	vector<string> vecCardType;		//MAKE AN ENUM
	string cardType;//for card type
	

};

//deck class inherit the card class
class Deck :public Card {

public:

	Deck();
	~Deck();

	//initialize the deck vec, assign 30 cards in it, each type has 5 cards.
	void initializeVecDeck(int numPlayers);

	//test to print the all card of initial vec_deck 
	void printVecDeck();
	
	//print the size of vec_dec
	void printVecDeckSize();

	//draw method is to allow a player to draw a card at random from the cards remaining in the deck
	//return a Card
	Card* draw();

	//the method is to return played cards into Deck_vec
	void addCard(Card* oneCard);

	//get the current vec_deck
	//vector<Card*>* get_current_vec_deck();
	string to_string();
	Deck(const Deck&);
	Deck& operator=(const Deck&);
	friend ostream& operator<<(ostream&, Deck&);

private:
	vector<Card*> vecDeck;
	Card* ptrCard;//pointer to card
	Card* tempCard;//for draw and remove the element of vector
	
};

//HandCards class inherit Card class and Deck Class (convinent to change deck)
class Hand : public Deck {

public:
	Hand();
	~Hand();

	//set vec hand cards
	void addCard(Card* a_card);
	
	//test to print the vec_hand_cards
	void printVecHand();

	//play() method that is called to play the card
	//creates an order and add it to the player's list, then return the card to the deck.
	void play(Card* aCard, Deck* aDeck);

	//get pointer to vec_hand_cards
	vector<Card*>* getVecHand();

	//test to print the vec_play_cards
	void printVecPlayCards();

	//get pointer to vec_play_cards
	vector<Card*>* getVecPlayCards();

	//clear play_cards after player played cards
	void clearPlayCards();

	//return a card to the deck cards
	void returnPlayedCardToDeck(Deck* aDeck);

	//remove the a played card from hand_cards
	void removePlayedCardOfHand(Card* r_card);
	
	//remove all played cards of handcards
	void removeAllPlayedCardsOfHand();

	int getVecHandSize();

	string to_string();
	Hand(const Hand&);
	Hand& operator=(const Hand&);
	friend ostream& operator<<(ostream&, Hand&);

private:
	vector<Card*> vecHand;//for storing hand cards
	vector<Card*> vecPlayCards;//for store play cards

};