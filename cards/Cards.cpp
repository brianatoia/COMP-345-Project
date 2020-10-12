#include "Cards.h"
#include <random>
#include <algorithm>
using namespace std;

Card::Card()
{
	//cout << " Card is starting...initial types..." << endl;
	initializeVecCardType();
}

void Card::initializeVecCardType()
{
	//assign types to array
	vecCardType = vector<string>();
	vecCardType.push_back("bomb");
	vecCardType.push_back("reinforcement");
	vecCardType.push_back("blockade");
	vecCardType.push_back("airlift");
	vecCardType.push_back("diplomacy");

}

void Card::printVecCardType()
{
	cout << "the arr_types_card contains:  " << endl;
	for (int i = 0; i < vecCardType.size(); i++) {
		cout << vecCardType.at(i) << endl;
	}
}

void Card::setCardTypeId(int num)
{
	cardType = vecCardType.at(num);
}

string Card::getCardType()
{
	return cardType;
}


Deck::Deck()
{
	this->ptrCard = new Card();
	this->tempCard = new Card();
}

Deck::~Deck()
{
	delete ptrCard;
	delete tempCard;
}

void Deck::initializeVecDeck(int numPlayers)
{

	int deckSize = (numPlayers * 10);
	int handSize = deckSize / numPlayers;

	for (int i = 0; i <= deckSize; i++) {
		ptrCard = new Card();
		if (i % handSize == 0 || i % handSize == 5) {
			ptrCard->setCardTypeId(0);
			vecDeck.push_back(ptrCard);
		}
		else if (i % handSize == 1 || i % handSize == 6) {
			ptrCard->setCardTypeId(1);
			vecDeck.push_back(ptrCard);
		}
		else if (i % handSize == 2 || i % handSize == 7) {
			ptrCard->setCardTypeId(2);
			vecDeck.push_back(ptrCard);
		}
		else if (i % handSize == 3 || i % handSize == 8) {
			ptrCard->setCardTypeId(3);
			vecDeck.push_back(ptrCard);
		}
		else if (i % handSize == 4 || i % handSize == 9) {
			ptrCard->setCardTypeId(4);
			vecDeck.push_back(ptrCard);
		}

	}

	random_shuffle(vecDeck.begin(), vecDeck.end());
}

void Deck::printVecDeckSize()
{
	cout << "\n The current vecDeck contains " << vecDeck.size() << " cards. \n" << endl;

}

Card* Deck::draw()
{
	if (vecDeck.empty())
	{
		cout << "there are no more cards in the deck " << endl;
		return 0;
	}
	printVecDeckSize();

	tempCard = *vecDeck.begin();
	//remove this card from the deck vector
	vecDeck.erase(vecDeck.begin());
	return tempCard;
}

void Deck::addCard(Card* oneCard)
{
	//add card to the deck
	vecDeck.push_back(oneCard);
}



Hand::Hand()
{
	this->vecHand = vector<Card*>();
	this->vecPlayCards = vector<Card*>();
}

Hand::~Hand()
{
	vecHand.clear();//vector<Card*> vecHand;//for storing cards in player hand

	vecPlayCards.clear();//vector<Card*> vecPlayCards;//for storing cards to be played this turn
}

void Hand::addCard(Card* aCard)
{
	//add the card into the hand
	vecHand.push_back(aCard);

}

void Hand::play(Card* aCard, Deck* aDeck)
{
	//when a card is played, this card is put into vecPlayCards which is a vector of all cards to be played.

	if (!vecHand.empty()) {
		Card* temp = new Card(*aCard);
		vecPlayCards.push_back(temp);
		returnPlayedCardToDeck(aDeck);
		removePlayedCardOfHand(aCard);
		cout << "card played" << endl;
		//for next assignment implement an order before immediately clearing
		vecPlayCards.clear();
	}
	else
	{
		cout << "You have no cards please draw" << endl;
	}


}


