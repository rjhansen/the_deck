#include "decky.h"
#include <array>
#include <gtest/gtest.h>
#include <print>

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
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_A) < Card(Suit::NONE, Rank::JOKER_B));
}
TEST(card, lt_incorrect)
{
    EXPECT_FALSE(Card(0) < Card(0));
    EXPECT_FALSE(Card(1) < Card(0));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_A) < Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_B) < Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_B) < Card(Suit::NONE, Rank::JOKER_B));
}
TEST(card, eq_correct)
{
    EXPECT_TRUE(Card(0) == Card(0));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_A) == Card(Suit::NONE, Rank::JOKER_A));
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_B) == Card(Suit::NONE, Rank::JOKER_B));
}
TEST(card, eq_incorrect)
{
    EXPECT_FALSE(Card(0) == Card(1));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_A) == Card(Suit::NONE, Rank::JOKER_B));
    EXPECT_FALSE(Card(Suit::NONE, Rank::JOKER_B) == Card(Suit::NONE, Rank::JOKER_A));
}

TEST(card, test_card_as_int)
{
    EXPECT_TRUE(Card(0).card_as_int() == 0);
    EXPECT_TRUE(Card(10).card_as_int() == 10);
    EXPECT_TRUE(Card(20).card_as_int() == 20);
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_A).card_as_int() == 52);
    EXPECT_TRUE(Card(Suit::NONE, Rank::JOKER_B).card_as_int() == 52);
}

// deck: container of cards
// deck: initially sorted
TEST(deck, default_create)
{
    auto deck = Deck();
    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[51] == Card(51));
}

// deck: size
TEST(deck, size)
{
    {
        auto deck = Deck();
        EXPECT_TRUE(deck.size() == 52);
    }
    {
        auto deck = Deck(1);
        EXPECT_TRUE(deck.size() == 54);
    }
}

// deck: copy from one range of cards to a deck
TEST(deck, copy_range)
{
    Card cards[5] = { Card(0), Card(1), Card(2), Card(3), Card(4) };
    auto deck = Deck(cards); // Ugh, C++. Evaluating the pointer to cards as a bool

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

    EXPECT_TRUE(deck[0] != Card(0) || deck[13] != Card(13) || deck[26] != Card(26) || deck[39] != Card(39) || deck[51] != Card(51));
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

    EXPECT_TRUE(deck[0] != Card(0) || deck[13] != Card(13) || deck[26] != Card(26) || deck[39] != Card(39) || deck[51] != Card(51));

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
    EXPECT_TRUE(deck.deal(0) == Card(0) && deck[0] == Card(1) && deck.deck.size() == 51);
}

TEST(deck, deal_bottom)
{
    auto deck = Deck();
    // Deal bottom
    EXPECT_TRUE(deck.deal(51) == Card(51) && *(deck.deck.end() - 1) == Card(50) && deck.deck.size() == 51);
}

TEST(deck, deal_middle)
{
    auto deck = Deck();
    // Deal middle
    EXPECT_TRUE(deck.deal(10) == Card(10) && deck.deck.size() == 51);
}

// deck: remove arbitrary set of cards
TEST(deck, arbitrary_cards)
{
    auto deck = Deck();
    auto Pred = [](const Card& x) { return x.SUIT == Suit::CLUB; }; // Lambda fxn that returns true if the card is a club

    deck.remove_if(Pred);

    // filter_view returns a view of a range, returning only the objects where Pred is true
    for (const auto& curr_card : std::ranges::filter_view(deck.deck, Pred))
        EXPECT_TRUE(false); // If there's ANYTHING in this set, then the test failed
}

