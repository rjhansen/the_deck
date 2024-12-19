#include "decky.h"

// Card method definitions
bool Card::operator <(const Card& other) const // the const at the end tells the compiler that this will never change the state of the object
{
    // Joker A == Joker A
    if((RANK == Rank::JOKER_A && other.RANK == Rank::JOKER_A) ||
       (RANK == Rank::JOKER_B && other.RANK == Rank::JOKER_B))
        return false;
    
    // Joker A is less than everything else
    if(RANK == Rank::JOKER_A && other.RANK != Rank::JOKER_A)
        return true;

    // Joker B is less than everything except Joker A
    // We've already tested for self-equality above
    if(RANK == Rank::JOKER_B && other.RANK != Rank::JOKER_A)
        return true;

    // All other cases
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


int32_t Card::card_as_int() const
{
    if(SUIT==Suit::NONE && (RANK==Rank::JOKER_A || RANK==Rank::JOKER_B))
        return 52; // For the Solitaire algorithm's count_cut calculation

    return static_cast<int32_t>(SUIT)*13 + static_cast<int32_t>(RANK);
}
