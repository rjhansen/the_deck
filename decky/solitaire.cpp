#include <print>
#include "decky.h"

uint32_t get_keystream(Deck& deck)
{
    uint32_t ks_val = 53;

    while(ks_val == 53)
    {
        deck.bury_joker_a();
        deck.bury_joker_b();
        deck.triple_cut();
        deck.count_cut();
        ks_val = deck.get_keystream_value();
    }
    return ks_val;
}

int main(void)
{
    auto deck = Deck();
    auto joker_a = Card(Suit::NONE, Rank::JOKER_A);
    auto joker_b = Card(Suit::NONE, Rank::JOKER_B);

    deck.insert(joker_a, 52);
    deck.insert(joker_b, 10);

    for (int i = 0; i < 10; i++)
        std::print("{} ", get_keystream(deck));
    std::print("\n");
}