#ifndef DECKY_H
#define DECKY_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <span>
#include <stdexcept>
#include <vector>

enum class Opmode {
    ENCRYPT = 0,
    DECRYPT
};

struct Card {
    enum class Suit {
        CLUB = 0,
        DIAMOND,
        HEART,
        SPADE,
        NONE
    };

    enum class Rank {
        ACE = 0,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        JOKER_A,
        JOKER_B
    };
    Suit SUIT;
    Rank RANK;

    Card()
        : SUIT { Suit::CLUB }
        , RANK { Rank::TWO }
    {
    }

    Card(const Suit suit, const Rank rank)
        : SUIT { suit }
        , RANK { rank }
    {
    }

    explicit Card(const int32_t rank)
        : SUIT { static_cast<Suit>(std::clamp(rank / 13, 0, 3)) }
        , RANK { static_cast<Rank>(rank % 13) }
    {
        if (rank < 0 || rank > 51)
            throw std::range_error("Card rank must be in the range of 0-51");
    }

    Card(const Card& other) = default;
    Card& operator=(const Card& other) = default;

    bool operator<(const Card& other) const; // the const at the end tells the compiler that this will never change the state of the object
    bool operator==(const Card& other) const;

    [[nodiscard]]
    int32_t card_as_int() const;
};

std::ostream& operator<<(std::ostream& stream, const Card& card);

struct Deck {
private:
    std::random_device rd;
    std::mt19937 gen;
    static const Card JOKER_A;
    static const Card JOKER_B;

public:
    std::vector<Card> deck;

    enum class Kind {
        WITHOUT_JOKERS = 0,
        WITH_JOKERS = 1
    };

    explicit Deck(Kind k = Kind::WITHOUT_JOKERS)
        : rd()
        , gen(rd()) // No curly brackets needed when calling a member's constructor directly
    {
        std::vector<int> zero_to_51(52);
        std::iota(zero_to_51.begin(), zero_to_51.end(), 0);
        std::ranges::transform(zero_to_51,
            std::back_inserter(deck),
            [](const int x) {
                return Card(x);
            }); // lambda function to turn int to Card
        if (k == Kind::WITH_JOKERS) {
            deck.emplace_back(JOKER_A);
            deck.emplace_back(JOKER_B);
        }
    }

    // Copy constructor. Feels like cheating.
    Deck(const Deck& other)
        : rd()
        , gen(rd())
        , deck(other.deck)
    {
    }

    explicit Deck(const std::span<const Card>& other_deck)
        : rd()
        , gen(rd())
    {
        deck.clear();
        std::ranges::copy(other_deck, std::back_inserter(deck));
    }

    Card& operator[](size_t index);
    const Card& operator[](size_t index) const;
    bool operator==(const Deck& other) const;

    void shuffle();
    void sort();
    Card deal(size_t position);
    void insert(const Card& card, size_t position);
    void triple_cut();
    void bury_1_with_wraparound(const Card& card);
    void bury_with_wraparound(const Card& card, size_t slots_down);
    void bury_joker_a();
    void bury_joker_b();
    void count_cut();
    [[nodiscard]]
    uint32_t get_keystream_value() const;

    void dump() const
    {
        std::ranges::copy(deck, std::ostream_iterator<Card>(std::cout, " | "));
    }

    void remove_if(auto Pred)
    {
        auto e = std::ranges::remove_if(deck, Pred); // Arranges the vector such that the things
        deck.erase(e.begin(), e.end());
    }

    [[nodiscard]]
    size_t size() const
    {
        return deck.size();
    }
};

uint8_t get_raw_keystream_value(Deck& deck);
uint8_t get_keystream_value(Deck& deck);
std::vector<uint8_t> convert_string_to_uint8(std::string input_string);
std::string convert_uint8_to_string(const std::vector<uint8_t>& input_numbers);

std::string crypt(const std::string& input, Deck deck, Opmode mode);
std::string encrypt(const std::string& plaintext, const Deck& deck);
std::string decrypt(const std::string& ciphertext, const Deck& deck);

template <std::input_iterator T, std::output_iterator<uint8_t> U>
void stl_crypt(T begin, T end, U output, const Deck& deck, Opmode mode)
{
    /* This templated function's complexity calls for some road signs.
     * It's actually not bad: just pay attention to where in the code
     * you are and you'll make it out fine. -- rjh */

    /* Sanity checks */
    if (end == begin)
        return;
    static_assert(sizeof(decltype(*begin)) == 1);

    /* Make our own copy of external data */
    Deck d(deck);
    std::vector<uint8_t> tmp;
    std::copy_if(begin,
        end,
        std::back_inserter(tmp),
        [](auto x) { return ('A' <= x && 'Z' >= x); });
    while (tmp.size() % 5)
        tmp.push_back('X');

    /* Define predicates we'll be using later */
    auto keystream = [&](uint8_t c) -> uint8_t {
        auto deck_val = get_keystream_value(d);
        if (mode == Opmode::ENCRYPT) {
            uint8_t v = c + deck_val;
            while (v > 26)
                v -= 26;
            return 'A' - 1 + v;
        }
        int8_t v = static_cast<int8_t>(c) - static_cast<int8_t>(deck_val);
        while (v < 0)
            v += 26;
        return 'A' - 1 + v;
    };

    /* Convert our message into Solitaire ordinals */
    std::transform(tmp.cbegin(),
        tmp.cend(),
        tmp.begin(),
        [](auto x) { return x - 'A' + 1; });

    /* Apply the Solitaire algorithm */
    std::transform(tmp.cbegin(), tmp.cend(), tmp.begin(), keystream);

    /* Copy to output and we're done */
    std::copy(tmp.cbegin(), tmp.cend(), output);
}

template <std::input_iterator T, std::output_iterator<uint8_t> U>
void solitaire(T begin, T end, U output, const Deck& deck, Opmode mode)
{
    stl_crypt(begin, end, output, deck, mode);
}
void solitaire(std::istream&& input, std::ostream& output, const Deck& deck, Opmode mode);
void solitaire(std::istream& input, std::ostream& output, const Deck& deck, Opmode mode);
void solitaire(std::istream&& input, std::ostream&& output, const Deck& deck, Opmode mode);
void solitaire(std::istream& input, std::ostream&& output, const Deck& deck, Opmode mode);

#endif