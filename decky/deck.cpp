
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
        std::shuffle(deck.begin(), deck.end(), gen);
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
            auto c = &deck.at(position); // takes care of bounds checks, because vector.at() does bounds checking
            deck.insert(c, card);
        }

    }

    void Deck::triple_cut()
    {
        auto first_joker = std::find_if(deck.begin(),
                                        deck.end(),
                                        [](const auto& card){ return (card.RANK == Rank::JOKER_A || card.RANK == Rank::JOKER_B);}
                                        );
        if(first_joker == deck.cend())  // Test if there are not enough jokers in the deck
            throw std::exception("No jokers found while trying to perform a triple cut. We need two.");

        auto second_joker = std::find_if(first_joker + 1,
                                         deck.end(),
                                         [](const auto& card){ return (card.RANK == Rank::JOKER_A || card.RANK == Rank::JOKER_B);}
                                        );
        if(second_joker == deck.cend())
            throw std::exception("Only one joker found while trying to perform a triple cut. We need two.");
        

        // If you don't want to have to type std:: in front of every std function,
        // you can import it at the top of the file using a command like
        // using std::vector;
        // Do this for every std:: name/function/object you intend to use a lot
        std::vector<Card> to_first_joker(deck.begin(), first_joker);
        std::vector<Card> after_second_joker(second_joker+1, deck.end());

        // Remove the parts outside the jokers from the deck
        deck.erase(deck.begin(), first_joker);
        deck.erase(second_joker+1, deck.end());

        // Append the middle portion after the original last portion, and then append the first portion to that
        std::copy(deck.cbegin(), deck.cend(), std::back_inserter(after_second_joker));
        std::copy(to_first_joker.cbegin(), to_first_joker.cend(), std::back_inserter(after_second_joker));

        deck = after_second_joker;
    }