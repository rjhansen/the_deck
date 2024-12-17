#include "decky.h"

// Card method definitions
bool Card::operator <(const Card& other) const // the const at the end tells the compiler that this will never change the state of the object
{
    if(other.RANK == Rank::JOKER)
        return false;

    auto suit = static_cast<uint32_t>(SUIT);
    auto rank = static_cast<uint32_t>(RANK);
    auto other_suit = static_cast<uint32_t>(other.SUIT);
    auto other_rank = static_cast<uint32_t>(other.RANK);

    // Suit ranking order: clubs, diamonds, hearts, spades
    auto deck_rank = (suit * 13) + rank;
    auto other_deck_rank = (other_suit * 13) + other_rank;
    
    return deck_rank < other_deck_rank;
}

bool Card::operator ==(const Card& other) const
{
    if(other.SUIT == SUIT && other.RANK == RANK)
        return true;

    return false;
}
