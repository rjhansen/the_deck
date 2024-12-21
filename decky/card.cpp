#include "decky.h"

bool Card::operator<(const Card& other) const // the const at the end tells the compiler that this will never change the state of the object
{
    if ((RANK == Rank::JOKER_A && other.RANK == Rank::JOKER_A) || (RANK == Rank::JOKER_B && other.RANK == Rank::JOKER_B))
        return false;

    if (RANK == Rank::JOKER_A)
        return true;

    if (RANK == Rank::JOKER_B && other.RANK != Rank::JOKER_A)
        return true;

    auto suit = static_cast<uint32_t>(SUIT);
    auto rank = static_cast<uint32_t>(RANK);
    auto other_suit = static_cast<uint32_t>(other.SUIT);
    auto other_rank = static_cast<uint32_t>(other.RANK);

    auto deck_rank = (suit * 13) + rank;
    auto other_deck_rank = (other_suit * 13) + other_rank;

    return deck_rank < other_deck_rank;
}

bool Card::operator==(const Card& other) const
{
    if (other.SUIT == SUIT && other.RANK == RANK)
        return true;

    return false;
}

int32_t Card::card_as_int() const
{
    if (SUIT == Suit::NONE && (RANK == Rank::JOKER_A || RANK == Rank::JOKER_B))
        return 52;

    return static_cast<int32_t>(SUIT) * 13 + static_cast<int32_t>(RANK);
}
