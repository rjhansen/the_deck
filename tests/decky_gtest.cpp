#include <gtest/gtest.h>
#include "decky.h"


// Test two things: deck and cards
// card: suit, rank
TEST(card, deck_rank_too_high)
{
    EXPECT_THROW(Card(52), std::range_error);
};
//TEST(card, lt_correct){}
//TEST(card, lt_incorrect){}
//TEST(card, eq_correct){}
//TEST(card, eq_incorrect){}
// card: less_than, equal

// deck: container of cards
// deck: test random access into deck
// deck: initially sorted
// deck: shuffle
// deck: sort
// deck: deal from arbitrary position (top, middle, bottom)
// deck: insert card into arbitrary position (top, middle, bottom)

// STRETCH GOAL: Implement Solitaire (crypto cipher)