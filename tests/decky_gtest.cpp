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
    EXPECT_TRUE(deck[0]== Card(0));
    EXPECT_TRUE(deck[51] == Card(51));
}

// deck: copy from one range of cards to a deck
TEST(deck, copy_range)
{
    Card cards[5] = {Card(0), Card(1), Card(2), Card(3), Card(4) };
    auto deck = Deck(cards);

    EXPECT_TRUE(deck.deck.size() == 5);
    EXPECT_TRUE(deck[0] == Card(0));
}

// deck: test random access into deck
TEST(deck, random_access)
{
    auto deck = Deck();

    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[13] == Card(13));
    EXPECT_TRUE(deck[26] == Card(26));
    EXPECT_TRUE(deck[39] == Card(39));
    EXPECT_TRUE(deck[51] == Card(51));
}

// deck: shuffle
TEST(deck, shuffle)
{
    auto deck = Deck();
    
    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[13] == Card(13));
    EXPECT_TRUE(deck[26] == Card(26));
    EXPECT_TRUE(deck[39] == Card(39));
    EXPECT_TRUE(deck[51] == Card(51));

    deck.shuffle();

    EXPECT_TRUE(deck[0]  != Card(0)  || 
                deck[13] != Card(13) ||
                deck[26] != Card(26) ||
                deck[39] != Card(39) ||
                deck[51] != Card(51));
}

// deck: sort
TEST(deck, sort)
{
    auto deck = Deck();
    
    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[13] == Card(13));
    EXPECT_TRUE(deck[26] == Card(26));
    EXPECT_TRUE(deck[39] == Card(39));
    EXPECT_TRUE(deck[51] == Card(51));

    deck.shuffle();

    EXPECT_TRUE(deck[0]  != Card(0)  || 
                deck[13] != Card(13) ||
                deck[26] != Card(26) ||
                deck[39] != Card(39) ||
                deck[51] != Card(51));

    deck.sort();

    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[13] == Card(13));
    EXPECT_TRUE(deck[26] == Card(26));
    EXPECT_TRUE(deck[39] == Card(39));
    EXPECT_TRUE(deck[51] == Card(51));
}

// deck: deal from arbitrary position (top, middle, bottom)
TEST(deck, deal_arbitrary)
{
    auto deck = Deck();
    // Deal top
    EXPECT_TRUE(deck.deal(0) == Card(0) &&
                deck[0] == Card(1) &&
                deck.deck.size == 51);

    // Deal bottom
    EXPECT_TRUE(deck.deal(50) == Card(50) &&
                *(deck.deck.end()-1) == Card(51) &&
                deck.deck.size() == 50);

    // Deal middle
    EXPECT_TRUE(deck.deal(10) == Card(11) &&
                deck.deck.size() == 49);


}
// deck: insert card into arbitrary position (top, middle, bottom)

// STRETCH GOAL: Implement Solitaire (crypto cipher)