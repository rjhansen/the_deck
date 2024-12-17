#include <cstdint>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <ranges>


// Enum classes provide compiler guarantees of max and min values
enum class Suit
{
    CLUB = 0,
    DIAMOND,
    HEART,
    SPADE,
    NONE
};

enum class Rank
{
    TWO = 0,
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
    ACE,
    JOKER
};

struct Card
{
    const Suit SUIT;
    const Rank RANK;

    Card(): SUIT{Suit::CLUB}, RANK{Rank::TWO}
    {}

    Card(const Suit suit, const Rank rank):
        SUIT{suit}, RANK{rank} // Initialize member variables (new style)
        // Makes the initialization of the member variables essentially atomic
        {
            // nothing to do here because of new hotness
        }

    // static_cast is a generic function that enforces type checks at compile time
    Card(const int32_t rank): 
        // std::clamp allows you to have a compiler-guaranteed check on value ranges
        SUIT{static_cast<Suit>(std::clamp(rank / 13, 0, 3))},
        RANK{static_cast<Rank>(rank % 13)}
        // Older initializer style
        //SUIT(static_cast<Suit>(std::clamp(rank / 13, 0, 3))), RANK(static_cast<Rank>(rank % 13))
        {
            if(rank < 0 || rank > 51)
                throw std::range_error("Card rank must be in the range of 0-51");
        }

    bool operator <(const Card& other) const; // the const at the end tells the compiler that this will never change the state of the object
    bool operator ==(const Card& other) const;
};

struct Deck
{
    std::vector<Card> deck;

    Deck()
    {
        std::vector<int> zero_to_51;
        std::ranges::iota(zero_to_51, 0);
        std::ranges::transform(zero_to_51, std::emplace_back(deck), [](const int x) {return Card(x);});

    }

};



