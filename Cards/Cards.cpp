#include "Cards.h"

Card::Card()
{
    //cout << " Card is starting...initial types..." << endl;
    initialize_vec_card_type();
}

Card::~Card()
{
    //delete pointer
    //delete(card_type);
}

void Card::initialize_vec_card_type()
{
    //assign types to array

    vec_card_type.push_back("bomb");
    vec_card_type.push_back("reinforcement");
    vec_card_type.push_back("blockade");
    vec_card_type.push_back("airlift");
    vec_card_type.push_back("diplomacy");

}

vector<string>* Card::get_ptr_card_type_arr()
{
    return &vec_card_type;
}

void Card::print_vec_card_type()
{
    cout << "the arr_types_card contains:  " << endl;
    for (int i = 0; i < vec_card_type.size(); i++) {
        cout << vec_card_type.at(i) << endl;
    }
}

void Card::set_card_type_id(int num)
{
    card_type = &vec_card_type.at(num);
}

string* Card::get_card_type()
{
    return card_type;
}



Deck::Deck()
{
}

Deck::~Deck()
{
    //delete ptr
    //delete(ptr_card);//pointer to card
    //delete(temp_card);
}

void Deck::initialize_vec_deck()
{
    //assign 30 cards in vec, each type has 5 cards.
    for (int i = 0; i < 35; i++) {
        ptr_card = new Card;
        if (i < 7) {
            ptr_card->set_card_type_id(0);
            vec_deck.push_back(ptr_card);
        }
        if (i >= 7 && i < 14) {
            ptr_card->set_card_type_id(1);
            vec_deck.push_back(ptr_card);
        }
        if (i >= 14 && i < 21) {
            ptr_card->set_card_type_id(2);
            vec_deck.push_back(ptr_card);
        }
        if (i >= 21 && i < 28) {
            ptr_card->set_card_type_id(3);
            vec_deck.push_back(ptr_card);
        }
        if (i >= 28 && i < 35) {
            ptr_card->set_card_type_id(4);
            vec_deck.push_back(ptr_card);
        }

    }
}

void Deck::print_vec_deck_size()
{
    cout << "\n The current vec_deck contains " << vec_deck.size() << " cards. \n" << endl;

}

void Deck::print_vec_deck()
{
    cout << "\n The current vec_deck contains " << vec_deck.size() << " cards. \n" << endl;
    for (int i = 0; i < vec_deck.size(); i++) {
        cout << "  #" << i << " card is " << *vec_deck.at(i)->get_card_type() << endl;
    }
}

Card* Deck::draw()
{
    //draw randomly from the remaining of the deck
    //random is from 0 to the size of current vec_deck, [0, size). (rand()%(b-a))+a is [a,b).
    int temp = (rand() % vec_deck.size());
    temp_card = vec_deck.at(temp);

    //remove this card from the deck vector
    vec_deck.erase(vec_deck.begin() + temp);

    return temp_card;
}

void Deck::add_card_to_deck_vec(Card* one_card)
{
    //use push_back to add the card into vec_deck_cards
    vec_deck.push_back(one_card);
}



HandCards::HandCards()
{
}

HandCards::~HandCards()
{
    //destruct
}

void HandCards::set_vec_hand_cards(Card* a_card)
{
    //add the card into the handcards,we can recall the deck->draw() to get a card
    vec_hand_cards.push_back(a_card);

}

void HandCards::print_vec_hand_cards()
{
    if (vec_hand_cards.size() > 0) {
        cout << "this hand has " << vec_hand_cards.size() << " cards " << endl;
        for (int j = 0; j < vec_hand_cards.size(); j++) {
            cout << "  #" << j << " is " << *vec_hand_cards.at(j)->get_card_type() << endl;
        }
    }
    else {
        cout << "\n this hand has no cards" << endl;
    }
}

void HandCards::print_vec_play_cards()
{
    if (vec_play_cards.size() > 0) {
        cout << " the order in which cards must be played is: " << endl;
        //traverse to print all play cards order
        for (int i = 0; i < vec_play_cards.size(); i++) {
            cout << "  #" << i << " is " << *vec_play_cards.at(i)->get_card_type() << endl;
        }
    }
    else {
        cout << "\n there are no more cards to play" << endl;
    }
}

void HandCards::play(Card* a_card)
{
    //set this card to play order (vec_play_cards)
    vec_play_cards.push_back(a_card);

}

void HandCards::return_played_card_to_deck(Deck* a_Deck)
{
    //return played cards to deck cards
    //traverse the vec_play_cards
    for (int n = 0; n < vec_play_cards.size(); n++) {
        //add the played card of vec_play_cards to Deck cards
        a_Deck->add_card_to_deck_vec(vec_play_cards.at(n));
    }

}

vector<Card*>* HandCards::get_vec_hand_cards()
{
    return &vec_hand_cards;
}

vector<Card*>* HandCards::get_vec_play_cards()
{
    return &vec_play_cards;
}

void HandCards::remove_played_card_of_hand_cards(Card* r_card)
{
    //traverse the hand cards, find a same type card, then delete it.
    //we don't need to find the specific card because it is hard, just delete one same type card from handcards
    for (int p = 0; p < vec_hand_cards.size(); p++) {
        if (*vec_hand_cards.at(p)->get_card_type() == *r_card->get_card_type()) {
            //find a same type card, then delete, and return.
            vec_hand_cards.erase(vec_hand_cards.begin() + p);
            cout << "...removing card from hand: " << *r_card->get_card_type() << endl;
            return;
        }
    }
}

void HandCards::remove_all_played_cards_of_hand_cards()
{
    //traverse to find each play card of vec_play_cards, then delete same type card in handcards
    for (int j = 0; j < vec_play_cards.size(); j++) {
        //traverse the handcards
        //recall remove a played card of hand cards
        remove_played_card_of_hand_cards(vec_play_cards.at(j));
    }
}

void HandCards::clear_play_cards()
{
    //clear play_cards after player played cards
    vec_play_cards.clear();
    cout << "\n clearing play_cards vector " << endl;
}

