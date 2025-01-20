#include "decky.h"
#include <array>
#include <gtest/gtest.h>
#include <print>
#include <ranges>

using std::range_error;
using std::ranges::find_if;
using std::logic_error;
using std::array;
using std::ranges::all_of;
using std::views::zip;
using std::get;
using std::string;
using std::transform;
using std::vector;


TEST(card, deck_rank_too_high)
{
    EXPECT_THROW(Card(52), range_error);
};

TEST(card, lt_correct)
{
    EXPECT_TRUE(Card(0) < Card(1));
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_A) < Card(0));
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_B) < Card(0));
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_A) < Card(Card::Suit::NONE, Card::Rank::JOKER_B));
}

TEST(card, lt_incorrect)
{
    EXPECT_FALSE(Card(0) < Card(0));
    EXPECT_FALSE(Card(1) < Card(0));
    EXPECT_FALSE(Card(Card::Suit::NONE, Card::Rank::JOKER_A) < Card(Card::Suit::NONE, Card::Rank::JOKER_A));
    EXPECT_FALSE(Card(Card::Suit::NONE, Card::Rank::JOKER_B) < Card(Card::Suit::NONE, Card::Rank::JOKER_A));
    EXPECT_FALSE(Card(Card::Suit::NONE, Card::Rank::JOKER_B) < Card(Card::Suit::NONE, Card::Rank::JOKER_B));
}

TEST(card, eq_correct)
{
    EXPECT_TRUE(Card(0) == Card(0));
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_A) == Card(Card::Suit::NONE, Card::Rank::JOKER_A));
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_B) == Card(Card::Suit::NONE, Card::Rank::JOKER_B));
}

TEST(card, eq_incorrect)
{
    EXPECT_FALSE(Card(0) == Card(1));
    EXPECT_FALSE(Card(Card::Suit::NONE, Card::Rank::JOKER_A) == Card(Card::Suit::NONE, Card::Rank::JOKER_B));
    EXPECT_FALSE(Card(Card::Suit::NONE, Card::Rank::JOKER_B) == Card(Card::Suit::NONE, Card::Rank::JOKER_A));
}

TEST(card, test_card_as_int)
{
    EXPECT_TRUE(Card(0).card_as_int() == 0);
    EXPECT_TRUE(Card(10).card_as_int() == 10);
    EXPECT_TRUE(Card(20).card_as_int() == 20);
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_A).card_as_int() == 52);
    EXPECT_TRUE(Card(Card::Suit::NONE, Card::Rank::JOKER_B).card_as_int() == 52);
}

TEST(deck, default_create)
{
    auto deck = Deck();
    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[51] == Card(51));
}

TEST(deck, size)
{
    {
        auto deck = Deck();
        EXPECT_TRUE(deck.size() == 52);
    }
    {
        auto deck = Deck(Deck::Kind::WITH_JOKERS);
        EXPECT_TRUE(deck.size() == 54);
    }
}

TEST(deck, copy_range)
{
    Card cards[5] = { Card(0), Card(1), Card(2), Card(3), Card(4) };
    auto deck = Deck(cards); // Ugh, C++. Evaluating the pointer to cards as a bool

    EXPECT_TRUE(deck.deck.size() == 5);
    EXPECT_TRUE(deck[0] == Card(0));
}

TEST(deck, random_access)
{
    auto deck = Deck();

    EXPECT_TRUE(deck[0] == Card(0));
    EXPECT_TRUE(deck[13] == Card(13));
    EXPECT_TRUE(deck[26] == Card(26));
    EXPECT_TRUE(deck[39] == Card(39));
    EXPECT_TRUE(deck[51] == Card(51));
}

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

TEST(deck, deal_top)
{
    auto deck = Deck();
    EXPECT_TRUE(deck.deal(0) == Card(0) && deck[0] == Card(1) && deck.deck.size() == 51);
}

TEST(deck, deal_bottom)
{
    auto deck = Deck();
    EXPECT_TRUE(deck.deal(51) == Card(51) && *(deck.deck.end() - 1) == Card(50) && deck.deck.size() == 51);
}

TEST(deck, deal_middle)
{
    auto deck = Deck();
    EXPECT_TRUE(deck.deal(10) == Card(10) && deck.deck.size() == 51);
}

