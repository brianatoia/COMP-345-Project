#include "Cards.h"
#include <random>
#include <algorithm>
using namespace std;

Card::Card()
{
	this->cardType = BOMB;
}

Card::~Card()
{

}

void Card::setCardType(order o)
{
	this->cardType = o;
}

string Card::getCardType()
{
	switch (cardType)
	{
	case BOMB: return "Bomb";
	case AIRLIFT: return "Airlift";
	case DIPLOMACY: return "Diplomacy";
	case REINFORCEMENT: return "Reinforcement";
	case BLOCKADE: return "Blockade";
	default: return "Bomb";
	}
}


Deck::Deck()
{
	this->deck = vector<Card*>();
}

Deck::~Deck()
{
	deck.clear();

}

void Deck::initializeDeck(int numPlayers)
{

	int deckSize = (numPlayers * 10);
	int handSize = deckSize / numPlayers;

	for (int i = 0; i <= deckSize; i++) {
		Card* ptrCard = new Card();
		if (i % handSize == 0 || i % handSize == 5) {
			ptrCard->setCardType(BOMB);
			deck.push_back(ptrCard);
		}
		else if (i % handSize == 1 || i % handSize == 6) {
			ptrCard->setCardType(AIRLIFT);
			deck.push_back(ptrCard);
		}
		else if (i % handSize == 2 || i % handSize == 7) {
			ptrCard->setCardType(BLOCKADE);
			deck.push_back(ptrCard);
		}
		else if (i % handSize == 3 || i % handSize == 8) {
			ptrCard->setCardType(REINFORCEMENT);
			deck.push_back(ptrCard);
		}
		else if (i % handSize == 4 || i % handSize == 9) {
			ptrCard->setCardType(DIPLOMACY);
			deck.push_back(ptrCard);
		}

	}

	random_shuffle(deck.begin(), deck.end());
}

void Deck::printDeckSize()
{
	cout << "\n The current Deck contains " << deck.size() << " cards. \n" << endl;

}

Card* Deck::draw()
{
	if (deck.empty())
	{
		cout << "there are no more cards in the deck " << endl;
		return 0;
	}
	printDeckSize();

	Card* tempCard = *deck.begin();
	//remove this card from the deck vector
	deck.erase(deck.begin());
	return tempCard;
}

void Deck::addCard(Card* oneCard)
{
	//add card to the deck
	deck.push_back(oneCard);
}



Hand::Hand()
{
	this->hand = vector<Card*>();
	this->playCards = vector<Card*>();
}

Hand::~Hand()
{
	for (auto i : hand)
	{
		delete i;
		i = nullptr;
	}

	for (auto p : playCards)
	{
		delete p;
		p = nullptr;
	}
}

void Hand::addCard(Card* aCard)
{
	//add the card into the hand
	hand.push_back(aCard);

}

void Hand::play(Card* aCard, Deck* aDeck)
{
	//when a card is played, this card is put into PlayCards which is a vector of all cards to be played.

	if (!hand.empty()) {
		Card* temp = new Card(*aCard);
		playCards.push_back(temp);
		returnPlayedCardToDeck(aDeck);
		removePlayedCardOfHand(aCard);
		cout << "card played" << endl;
		//for next assignment implement an order before immediately clearing
		playCards.clear();
	}
	else
	{
		cout << "You have no cards please draw" << endl;
	}

}


void Hand::returnPlayedCardToDeck(Deck* aDeck)
{
	//return played cards to deck cards
	//traverse the _play_cards
	for (int n = 0; n < playCards.size(); n++) {
		//add the played card of PlayCards to Deck cards
		aDeck->addCard(playCards.at(n));
	}

}

void Hand::printPlayCards()
{
	if (playCards.size() > 0) {
		cout << " the order in which cards must be played is: " << endl;
		//traverses the vector which holds all cards to be played, this is made to easily show what cards there are to be played
		for (int i = 0; i < playCards.size(); i++) {
			cout << "  #" << i << " is " << playCards.at(i)->getCardType() << endl;
		}
	}
	else {
		cout << "\n there are no more cards to play" << endl;
	}
}

vector<Card*>* Hand::getHand()
{
	if (hand.empty()) {
		cout << "this hand is empty" << endl;
		return 0;
	}
	else
		return &hand;
}

vector<Card*>* Hand::getPlayCards()
{
	return &playCards;
}

void Hand::removePlayedCardOfHand(Card* rCard)
{
	//traverse the hand cards, find a same type card, then delete it. 
	//we don't need to find the specific card because it is hard, just delete one same type card from handcards
	for (int p = 0; p < hand.size(); p++) {
		if (hand.at(p)->getCardType() == rCard->getCardType()) {
			//find a same type card, then delete, and return.
			hand.erase(hand.begin() + p);
			cout << "...removing card from hand: " << rCard->getCardType() << endl;
			return;
		}
	}
}

bool Hand::findCardType(string type)
{
	for (auto card : hand)
	{
		if (type.compare(card->getCardType()) == 0) return true;
	}
	return false;
}

int Hand::findNumberOfType(string type)
{
	int amount = 0;
	for (auto card : hand)
	{
		if (type.compare(card->getCardType()) == 0) amount++;
	}
	return amount;
}

Card* Hand::getCard(string type)
{
	for (auto card : hand)
	{
		if (type.compare(card->getCardType()) == 0) return card;
	}
}

int Hand::getHandSize()
{
	return hand.size();
}


//Copy Constructors
Card::Card(const Card& copy)
{
	this->cardType = copy.cardType;
}

Deck::Deck(const Deck& orig) :deck(orig.deck.size())
{
	for (size_t i = 0; i < orig.deck.size(); ++i)
	{
		deck[i] = new Card(*orig.deck[i]);
	}
}

Hand::Hand(const Hand& orig) :hand(orig.hand.size())
{
	for (size_t i = 0; i < orig.hand.size(); ++i)
	{
		hand[i] = new Card(*orig.hand[i]);
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
	for (size_t i = 0; i < rightSide.deck.size(); ++i)
	{
		deck[i] = new Card(*rightSide.deck[i]);
	}
	return *this;
}

Hand& Hand::operator=(const Hand& rightSide)
{
	for (size_t i = 0; i < rightSide.hand.size(); ++i)
	{
		hand[i] = new Card(*rightSide.hand[i]);
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
	string str = string("\n the current Deck contains ") + std::to_string(deck.size()) + " cards. \n";
	for (int i = 0; i < deck.size(); i++) {
		str += string(" # ") + std::to_string(i);
		str += string(" card is ") + deck.at(i)->getCardType() + string("\n");
	}
	return str;
}

string Hand::to_string()
{
	string str = "";
	if (hand.size() > 0) {
		str += string("this hand has ") + std::to_string(hand.size());
		str += string(" cards \n");
		for (int j = 0; j < hand.size(); j++) {
			str += string("  #") + std::to_string(j);
			str += string(" is ") + hand.at(j)->getCardType() + string("\n");
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