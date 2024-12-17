#include <cstdint>
#include <exception>
#include <algorithm>


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

    Card(const Suit suit, const Rank rank):
        SUIT{suit}, RANK{rank} // Initialize member variables (new style)
        // Makes the initialization of the member variables essentially atomic
        {
            // nothing to do here because of new hotness
        }

    // static_cast is a generic function that enforces type checks at compile time
    Card(uint32_t rank)
        // std::clamp allows you to have a compiler-guaranteed check on value ranges
        SUIT{static_cast<SUIT>(std::clamp(rank / 13, 0, 3)}, RANK{static_cast<RANK>(rank % 13)}
    {
        if(rank > 51)
            throw std::range_error("Card rank must be in the range of 0-51");
    }

    bool operator <(const Card& other) const; // the const at the end tells the compiler that this will never change the state of the object
    bool operator ==(const Card& other) const;
};






























