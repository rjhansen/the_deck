#include "decky.h"
#include <print>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc == 0)
        process(std::cin, Deck(1), Opmode::ENCRYPT);
    else {
        std::ifstream foo(argv[1]);
        process(foo, Deck(1), Opmode::ENCRYPT);
    }

    return 0;
}