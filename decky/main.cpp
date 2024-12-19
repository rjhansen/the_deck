#include <print>
#include "decky.h"

int main(void)
{
    auto deck = Deck(1);

    for (int i = 0; i < 10; i++)
        std::print("{} ", get_keystream_value(deck));
    std::print("\n");
}