void Hand::returnPlayedCardToDeck(Deck* aDeck)
{
	//return played cards to deck cards
	//traverse the vec_play_cards
	for (int n = 0; n < vecPlayCards.size(); n++) {
		//add the played card of vecPlayCards to Deck cards
		aDeck->addCard(vecPlayCards.at(n));
	}

}

void Hand::printVecPlayCards()
{
	if (vecPlayCards.size() > 0) {
		cout << " the order in which cards must be played is: " << endl;
		//traverses the vector which holds all cards to be played, this is made to easily show what cards there are to be played
		for (int i = 0; i < vecPlayCards.size(); i++) {
			cout << "  #" << i << " is " << vecPlayCards.at(i)->getCardType() << endl;
		}
	}
	else {
		cout << "\n there are no more cards to play" << endl;
	}
}

vector<Card*>* Hand::getVecHand()
{
	if (vecHand.empty()) {
		cout << "this hand is empty" << endl;
		return 0;
	}
	else
		return &vecHand;
}

vector<Card*>* Hand::getVecPlayCards()
{
	return &vecPlayCards;
}

void Hand::removePlayedCardOfHand(Card* rCard)
{
	//traverse the hand cards, find a same type card, then delete it. 
	//we don't need to find the specific card because it is hard, just delete one same type card from handcards
	for (int p = 0; p < vecHand.size(); p++) {
		if (vecHand.at(p)->getCardType() == rCard->getCardType()) {
			//find a same type card, then delete, and return.
			vecHand.erase(vecHand.begin() + p);
			cout << "...removing card from hand: " << rCard->getCardType() << endl;
			return;
		}
	}
}

int Hand::getVecHandSize()
{
	return vecHand.size();

}


//Copy Constructors
Card::Card(const Card& copy)
{
	this->cardType = copy.cardType;
}

Deck::Deck(const Deck& orig) :vecDeck(orig.vecDeck.size())
{
	for (size_t i = 0; i < orig.vecDeck.size(); ++i)
	{
		vecDeck[i] = new Card(*orig.vecDeck[i]);
	}
}

Hand::Hand(const Hand& orig) :vecHand(orig.vecHand.size())
{
	for (size_t i = 0; i < orig.vecHand.size(); ++i)
	{
		vecHand[i] = new Card(*orig.vecHand[i]);
	}
}


//Assignment Operators
Card& Card::operator=(const Card& rightSide)
{
	this->cardType = rightSide.cardType;
	return *this;
}

Deck& Deck::operator=(const Deck& rightSide)
{
	for (size_t i = 0; i < rightSide.vecDeck.size(); ++i)
	{
		vecDeck[i] = new Card(*rightSide.vecDeck[i]);
	}
	return *this;
}

Hand& Hand::operator=(const Hand& rightSide)
{
	for (size_t i = 0; i < rightSide.vecHand.size(); ++i)
	{
		vecHand[i] = new Card(*rightSide.vecHand[i]);
	}
	return *this;
}


//to_string functions
string Card::to_string()
{
	string str = "Card: " + cardType;
	return str;
}

string Deck::to_string()
{
	string str = string("\n the current vecDeck contains ") + std::to_string(vecDeck.size()) + " cards. \n";
	for (int i = 0; i < vecDeck.size(); i++) {
		str += string(" # ") + std::to_string(i);
		str += string(" card is ") + vecDeck.at(i)->getCardType() + string("\n");
	}
	return str;
}

string Hand::to_string()
{
	string str = "";
	if (vecHand.size() > 0) {
		str += string("this hand has ") + std::to_string(vecHand.size());
		str += string(" cards \n");
		for (int j = 0; j < vecHand.size(); j++) {
			str += string("  #") + std::to_string(j);
			str += string(" is ") + vecHand.at(j)->getCardType() + string("\n");
		}
	}
	else {
		str += string("\n this hand has no cards \n");
	}
	return str;
}


//stream insertion operator
ostream& operator<<(ostream& strm, Card& card)
{
	return strm << card.to_string();
}

ostream& operator<<(ostream& strm, Deck& deck)
{
	return strm << deck.to_string();
}

ostream& operator<<(ostream& strm, Hand& hand)
{
	return strm << hand.to_string();
}