// deck: insert card into arbitrary position (top, middle, bottom)
TEST(deck, insert_arbitrary)
{
    Card joker = Card(Suit::NONE, Rank::JOKER_A);
    {
        auto deck = Deck();
        deck.insert(joker, 51);
        EXPECT_TRUE(deck[51] == joker);
    }

    {
        auto deck = Deck();
        deck.insert(joker, 52);
        EXPECT_TRUE(deck[52] == joker);
    }

    {
        auto deck = Deck();
        deck.insert(joker, 100);
        EXPECT_TRUE(deck[52] == joker);
    }
    // std::print("Joker: Suit: {}, Rank: {}\n", static_cast<int>(joker.SUIT), static_cast<int>(joker.RANK));
    // std::print("Deck[52]: Suit: {}, Rank: {}\n", static_cast<int>(deck[52].SUIT), static_cast<int>(deck[52].RANK));
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

        // deck.deck.insert(deck.deck.begin(), Card(Suit::NONE, Rank::JOKER_A));
        // deck.deck.push_back(Card(Suit::NONE, Rank::JOKER_B));
        deck.insert(Card(Suit::NONE, Rank::JOKER_A), 0);
        deck.insert(Card(Suit::NONE, Rank::JOKER_B), 53);
        deck.triple_cut();

        for (size_t i = 0; i < deck2.size(); i++)
            EXPECT_TRUE(deck[i + 1] == deck2[i]); // one card ahead of the increment (the first joker)
        EXPECT_TRUE(*(deck.deck.crbegin()) == Card(Suit::NONE, Rank::JOKER_B));
    }

    // Standard case: both jokers somewhere in the deck, Joker A first
    {
        auto deck = Deck();
        auto deck2 = Deck();

        deck.insert(Card(Suit::NONE, Rank::JOKER_A), 13);
        deck.insert(Card(Suit::NONE, Rank::JOKER_B), 30);

        deck.triple_cut();

        // The checks are ugly because of array math. Ugh.
        size_t joker_1_pos = 23; // deck.size() - 30
        size_t joker_2_pos = 40; // (deck.size() - 30) + (joker_a_pos - joker_b_pos)

        // std::print("Loop 1: last part of deck")
        for (size_t i = 0; i < 13; i++)
            EXPECT_TRUE(deck[joker_2_pos + 1 + i] == deck2[i]);

        // std::print("Loop 2: middle part of deck")
        for (size_t i = 0; i < 16; i++)
            EXPECT_TRUE(deck[joker_1_pos + 1 + i] == deck2[13 + i]);

        // std::print("Loop 3: first part of deck")
        for (size_t i = 0; i < joker_1_pos; i++)
            EXPECT_TRUE(deck[i] == deck2[30 - 1 + i]);

        EXPECT_TRUE(deck[23] == Card(Suit::NONE, Rank::JOKER_A));
        EXPECT_TRUE(deck[40] == Card(Suit::NONE, Rank::JOKER_B));

        // If you want to do it with iterators and automatic computation, it would look something like this:
        // auto joker_a = std::find(deck.deck.cbegin(), deck.deck.cend(), Card(Suit::NONE, Rank::JOKER_A));
        // auto joker_b = std::find(deck.deck.cbegin(), deck.deck.cend(), Card(Suit::NONE, Rank::JOKER_B));

        // auto tup = std::tuple(deck, Card(0));
        // the auto[iter, i] is how you declare a tuple in C++ and then do a destructuring bind on it.
        // This allows you to access each tuple member individually, rather than having to do a std::get() on each member.
        // for(auto[iter, i]=std::tuple(deck.deck.cbegin(), std::distance(joker_b, deck.deck.cend()-1)); iter < joker_a; iter++, i++)
        //    EXPECT_TRUE(*iter == deck2[i]);

        // for(auto[iter, i]=std::tuple(joker_a+1, std::distance(deck.deck.cbegin(), joker_a)); iter < joker_b; iter++, i++)
        //     EXPECT_TRUE(*iter == deck2[i]);

        // for(auto[iter, i]=std::tuple(joker_b+1, 0); iter < deck.deck.cend(); iter++, i++)
        //     EXPECT_TRUE(*iter == deck2[i]);
    }

    // Edge case: both jokers next to each other in the middle, Joker A first
    {
        auto deck = Deck();
        auto deck2 = Deck();

        deck.insert(Card(Suit::NONE, Rank::JOKER_A), 26);
        deck.insert(Card(Suit::NONE, Rank::JOKER_B), 27);

        deck.triple_cut();

        // The checks are ugly because of array math. Ugh.
        // std::print("Loop 1: last part of deck")
        for (size_t i = 0; i < 26; i++)
            EXPECT_TRUE(deck[28 + i] == deck2[i]);

        // std::print("Loop 2: first part of deck")
        for (size_t i = 0; i < 26; i++)
            EXPECT_TRUE(deck[i] == deck2[26 + i]);

        EXPECT_TRUE(deck[26] == Card(Suit::NONE, Rank::JOKER_A));
        EXPECT_TRUE(deck[27] == Card(Suit::NONE, Rank::JOKER_B));
    }
}

