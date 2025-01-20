#include <ranges>
#include <stdexcept>
#include <tuple>

#include "decky.h"

using std::get;
using std::logic_error;
using std::mt19937;
using std::ostream;
using std::out_of_range;
using std::random_device;
using std::swap;
using std::tuple;
using std::vector;
using std::ranges::all_of;
using std::ranges::copy;
using std::ranges::find;
using std::ranges::find_if;
using std::views::zip;

namespace The_Deck {
mt19937 Deck::gen { random_device {}() };

const Card Deck::JOKER_A(Card::Suit::NONE, Card::Rank::JOKER_A);
const Card Deck::JOKER_B(Card::Suit::NONE, Card::Rank::JOKER_B);

Card& Deck::operator[](size_t index) { return deck.at(index); }

const Card& Deck::operator[](size_t index) const { return deck.at(index); }

bool Deck::operator==(const Deck& other) const
{
    return all_of(zip(deck, other.deck),
        [](const auto& item) { return get<0>(item) == get<1>(item); });
}

void Deck::shuffle() { std::ranges::shuffle(deck, gen); }

void Deck::sort() { std::sort(deck.begin(), deck.end()); }

Card Deck::deal(const long position)
{
    if (position < 0 || position >= deck.size())
        throw out_of_range("Deck::deal: position is out of range");
    // Already bounds-checked in previous line.  No risk exposure here.
    Card card(deck[position]);
    deck.erase(deck.begin() + position);
    return card;
}

void Deck::insert(const Card& card, const long position)
{
    if (position < 0 || position > deck.size())
        throw out_of_range("Deck::deal: position is out of range");
    if (position == deck.size())
        deck.push_back(card);
    else
        deck.insert(deck.cbegin() + position, card);
}

void Deck::triple_cut()
{
    auto first_joker = find_if(deck, [](const auto& card) {
        return (card.RANK == Card::Rank::JOKER_A || card.RANK == Card::Rank::JOKER_B);
    });
    if (first_joker == deck.cend()) // Test if there are not enough jokers in the deck
        throw logic_error(
            "No jokers found while trying to perform a triple cut. We need two.");

    auto second_joker = find_if(first_joker + 1, deck.end(), [](const auto& card) {
        return (card.RANK == Card::Rank::JOKER_A || card.RANK == Card::Rank::JOKER_B);
    });
    if (second_joker == deck.cend())
        throw logic_error("Only one joker found while trying to perform a triple "
                          "cut. We need two.");

    vector<Card> to_first_joker(deck.begin(), first_joker);
    vector<Card> after_second_joker(second_joker + 1, deck.end());

    // Erase the second half first to avoid making the first iterator invalid
    deck.erase(second_joker + 1, deck.end());
    deck.erase(deck.begin(), first_joker);

    copy(deck, back_inserter(after_second_joker));
    copy(to_first_joker.cbegin(), to_first_joker.cend(),
        back_inserter(after_second_joker));

    deck = after_second_joker;
}

void Deck::bury_1_with_wraparound(const Card& card)
{
    auto card_location { find(deck, card) };

    if (card_location == deck.end())
        throw logic_error("Card not found");

    if (card_location == (deck.end() - 1)) {
        auto last_card = *(deck.end() - 1);
        deck.pop_back();
        deck.insert(deck.begin(), last_card);
        card_location = deck.begin();
    }
    swap(*card_location, *(card_location + 1));
}

void Deck::bury_with_wraparound(const Card& card, const size_t slots_down)
{
    for (size_t i = 0; i < slots_down; i++)
        bury_1_with_wraparound(card);
}

void Deck::bury_joker_a() { bury_1_with_wraparound(JOKER_A); }

void Deck::bury_joker_b() { bury_with_wraparound(JOKER_B, 2); }

void Deck::count_cut()
{
    const Card& last_card = *(deck.end() - 1);
    auto index = last_card.card_as_int() + 1;

    if (index == deck.size())
        return;

    vector temp_cards(deck.begin(), deck.begin() + index);
    deck.erase(deck.begin(), deck.begin() + index);
    deck.insert(deck.end() - 1, temp_cards.begin(), temp_cards.end());
}

uint32_t Deck::get_keystream_value() const
{
    // The writeup of Solitaire assumes one-based array indexing,
    // hence our weird +1s here.
    auto index = deck.begin()->card_as_int() + 1;
    return static_cast<uint32_t>(deck.at(index).card_as_int()) + 1;
}

ostream& operator<<(ostream& stream, const Card& card)
{
    stream << (static_cast<int32_t>(card.SUIT)) << " "
           << (static_cast<int32_t>(card.RANK));
    return stream;
}
} // namespace The_Deck
