#include <gtest/gtest.h>
#include "decky.h"


// Test two things: deck and cards
// card: suit, rank
TEST(card, deck_rank_too_high)
{
    EXPECT_THROW(Card(52), std::range_error);
};

// card: less_than, equal
TEST(card, lt_correct)
{
    EXPECT_TRUE(Card(0) < Card(1));
}
TEST(card, lt_incorrect)
{
    EXPECT_FALSE(Card(1) < Card(0));
}
TEST(card, eq_correct)
{
    EXPECT_TRUE(Card(0) == Card(0));
}
TEST(card, eq_incorrect)
{
    EXPECT_FALSE(Card(0) == Card(1));
}

// deck: container of cards
// deck: initially sorted
TEST(deck, default_create)
{
    auto deck = Deck();
    EXPECT_TRUE(deck.deck.at(0) == Card(0));
    EXPECT_TRUE(deck.deck.at(51) == Card(51));
}
// deck: test random access into deck
// deck: shuffle
// deck: sort
// deck: deal from arbitrary position (top, middle, bottom)
// deck: insert card into arbitrary position (top, middle, bottom)

// STRETCH GOAL: Implement Solitaire (crypto cipher)