// STRETCH GOAL: Implement Solitaire (crypto cipher)
// solitaire_ks: triple cut: See tests for deck
//

TEST(deck, bury_1_with_wraparound)
{
    {
        auto deck = Deck();

        deck.bury_1_with_wraparound(Card(0));
        EXPECT_TRUE(deck[1] == Card(0));
        EXPECT_TRUE(deck[0] == Card(1));
    }

    {
        auto deck = Deck();

        deck.bury_1_with_wraparound(Card(51));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == Card(51));
        EXPECT_TRUE(deck[51] == Card(50));
    }

    {
        auto deck = Deck();

        deck.bury_1_with_wraparound(Card(50));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[50] == Card(51));
        EXPECT_TRUE(deck[51] == Card(50));
    }
}

TEST(deck, bury_with_wraparound)
{
    {
        auto deck = Deck();

        deck.bury_with_wraparound(Card(0), 3);
        EXPECT_TRUE(deck[0] == Card(1));
        EXPECT_TRUE(deck[1] == Card(2));
        EXPECT_TRUE(deck[2] == Card(3));
        EXPECT_TRUE(deck[3] == Card(0));
    }

    {
        auto deck = Deck();

        deck.bury_with_wraparound(Card(51), 2);
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == Card(1));
        EXPECT_TRUE(deck[2] == Card(51));
        EXPECT_TRUE(deck[51] == Card(50));
    }
}

// deck: locate the A joker, and move it down by 1. If it's at the end of the deck, move it by 2 (so it's the second card)
TEST(deck, bury_joker_a_with_wrap)
{
    auto joker_a = Card(Suit::NONE, Rank::JOKER_A);

    // Test normal bury
    {
        auto deck = Deck();
        deck.insert(joker_a, 0);
        deck.bury_joker_a();

        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == joker_a);
    }

    // Test Wraparound bury
    {
        auto deck = Deck();
        deck.insert(joker_a, 52);
        deck.bury_joker_a();

        EXPECT_TRUE(deck[52] == Card(51));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == joker_a);
    }
}

// deck: locate the B joker and move it down by 2. If it's the second-to-last card, it becomes the 2nd card.
//               If it's last, it becomes 3rd.
TEST(deck, bury_joker_b_with_wrap)
{
    auto joker_b = Card(Suit::NONE, Rank::JOKER_B);

    // Test normal bury
    {
        auto deck = Deck();
        deck.insert(joker_b, 0);
        deck.bury_joker_b();

        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == Card(1));
        EXPECT_TRUE(deck[2] == joker_b);
    }

    // Test Wraparound bury (second-to-last card)
    {
        auto deck = Deck();
        deck.insert(joker_b, 51);
        deck.bury_joker_b();

        EXPECT_TRUE(deck[52] == Card(51));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == joker_b);
    }

    // Test Wraparound bury (last card)
    {
        auto deck = Deck();
        deck.insert(joker_b, 52);
        deck.bury_joker_b();

        EXPECT_TRUE(deck[52] == Card(51));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == Card(1));
        EXPECT_TRUE(deck[2] == joker_b);
    }
}

// deck: count cut: look at the value of the bottom card, if it's either joker, then its value is 53.
//               Remove that many cards from the top of the deck and insert them just above the last card in the deck.
TEST(deck, count_cut)
{
    auto deck = Deck();
    deck.count_cut();
    EXPECT_TRUE(deck[50] == Card(50));

    auto joker_a = Card(Suit::NONE, Rank::JOKER_A);
    auto joker_b = Card(Suit::NONE, Rank::JOKER_B);

    deck.insert(joker_a, 52);
    deck.insert(joker_b, 10);
    deck.count_cut();
    EXPECT_TRUE(deck[50] == Card(49));
    EXPECT_TRUE(deck[10] == joker_b);
    EXPECT_TRUE(deck[53] == joker_a);

    deck.triple_cut();
    deck.count_cut();
    EXPECT_TRUE(deck[53] == Card(9));
}

