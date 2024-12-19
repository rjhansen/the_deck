#include <algorithm>
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <print>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

// Enum classes provide compiler guarantees of max and min values
enum class Opmode {
    ENCRYPT = 0,
    DECRYPT
};

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

struct Card {
    Suit SUIT;
    Rank RANK;

    Card()
        : SUIT { Suit::CLUB }
        , RANK { Rank::TWO }
    {
    }

    Card(const Suit suit, const Rank rank)
        : SUIT { suit }
        , RANK { rank } // Initialize member variables (new style)
    // Makes the initialization of the member variables essentially atomic
    {
        // nothing to do here because of new hotness
    }

    // static_cast is a generic function that enforces type checks at compile time
    Card(const int32_t rank)
        : // std::clamp allows you to have a compiler-guaranteed check on value ranges
        SUIT { static_cast<Suit>(std::clamp(rank / 13, 0, 3)) }
        , RANK { static_cast<Rank>(rank % 13) }
    // Older initializer style
    // SUIT(static_cast<Suit>(std::clamp(rank / 13, 0, 3))), RANK(static_cast<Rank>(rank % 13))
    {
        if (rank < 0 || rank > 51)
            throw std::range_error("Card rank must be in the range of 0-51");
    }

    Card(const Card& other)
        : SUIT { other.SUIT }
        , RANK { other.RANK }
    {
    }

    Card& operator=(const Card& other)
    {
        SUIT = other.SUIT;
        RANK = other.RANK;

        return *this;
    }

    bool operator<(const Card& other) const; // the const at the end tells the compiler that this will never change the state of the object
    bool operator==(const Card& other) const;

    int32_t card_as_int() const;
};

std::ostream& operator<<(std::ostream& stream, const Card& card)
{
    // MAKE SURE TO PUT PARENS AROUND THE static_casts BECAUSE OTHERWISE C++ WILL TRY TO 
    // DO A BINARY SHIFT LEFT ON THE INTS
    // THE ERRORS MAKE THE DEAD RISE
    stream << (static_cast<int32_t>(card.SUIT)) << " " << (static_cast<int32_t>(card.RANK));
    return stream;
}

struct Deck {
private:
    std::random_device rd;
    std::mt19937 gen; // Mersenne Twister (1993) PRNG

    // This is too new. We've been doing it this way since 1998, but apparently MSVC disagrees.
    // static const Card JOKER_A(Suit::NONE, Rank::JOKER_A);
    // static const Card JOKER_B(Suit::NONE, Rank::JOKER_B);

    // Old style, but works. See deck.cpp for the instantiation.
    static const Card JOKER_A;
    static const Card JOKER_B;

public:
    std::vector<Card> deck;
    // Creates a deck of cards, in order
    // The parameter is a uint8_t instead of a bool, because if we happen to pass an array
    // to Deck() to call the copy constructor (that's the next one down), C++ will interpret
    // the pointer as a bool to test for NULL, and as a result, will call this constructor
    // with a value of "false" for a NULL pointer and "true" otherwise. So copy constructors
    // break things.
    Deck(uint8_t with_jokers = 0)
        : rd()
        , gen(rd()) // No curly brackets needed when calling a member's constructor directly
    {
        /*
        // C++ 23 way; not supported by compilers yet
        std::vector<int> zero_to_51;
        std::ranges::iota(zero_to_51, 0);
        std::ranges::transform(zero_to_51, deck.emplace_back(), [](const int x) {return Card(x);});
        */

        // C++ 20 way; slightly more verbose, but supported
        std::vector<int> zero_to_51(52);
        std::iota(zero_to_51.begin(), zero_to_51.end(), 0);
        std::transform(zero_to_51.begin(), zero_to_51.end(), std::back_inserter(deck), [](const int x) { return Card(x); }); // lambda function to turn int to Card
        if (with_jokers) {
            deck.push_back(Card(Suit::NONE, Rank::JOKER_A));
            deck.push_back(Card(Suit::NONE, Rank::JOKER_B));
        }
    }

    // Copy constructor. Feels like cheating.
    Deck(const Deck& other)
        : rd()
        , gen(rd())
        , deck(other.deck)
    {
    }

    // C++ 20 way; slightly more wordy
    // MAGIC LIES HERE:
    // std::span will MAGICALLY figure out the size of the input data, even with standard C arrays that don't know their own size.
    // So always accept inputs as std::span, and it will make your code WAY more reliable
    Deck(const std::span<const Card>& other_deck)
        : rd()
        , gen(rd())
    {
        deck.clear();
    #ifdef __APPLE__
        std::copy(other_deck.begin(), other_deck.end(), std::back_inserter(deck));
    #else
        std::copy(other_deck.cbegin(), other_deck.cend(), std::back_inserter(deck));
    #endif
    }

    Card& operator[](const size_t index);
    const Card& operator[](const size_t index) const;
    bool operator==(const Deck& other) const;

    void shuffle();
    void sort();
    Card deal(const size_t position);
    void insert(const Card& card, const size_t position);
    void triple_cut();
    void bury_1_with_wraparound(const Card& card);
    void bury_with_wraparound(const Card& card, const size_t slots_down);
    void bury_joker_a();
    void bury_joker_b();
    void count_cut();
    uint32_t get_keystream_value();

    //void dump() const { std::print("{}\n", deck);}

    //void dump() const 
    //{ 
    //    size_t i = 0;
    //    std::print("Deck: ")
    //    for (const auto& card: deck)
    //        std::print("{} S: {}, R: {} | ", i++, card.SUIT, card.RANK);
    //    std::print("\n");
    //}

    //void dump() const 
    //{ 
    //    std::print("Deck: ");
    //    for(size_t i=0; i<deck.size(); i++)
    //        std::print("{} S: {}, R: {} | ", i, static_cast<int32_t>(deck[i].SUIT), static_cast<int32_t>(deck[i].RANK));
    //    std::print("\n");
    //}

    void dump_numeric() const
    {
        std::print("Deck: ");
    }

    void dump() const
    {
        std::copy(deck.cbegin(), deck.cend(), std::ostream_iterator<Card>(std::cout, " | "));
    }

    // This function is a templatized function, which means it MUST be implemented entirely in the header file
    // This is because the preprocessor/compiler will need access to the full definition whenever it encounters it
    // being called so that it can compute the correct types to build into the templates at compile time.
    // This is because templates must be FULLY AVAILABLE to each compilation unit in which they are used.
    void remove_if(auto Pred)
    {
        deck.erase(
            std::remove_if(deck.begin(), deck.end(), Pred), // Arranges the vector such that the things to be erased are all at the end, and returns an iterator to the first object to be erased
            deck.end());
    }

    // This one, too.
    size_t size() const { return deck.size(); }
};

uint8_t get_raw_keystream_value(Deck& deck);
uint8_t get_keystream_value(Deck& deck);
std::vector<uint8_t> convert_string_to_uint8(std::string input_string);
std::string convert_uint8_to_string(const std::vector<uint8_t>& input_numbers);

std::string crypt(const std::string& input, Deck deck, Opmode mode);
std::string encrypt(const std::string& plaintext, const Deck& deck);
std::string decrypt(const std::string& ciphertext, const Deck& deck);