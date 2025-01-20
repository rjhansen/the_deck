#include "decky.h"

namespace The_Deck {
bool Card::operator<(
    const Card& other) const // the const at the end tells the compiler that
                             // this will never change the state of the object
{
    if ((RANK == Rank::JOKER_A && other.RANK == Rank::JOKER_A) || (RANK == Rank::JOKER_B && other.RANK == Rank::JOKER_B))
        return false;

    if (RANK == Rank::JOKER_A)
        return true;

    if (RANK == Rank::JOKER_B && other.RANK != Rank::JOKER_A)
        return true;

    const auto suit = static_cast<uint32_t>(SUIT);
    const auto rank = static_cast<uint32_t>(RANK);
    const auto other_suit = static_cast<uint32_t>(other.SUIT);
    const auto other_rank = static_cast<uint32_t>(other.RANK);
    const auto deck_rank = (suit * 13) + rank;
    const auto other_deck_rank = (other_suit * 13) + other_rank;

    return deck_rank < other_deck_rank;
}

bool Card::operator==(const Card& other) const
{
    return other.SUIT == SUIT && other.RANK == RANK;
}

int32_t Card::card_as_int() const
{
    return (SUIT == Suit::NONE && (RANK == Rank::JOKER_A || RANK == Rank::JOKER_B))
        ? 52
        : static_cast<int32_t>(SUIT) * 13 + static_cast<int32_t>(RANK);
}
} // namespace The_Deck