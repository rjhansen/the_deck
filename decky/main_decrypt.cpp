#include "decky.h"
#include <fstream>
#include <iostream>
#include <print>

int main(int argc, char* argv[])
{
    if (argc <= 1)
        process(std::cin, Deck(Deck::Kind::WITH_JOKERS), Opmode::DECRYPT);
    else {
        std::ifstream foo(argv[1]);
        process(foo, Deck(Deck::Kind::WITH_JOKERS), Opmode::DECRYPT);
    }

    return 0;
}