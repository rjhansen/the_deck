
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

    Card Deck::deal(const size_t position)
    {
        Card card(deck.at(position));
        deck.erase(deck.begin() + position);
        return card;
    }

    void Deck::insert(const Card& card, const size_t position)
    {
        if(position == deck.size())
            deck.push_back(card);
        else
        {
            auto c = deck.at(position); // takes care of bounds checks, because vector.at() does bounds checking
            deck.insert(deck.begin() + position, card);
        }

    }
