#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 
#include <iterator> 
using namespace std;

enum order
{
	BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY, UNDEFINED
};

class Card
{
public:
	Card();

	~Card();

	//set card type of enum
	void setCardType(order o);

	//get card type returns a string based off enum
	string getCardType();

	string to_string();

	Card(const Card&);

	Card& operator=(const Card&);

	friend ostream& operator<<(ostream&, Card&);


private:
	order cardType;


};

//deck class inherit the card class
class Deck :public Card {

public:

	Deck();

	~Deck();

	//initialize the Deck depending on the number of players *for now the formula is numplayers * 10 **
	void initializeDeck(int numPlayers);

	//print the size of Deck
	void printDeckSize();

	//returns the top card of the Deck to the player's hand
	Card* draw();

	//add card into Deck
	void addCard(Card* oneCard);

	string to_string();

	Deck(const Deck&);

	Deck& operator=(const Deck&);

	friend ostream& operator<<(ostream&, Deck&);

private:
	vector<Card*> deck;
	
};


class Hand : public Deck {

public:
	Hand();

	~Hand();

	//add card to player's hand
	void addCard(Card* a_card);

	//when playing a card, at the moment, simply returns the card to the deck,and deletes from hand
	void play(Card* aCard, Deck* aDeck);

	//returns the list of cards in player's hand
	vector<Card*>* getHand();

	//shows all cards to played this turn
	void printPlayCards();

	//this vector holds all cards to be played this turn
	vector<Card*>* getPlayCards();

	//return a card to the deck cards
	void returnPlayedCardToDeck(Deck* aDeck);

	//remove a card from the player's hand
	void removePlayedCardOfHand(Card* r_card);

	int getHandSize();

	string to_string();

	Hand(const Hand&);

	Hand& operator=(const Hand&);

	friend ostream& operator<<(ostream&, Hand&);

private:
	vector<Card*> hand;//for storing cards in player hand

	vector<Card*> playCards;//for storing cards to be played this turn

};