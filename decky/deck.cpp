
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
        std::random_device rd;
        //std::mt19937 gen(rd()); // Mersenne Twister (1993) PRNG
        std::default_random_engine e{rd()};
        std::shuffle(deck.begin(), deck.end(), e);
    }

    void Deck::sort()
    {
        std::sort(deck.begin(), deck.end());
    }

    void Deck::remove_if(auto Pred)
    {
        deck.erase(
            std::remove_if(deck.begin(), deck.end(), Pred), // Arranges the vector such that the things to be erased are all at the end, and returns an iterator to the first object to be erased
            deck.end()
        );
    }

    Card Deck::deal(const size_t position)
    {
        Card card(deck.at(position));
        deck.erase(deck.begin() + position);
        return card;
    }
