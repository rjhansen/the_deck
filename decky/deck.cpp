
#include "decky.h"


    Card& Deck::operator[](const size_t index)
    {
        return deck.at(index);
    }

    const Card& Deck::operator[](const size_t index) const
    { 
        return deck.at(index);
    }

    void Deck::shuffle()
    {
        std::random_shuffle(deck.begin(), deck.end(), gen);
    }