// deck: look at the value of the top card. Either joker is 53. Count this many places below that card and take the value
//               of that card as the next value in the keystream. If the card counted to is either joker, ignore it and repeat the
//               keystream algorithm.
TEST(deck, get_keystream_value)
{
    auto deck = Deck(1);

    auto keystream_val = deck.get_keystream_value();
    EXPECT_TRUE(keystream_val == 2);
}

// solitaire_ks: test that the deck state is as expected after one run through the keystream generation steps
TEST(solitaire_ks, deck_state)
{
    auto deck = Deck(1);
    auto joker_a = Card(Suit::NONE, Rank::JOKER_A);
    auto joker_b = Card(Suit::NONE, Rank::JOKER_B);

    EXPECT_TRUE(deck[52] == joker_a);
    EXPECT_TRUE(deck[53] == joker_b);

    // Run through each step of the Solitaire keystream algorithm and verify deck state at each step
    deck.bury_joker_a();
    EXPECT_TRUE(deck[53] == joker_a);

    deck.bury_joker_b();
    EXPECT_TRUE(deck[1] == joker_b);

    deck.triple_cut();
    EXPECT_TRUE(deck[0] == joker_b);
    EXPECT_TRUE(deck[52] == joker_a);
    EXPECT_TRUE(deck[53] == Card(0));

    deck.count_cut();
    EXPECT_TRUE(deck[0] == Card(1));
    EXPECT_TRUE(deck[51] == joker_a);
    EXPECT_TRUE(deck[52] == joker_b);
    EXPECT_TRUE(deck[53] == Card(0));
}

// solitaire_ks: compare generated keystream to a test vector
TEST(solitaire_ks, keystream_generation)
{
    uint8_t test_vector[10] = { 4, 49, 10, 24, 8, 51, 44, 6, 4, 33 }; // this assumes the algorithm skips a 53 at step 4
    auto deck = Deck(1); // deck with jokers
    uint8_t ks_val = 0;

    for (const auto& x : test_vector) {
        ks_val = get_raw_keystream_value(deck);
        EXPECT_TRUE(x == ks_val);
    }
}

// solitaire_ks: test conversion of characters to numbers
TEST(solitaire_ks, convert_string_to_numbers)
{
    const char* input_string = "Do not use PC!";
    const std::array<uint8_t, 10> correct_result = { 4, 15, 14, 15, 20, 21, 19, 5, 16, 3 };

    auto result = convert_string_to_uint8(input_string);

    for (size_t i = 0; i < result.size(); i++)
        EXPECT_TRUE(result[i] == correct_result[i]);
}

// solitaire_ks: test conversion of numbers to chars
TEST(solitaire_ks, convert_numbers_to_string)
{
    // The latest and greatest initialization style: curly braces
    const std::vector<uint8_t> input_vector { 4, 15, 14, 15, 20, 21, 19, 5, 16, 3 };
    const std::string correct_result { "DONOTUSEPC" };

    auto result = convert_uint8_to_string(input_vector);

    for (size_t i = 0; i < result.size(); i++)
        EXPECT_TRUE(result[i] == correct_result[i]);
}

// solitaire_ks: test crypt
TEST(solitaire_ks, crypt_encrypt)
{
    std::string input_string {"AAA aaaa AAA"}; // input_string = "contents here"; would also work
    std::string expected_result {"EXKYIZSGEH"};
    auto deck = Deck(1);
    std::string result = crypt(input_string, deck, Opmode::ENCRYPT);

    std::print("Encryption result: {}\n", result);

    EXPECT_TRUE(result == expected_result);
    //EXPECT_TRUE(deck == Deck(1));
}
TEST(solitaire_ks, crypt_decrypt)
{
    auto deck = Deck(1);

}

// solitaire_ks: test encrypt
TEST(solitaire_ks, encrypt)
{
    std::string input_string {"AAA AAAA AAA"}; // input_string = "contents here"; would also work
    std::string expected_result {"EXKYIZSGEH"};
    auto deck = Deck(1);

}
// solitaire_ks: test decrypt
TEST(solitaire_ks, decrypt)
{
    auto deck = Deck(1);

}