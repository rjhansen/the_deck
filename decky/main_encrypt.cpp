#include "decky.h"
#include <fstream>
#include <iostream>
#include <print>

int main(int argc, char* argv[])
{
    if (argc <= 1)
        process(std::cin, Deck(Deck::Kind::WITH_JOKERS), Opmode::ENCRYPT);
    else {
        std::ifstream foo(argv[1]);
        process(foo, Deck(Deck::Kind::WITH_JOKERS), Opmode::ENCRYPT);
    }

    return 0;
}