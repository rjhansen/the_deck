#ifndef DECKY_H
#define DECKY_H
#ifdef _WIN32
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API
#endif

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <mutex>
#include <numeric>
#include <random>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

namespace The_Deck {
/** Defines two constants which may be used to specify encryption or
 * decryption in a typesafe way.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
enum class DLL_API Opmode { ENCRYPT = 0,
    DECRYPT };

/** A convenience class that encapsulates typesafe information about
 * a card’s rank and suit. It also supports ordering and comparison
 * operations.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
struct DLL_API Card {
    /** Following standard poker rules, there are five possible suits
     * once the joker cards are included in the deck.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    enum class DLL_API Suit { CLUB = 0,
        DIAMOND,
        HEART,
        SPADE,
        NONE };

    /** Whether aces are high or low is a house rule issue. For our
     * purposes implementing Solitaire, it works best as a zero.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    enum class DLL_API Rank {
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

    /** This is exposed to the user for ease of debugging. Please don’t take
     * it as an invitation to change it. Please note that in order to support
     * card assignments, this cannot be made const.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    Suit SUIT;

    /** This is exposed to the user for ease of debugging. Please don’t take
     * it as an invitation to change it. Please note that in order to support
     * card assignments, this cannot be made const.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    Rank RANK;

    /** This zero-argument exists to allow new Cards to be created implicitly,
     * which is sometimes a prerequisite for data structures fed into the STL.
     * Please don’t use it. Use one of the other constructors instead.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    Card()
        : SUIT { Suit::CLUB }
        , RANK { Rank::TWO }
    {
    }

    Card(Suit suit, Rank rank)
        : SUIT { suit }
        , RANK { rank }
    {
    }

    /** This is exposed to the user for ease of debugging.
     *
     * @throws std::range_error if the integral argument is outside the range
     * (0, 51) inclusive.
     * @bug Can’t be used to create jokers.
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     * @param rank The integer value representing the card to be created.
     */
    explicit Card(const int32_t rank)
        : SUIT { static_cast<Suit>(std::clamp(rank / 13, 0, 3)) }
        , RANK { static_cast<Rank>(rank % 13) }
    {
        if (rank < 0 || rank > 51)
            throw std::range_error("Card rank must be in the range of 0-51");
    }

    Card(const Card& other) = default;
    Card& operator=(const Card& other) = default;

    bool operator<(const Card& other) const;
    bool operator==(const Card& other) const;

    [[nodiscard]]
    /** Returns this object’s value in the range (0, 51) inclusive.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    int32_t card_as_int() const;
};

/** Exists principally for debugging purposes. It’s not part of the
 * API and may be dropped at any time.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API std::ostream& operator<<(std::ostream& stream, const Card& card);

/** Represents a deck of cards and supports many standard deck operations.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
struct DLL_API Deck {
private:
    inline static std::mt19937 gen { std::random_device{}() };
    static const Card JOKER_A;
    static const Card JOKER_B;
    inline static std::mutex gen_mutex;

public:
    /**  This is public to facilitate third parties (you) doing sneaky
     * stuff with the cards. Please don’t tamper with this part lightly,
     * as all manner of things can break.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    std::vector<Card> deck;

    enum class Kind { WITHOUT_JOKERS = 0,
        WITH_JOKERS = 1 };

    explicit Deck(Kind k = Kind::WITHOUT_JOKERS)
    {
        std::vector<int> zero_to_51(52);
        std::iota(zero_to_51.begin(), zero_to_51.end(), 0);
        std::ranges::transform(zero_to_51, std::back_inserter(deck),
            [](const int x) { return Card(x); });
        if (k == Kind::WITH_JOKERS) {
            deck.push_back(JOKER_A);
            deck.push_back(JOKER_B);
        }
    }

    Deck(const Deck& other) = default;

    /** Used to initialize a deck from any generic sequence of cards,
     * including arrays of them.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    explicit Deck(const std::span<const Card>& other_deck)
    {
        deck.clear();
        std::ranges::copy(other_deck, std::back_inserter(deck));
    }

    /** Used to do a bounds-checked peek into a deck. Useful for debugging,
     * and also shenanigans.
     *
     * @throws std::out_of_range if a bounds violation occurs.
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    Card& operator[](size_t index);

    /** Used to do a bounds-checked peek into a deck. Useful for debugging,
     * and also shenanigans.
     *
     * @throws std::out_of_range if a bounds violation occurs.
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    const Card& operator[](size_t index) const;

    /** Tests whether two decks are card for card identical.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    bool operator==(const Deck& other) const;

    /** Performs a high quality <b>but not cryptologically secure</b>
     * shuffle on the cards using the Mersenne Twister algorithm.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     * @bug This is not cryptographically secure!
     */
    void shuffle();