// deck: remove arbitrary set of cards
TEST(deck, arbitrary_cards)
{
    auto deck = Deck();
    auto Pred = [](const Card& x) { return x.SUIT == Card::Suit::CLUB; }; // Lambda fxn that returns true if the card is a club

    deck.remove_if(Pred);

    if (find_if(deck.deck, Pred) != deck.deck.end())
        EXPECT_TRUE(false); // If there's ANYTHING in this set, then the test failed
}

TEST(deck, insert_arbitrary)
{
    Card joker = Card(Card::Suit::NONE, Card::Rank::JOKER_A);
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
        bool exception_thrown = false;
        try
        {
            deck.insert(joker, 100);
        }
        catch (std::out_of_range& _)
        {
            exception_thrown = true;
        }
        EXPECT_TRUE(exception_thrown);
    }
}

TEST(deck, triple_cut)
{
    {
        auto deck = Deck();
        EXPECT_THROW(deck.triple_cut(), logic_error);

        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_A), 10);
        EXPECT_THROW(deck.triple_cut(), logic_error);

        auto deck2 = Deck();
        deck2.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_B), 10);
        EXPECT_THROW(deck.triple_cut(), logic_error);
    }

    {
        auto deck = Deck();
        auto deck2 = Deck();

        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_A), 0);
        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_B), 53);
        deck.triple_cut();

        for (size_t i = 0; i < deck2.size(); i++)
            EXPECT_TRUE(deck[i + 1] == deck2[i]); // one card ahead of the increment (the first joker)
        EXPECT_TRUE(*(deck.deck.crbegin()) == Card(Card::Suit::NONE, Card::Rank::JOKER_B));
    }

    {
        auto deck = Deck();
        auto deck2 = Deck();

        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_A), 13);
        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_B), 30);

        deck.triple_cut();

        size_t joker_1_pos = 23; // deck.size() - 30
        size_t joker_2_pos = 40; // (deck.size() - 30) + (joker_a_pos - joker_b_pos)

        for (size_t i = 0; i < 13; i++)
            EXPECT_TRUE(deck[joker_2_pos + 1 + i] == deck2[i]);

        for (size_t i = 0; i < 16; i++)
            EXPECT_TRUE(deck[joker_1_pos + 1 + i] == deck2[13 + i]);

        for (size_t i = 0; i < joker_1_pos; i++)
            EXPECT_TRUE(deck[i] == deck2[30 - 1 + i]);

        EXPECT_TRUE(deck[23] == Card(Card::Suit::NONE, Card::Rank::JOKER_A));
        EXPECT_TRUE(deck[40] == Card(Card::Suit::NONE, Card::Rank::JOKER_B));
    }

    {
        auto deck = Deck();
        auto deck2 = Deck();

        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_A), 26);
        deck.insert(Card(Card::Suit::NONE, Card::Rank::JOKER_B), 27);

        deck.triple_cut();

        for (size_t i = 0; i < 26; i++)
            EXPECT_TRUE(deck[28 + i] == deck2[i]);

        for (size_t i = 0; i < 26; i++)
            EXPECT_TRUE(deck[i] == deck2[26 + i]);

        EXPECT_TRUE(deck[26] == Card(Card::Suit::NONE, Card::Rank::JOKER_A));
        EXPECT_TRUE(deck[27] == Card(Card::Suit::NONE, Card::Rank::JOKER_B));
    }
}

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

TEST(deck, bury_joker_a_with_wrap)
{
    auto joker_a = Card(Card::Suit::NONE, Card::Rank::JOKER_A);

    {
        auto deck = Deck();
        deck.insert(joker_a, 0);
        deck.bury_joker_a();

        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == joker_a);
    }

    {
        auto deck = Deck();
        deck.insert(joker_a, 52);
        deck.bury_joker_a();

        EXPECT_TRUE(deck[52] == Card(51));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == joker_a);
    }
}

TEST(deck, bury_joker_b_with_wrap)
{
    auto joker_b = Card(Card::Suit::NONE, Card::Rank::JOKER_B);

    {
        auto deck = Deck();
        deck.insert(joker_b, 0);
        deck.bury_joker_b();

        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == Card(1));
        EXPECT_TRUE(deck[2] == joker_b);
    }

    {
        auto deck = Deck();
        deck.insert(joker_b, 51);
        deck.bury_joker_b();

        EXPECT_TRUE(deck[52] == Card(51));
        EXPECT_TRUE(deck[0] == Card(0));
        EXPECT_TRUE(deck[1] == joker_b);
    }

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

