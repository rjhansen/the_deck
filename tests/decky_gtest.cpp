#include <print>
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
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_A) < Card(0));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_B) < Card(0));
}
TEST(card, lt_incorrect)
{
    EXPECT_FALSE(Card(0) < Card(0));
    EXPECT_FALSE(Card(1) < Card(0));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_A) < Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_A) < Card(Suit::NONE, Rank::JOKER_B));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_B) < Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_B) < Card(Suit::NONE, Rank::JOKER_B));
}
TEST(card, eq_correct)
{
    EXPECT_TRUE(Card(0) == Card(0));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_A) == Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_A) == Card(Suit::NONE, Rank::JOKER_B));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_B) == Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_B) == Card(Suit::NONE, Rank::JOKER_B));
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

// deck: size
TEST(deck, size)
{
    auto deck = Deck();
    EXPECT_TRUE(deck.size() == 52);
}

// deck: copy from one range of cards to a deck
TEST(deck, copy_range)
{
    Card cards[5] = {Card(0), Card(1), Card(2), Card(3), Card(4)};
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
TEST(deck, deal_top)
{
    auto deck = Deck();
    // Deal top
    EXPECT_TRUE(deck.deal(0) == Card(0) &&
                deck[0] == Card(1) &&
                deck.deck.size() == 51);
}

TEST(deck, deal_bottom)
{
    auto deck = Deck();
    // Deal bottom
    EXPECT_TRUE(deck.deal(51) == Card(51) &&
                *(deck.deck.end()-1) == Card(50) &&
                deck.deck.size() == 51);
}

TEST(deck, deal_middle)
{
    auto deck = Deck();
    // Deal middle
    EXPECT_TRUE(deck.deal(10) == Card(10) &&
                deck.deck.size() == 51);
}

// deck: remove arbitrary set of cards
TEST(deck, arbitrary_cards)
{
    auto deck = Deck();
    auto Pred = [](const Card& x){return x.SUIT == Suit::CLUB;}; // Lambda fxn that returns true if the card is a club

    deck.remove_if(Pred);

    // filter_view returns a view of a range, returning only the objects where Pred is true
    for(const auto& curr_card: std::ranges::filter_view(deck.deck, Pred))
        EXPECT_TRUE(false); // If there's ANYTHING in this set, then the test failed
}

// deck: insert card into arbitrary position (top, middle, bottom)
TEST(deck, insert_arbitrary)
{
    auto deck = Deck();
    Card joker = Card(Suit::NONE, Rank::JOKER_A);

    deck.insert(joker, 52);
    EXPECT_TRUE(deck[52] == joker);

    deck.insert(joker, 100);
    EXPECT_TRUE(deck[53] == joker);
    //std::print("Joker: Suit: {}, Rank: {}\n", static_cast<int>(joker.SUIT), static_cast<int>(joker.RANK));
    //std::print("Deck[52]: Suit: {}, Rank: {}\n", static_cast<int>(deck[52].SUIT), static_cast<int>(deck[52].RANK));

}

// deck: triple cut: split deck into sections delimited by jokers, exchange the top and bottom section. 
//       Jokers and cards between them stay.
//       Part of the solitaire crypto algorithm
TEST(deck, triple_cut)
{
    // Edge case: not enough jokers
    {
        auto deck = Deck();
        EXPECT_THROW(deck.triple_cut(), std::logic_error);

        deck.insert(Card(Suit::NONE, Rank::JOKER_A), 10);
        EXPECT_THROW(deck.triple_cut(), std::logic_error);

        auto deck2 = Deck();
        deck2.insert(Card(Suit::NONE, Rank::JOKER_B), 10);
        EXPECT_THROW(deck.triple_cut(), std::logic_error);
    }
    // Edge case: jokers on top and bottom, Joker A first
    {
        auto deck = Deck();
        auto deck2 = Deck();

        //deck.deck.insert(deck.deck.begin(), Card(Suit::NONE, Rank::JOKER_A));
        //deck.deck.push_back(Card(Suit::NONE, Rank::JOKER_B));
        deck.insert(Card(Suit::NONE, Rank::JOKER_A), 0);
        deck.insert(Card(Suit::NONE, Rank::JOKER_B), 53);
        deck.triple_cut();

        for(size_t i = 0; i < deck2.size(); i++)
            EXPECT_TRUE(deck[i+1] == deck2[i]); // one card ahead of the increment (the first joker)
        EXPECT_TRUE(*(deck.deck.crbegin()) == Card(Suit::NONE, Rank::JOKER_B));

    }


    {
        // Standard case: both jokers somewhere in the deck, Joker A first
        auto deck = Deck();
        auto deck2 = Deck();

        deck.insert(Card(Suit::NONE, Rank::JOKER_A), 13);
        deck.insert(Card(Suit::NONE, Rank::JOKER_B), 30);
        deck.triple_cut();

        auto joker_a = std::find(deck.deck.cbegin(), deck.deck.cend(), Card(Suit::NONE, Rank::JOKER_A));
        auto joker_b = std::find(deck.deck.cbegin(), deck.deck.cend(), Card(Suit::NONE, Rank::JOKER_B));

        size_t joker_a_pos = 23; // deck.size() - 30
        size_t joker_b_pos = 40; // deck.size() - 30 + diff(joker_a_pos, joker_b_pos)
        

        // auto tup = std::tuple(deck, Card(0));
        // the auto[iter, i] is how you declare a tuple in C++ and then do a destructuring bind on it.
        // This allows you to access each tuple member individually, rather than having to do a std::get() on each member.
        for(auto[iter, i]=std::tuple(deck.deck.cbegin(), std::distance(joker_b, deck.deck.cend())); iter < joker_a; iter++, i++)
            EXPECT_TRUE(*iter == deck2[i]);
        
        for(auto[iter, i]=std::tuple(joker_a+1, std::distance(deck.deck.cbegin(), joker_a)); iter < joker_b; iter++, i++)
            EXPECT_TRUE(*iter == deck2[i]);

        for(auto[iter, i]=std::tuple(joker_b+1, 0); iter < deck.deck.cend(); iter++, i++)
            EXPECT_TRUE(*iter == deck2[i]);



        EXPECT_TRUE(deck[23] == Card(Suit::NONE, Rank::JOKER_A));
        EXPECT_TRUE(deck[40] == Card(Suit::NONE, Rank::JOKER_B));
    }


    // Edge case: both jokers next to each other in the middle, Joker A first

}

// STRETCH GOAL: Implement Solitaire (crypto cipher)
// solitaire_ks: triple cut: See tests for deck
//               

// solitaire_ks: locate the A joker, and move it down by 1. If it's at the end of the deck, move it by 2 (so it's the second card)
// solitaire_ks: locate the B joker and move it down by 2. If it's the second-to-last card, it becomes the 2nd card.
//               If it's last, it becomes 3rd.
// solitaire_ks: count cut: look at the value of the bottom card, if it's either joker, then its value is 53.
//               Remove that many cards from the top of the deck and insert them just above the last card in the deck.
// solitaire_ks: look at the value of the top card. Either joker is 53. Count this many places below that card and take the value
//               of that card as the next value in the keystream. If the card counted to is either joker, ignore it and repeat the
//               keystream algorithm.