    /** Sorts the deck according to the cards’ underlying comparison operator.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void sort();

    /** Can be used to deal one card from anywhere in the deck. Useful for
     * shenanigans.
     *
     * @throws std::out_of_range if a bounds violation occurs.
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    Card deal(long position);

    /** Inserts a card anywhere in the deck. Useful for shenanigans, and
     * extremely useful for implementing Solitaire.
     *
     * @bug Attempting to access a nonexistent position just adds the card
     * to the end of the deck instead of throwing a std::range_error.
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void insert(const Card& card, long position);

    /** Performs a Solitaire triple cut.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void triple_cut();

    /** Performs a Solitaire bury-one operation.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void bury_1_with_wraparound(const Card& card);

    /** Performs a Solitaire bury-N operation.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void bury_with_wraparound(const Card& card, size_t slots_down);

    /** Buries Joker-A according to Solitaire rules.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void bury_joker_a();

    /** Buries Joker-B according to Solitaire rules.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void bury_joker_b();

    /** Performs a Solitaire count-cut.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void count_cut();
    [[nodiscard]]

    /** Returns the value of the top card as a Solitaire keystream value.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    uint32_t get_keystream_value() const;

    /** Useful for debugging.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void dump() const
    {
        std::ranges::copy(deck, std::ostream_iterator<Card>(std::cout, " | "));
    }

    /** Removes all cards that meet a given predicate.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    void remove_if(auto Pred)
    {
        auto e = std::ranges::remove_if(
            deck, Pred); // Arranges the vector such that the things
        deck.erase(e.begin(), e.end());
    }

    [[nodiscard]]
    /** Returns the current size of the deck.
     *
     * @since December 2024
     * @author Eugene Libster <elibster@gmail.com>
     */
    size_t size() const
    {
        return deck.size();
    }
};

/** Returns the next Solitaire keystream value from the deck,
 * in a 1..N format.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API uint8_t get_raw_keystream_value(Deck& deck);

/** Returns the next Solitaire keystream value from the deck,
 * in range (1, 26) inclusive.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API uint8_t get_keystream_value(Deck& deck);

/** Converts a string into a sequence of integers ready for
 * Solitaire.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API std::vector<uint8_t> convert_string_to_uint8(std::string input_string);

/** Converts a vector of integers into a string of characters.
 *
 * @bug We really should make this work on a std::span.
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API std::string convert_uint8_to_string(std::span<const uint8_t> input_numbers);

/** Runs the Solitaire algorithm on a given string.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API std::string crypt(const std::string& input, const Deck& deck, Opmode mode);

/** Runs the Solitaire encryption algorithm on a given string.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API std::string encrypt(const std::string& plaintext, const Deck& deck);

/** Runs the Solitaire decryption algorithm on a given string.
 *
 * @since December 2024
 * @author Eugene Libster <elibster@gmail.com>
 */
DLL_API std::string decrypt(const std::string& ciphertext, const Deck& deck);

/** Provides an STL-friendly interface to the Solitaire algorithm.
 *
 * @since January 2025
 * @author Rob Hansen <rob@hansen.engineering>
 */
template <std::input_iterator T, std::output_iterator<uint8_t> U>
void stl_crypt(T begin, T end, U output, const Deck& deck, Opmode mode)
{
    /* Compile-time sanity check */
    static_assert(sizeof(decltype(*begin)) == 1);

    /* Runtime sanity checks */
    if (end == begin)
        return;

    Deck d(deck);

    auto keystream = [&](uint8_t c) -> uint8_t {
        auto deck_val = get_keystream_value(d);
        if (mode == Opmode::ENCRYPT) {
            uint8_t v = c + deck_val;
            while (v > 26)
                v -= 26;
            return 'A' - 1 + v;
        }
        // Narrowing conversions from unsigned integers to signed ones are
        // technically implementation-defined, except in the case where the
        // values lie within the range of the narrowed type. That's the case
        // here.
        int8_t v = static_cast<int8_t>(c) - static_cast<int8_t>(deck_val);
        while (v < 0)
            v += 26;
        return 'A' - 1 + v;
    };

    std::string working_copy { begin, end };
    std::transform(working_copy.begin(), working_copy.end(), working_copy.begin(),
        [](const auto& x) -> uint8_t { return ::toupper(x); });
    auto filtered_working_copy = std::views::filter(working_copy,
        [](const auto& x) { return (x >= 'A' && x <= 'Z'); });
    std::string codesheet { filtered_working_copy.begin(), filtered_working_copy.end() };
    while (codesheet.size() % 5)
        codesheet += "X";
    const auto result { convert_string_to_uint8(codesheet)
        | std::views::transform(keystream) };
    auto result_iter = result.begin();
    uint32_t index { 0 };
    while (result_iter != result.end()) {
        if (index && (index % 40 == 0))
            *output++ = '\n';
        else if (index && (index % 5 == 0))
            *output++ = ' ';
        index += 1;
        *output++ = *result_iter++;
    }
}

/** Provides another STL-friendly face for Solitaire.
 *
 * @since January 2025
 * @author Rob Hansen <rob@hansen.engineering>
 */
template <std::input_iterator T, std::output_iterator<uint8_t> U>
void solitaire(T begin, T end, U output, const Deck& deck, Opmode mode)
{
    stl_crypt(begin, end, output, deck, mode);
}

/** Provides another STL-friendly face for Solitaire.
 *
 * @since January 2025
 * @author Rob Hansen <rob@hansen.engineering>
 */
DLL_API void solitaire(std::istream&& input, std::ostream& output, const Deck& deck,
    Opmode mode);

/** Provides another STL-friendly face for Solitaire.
 *
 * @since January 2025
 * @author Rob Hansen <rob@hansen.engineering>
 */
DLL_API void solitaire(std::istream& input, std::ostream& output, const Deck& deck,
    Opmode mode);

/** Provides another STL-friendly face for Solitaire.
 *
 * @since January 2025
 * @author Rob Hansen <rob@hansen.engineering>
 */
DLL_API void solitaire(std::istream&& input, std::ostream&& output, const Deck& deck,
    Opmode mode);

/** Provides another STL-friendly face for Solitaire.
 *
 * @since January 2025
 * @author Rob Hansen <rob@hansen.engineering>
 */
DLL_API void solitaire(std::istream& input, std::ostream&& output, const Deck& deck,
    Opmode mode);
} // namespace The_Deck
#endif