TEST(deck, count_cut)
{
    auto deck = Deck();
    deck.count_cut();
    EXPECT_TRUE(deck[50] == Card(50));

    auto joker_a = Card(Card::Suit::NONE, Card::Rank::JOKER_A);
    auto joker_b = Card(Card::Suit::NONE, Card::Rank::JOKER_B);

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

TEST(deck, get_keystream_value)
{
    auto deck = Deck(Deck::Kind::WITH_JOKERS);

    auto keystream_val = deck.get_keystream_value();
    EXPECT_TRUE(keystream_val == 2);
}

TEST(solitaire_ks, deck_state)
{
    auto deck = Deck(Deck::Kind::WITH_JOKERS);
    auto joker_a = Card(Card::Suit::NONE, Card::Rank::JOKER_A);
    auto joker_b = Card(Card::Suit::NONE, Card::Rank::JOKER_B);

    EXPECT_TRUE(deck[52] == joker_a);
    EXPECT_TRUE(deck[53] == joker_b);

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

TEST(solitaire_ks, keystream_generation)
{
    auto deck = Deck(Deck::Kind::WITH_JOKERS);

    // this assumes the algorithm skips a 53 at step 4
    array<uint8_t, 10> test_vector { 4, 49, 10, 24, 8, 51, 44, 6, 4, 33 };
    array<uint8_t, 10> results {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::transform(results.begin(), results.end(), results.begin(),
        [&](const auto& _)
        {
            return get_raw_keystream_value(deck);
        });

    EXPECT_TRUE(all_of(zip(test_vector, results),
        [](const auto& x)
        {
            return get<0>(x) == get<1>(x);
        }));
}

TEST(solitaire_ks, convert_string_to_numbers)
{
    const array<uint8_t, 10> correct_result = { 4, 15, 14, 15, 20, 21, 19, 5, 16, 3 };
    auto result = convert_string_to_uint8("Do not use PC!");
    EXPECT_TRUE(all_of(zip(result, correct_result),
        [](const auto &x) { return get<0>(x) == get<1>(x); }));
}

TEST(solitaire_ks, convert_numbers_to_string)
{
    const vector<uint8_t> input_vector { 4, 15, 14, 15, 20, 21, 19, 5, 16, 3 };
    const string correct_result { "DONOTUSEPC" };
    const auto result = convert_uint8_to_string(input_vector);
    EXPECT_TRUE(result == correct_result);
}

TEST(solitaire_ks, crypt_encrypt)
{
    string input_string {"AAA aaaa AAA"}; // input_string = "contents here"; would also work
    string expected_result {"EXKYIZSGEH"};
    auto deck = Deck(Deck::Kind::WITH_JOKERS);
    string result = crypt(input_string, deck, Opmode::ENCRYPT);

    EXPECT_TRUE(result == expected_result);
    EXPECT_TRUE(deck == Deck(Deck::Kind::WITH_JOKERS));
}

TEST(solitaire_ks, crypt_decrypt)
{
    string input_string {"EXKY IZS GEH!"};
    string expected_result {"AAAAAAAAAA"};

    auto deck = Deck(Deck::Kind::WITH_JOKERS);
    string result = crypt(input_string, deck, Opmode::DECRYPT);

    EXPECT_TRUE(result == expected_result);
    EXPECT_TRUE(deck == Deck(Deck::Kind::WITH_JOKERS));
}

TEST(solitaire_ks, encrypt)
{
    string input_string {"AAA AAAA AAA"}; // input_string = "contents here"; would also work
    string expected_result {"EXKYIZSGEH"};
    auto deck = Deck(Deck::Kind::WITH_JOKERS);
    
    string result = encrypt(input_string, deck);

    EXPECT_TRUE(result == expected_result);
    EXPECT_TRUE(deck == Deck(Deck::Kind::WITH_JOKERS));
}

TEST(solitaire_ks, decrypt)
{
    string input_string {"EXKY IZS GEH!"};
    string expected_result {"AAAAAAAAAA"};

    auto deck = Deck(Deck::Kind::WITH_JOKERS);
    string result = decrypt(input_string, deck);

    EXPECT_TRUE(result == expected_result);
    EXPECT_TRUE(deck == Deck(Deck::Kind::WITH_JOKERS));
}
