#include "decky.h"
#include <print>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::istream& stream = argc == 1 ? std::cin : std::ifstream(argv[1]);

    process(stream, Deck(1), Opmode::ENCRYPT);

    return 0;
}