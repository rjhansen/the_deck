#include <cstdint>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <vector>
#include <ranges>
#include <numeric>
#include <random>
#include <span>


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
    private:
        std::random_device rd;
        std::mt19937 gen; // Mersenne Twister (1993) PRNG

    public:
        std::vector<Card> deck;
        // Creates a deck of cards, in order
        Deck(): rd(), gen(rd) // No curly brackets needed when calling a constructor directly
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
            std::transform(zero_to_51.begin(), zero_to_51.end(), std::back_inserter(deck), [](const int x) {return Card(x);}); // lambda function to turn int to Card
        }

        // C++ 20 way; slightly more wordy
        // MAGIC LIES HERE: 
        // std::span will MAGICALLY figure out the size of the input data, even with standard C arrays that don't know their own size.
        // So always accept inputs as std::span, and it will make your code WAY more reliable
        Deck(const std::span<const Card>& other_deck): gen(std::random_device())
        {
            deck.clear();
            std::copy(other_deck.cbegin(), other_deck.cend(), std::back_inserter(deck));
        }

        Card& operator[](const size_t index);
        const Card& operator[](const size_t index) const;

        void shuffle();

};



