#include "decky.h"
#include <fstream>

using std::cin;
using std::cout;
using std::ifstream;

using The_Deck::Deck;
using The_Deck::Opmode;
using The_Deck::solitaire;

int main(int argc, char* argv[])
{
    auto deck = Deck(Deck::Kind::WITH_JOKERS);
    auto mode = Opmode::ENCRYPT;

    if (argc == 1) {
        solitaire(cin, cout, deck, mode);
    } else {
        solitaire(ifstream(argv[1]), cout, deck, mode);
    }
    cout << "\n";

    return 0;
}