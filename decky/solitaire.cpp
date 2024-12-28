#include "decky.h"
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>

using std::back_inserter;
using std::istream;
using std::istream_iterator;
using std::ostream;
using std::ostream_iterator;
using std::string;
using std::vector;
using std::ranges::remove_if;
using std::ranges::transform;

uint8_t get_raw_keystream_value(Deck& deck)
{
    uint8_t ks_val = 53;

    while (ks_val == 53) {
        deck.bury_joker_a();
        deck.bury_joker_b();
        deck.triple_cut();
        deck.count_cut();
        ks_val = deck.get_keystream_value();
    }

    return ks_val;
}

uint8_t get_keystream_value(Deck& deck)
{
    uint8_t ks_val = get_raw_keystream_value(deck);

    while (ks_val > 26)
        ks_val -= 26;

    // it's now mathematically impossible for ks to be greater than 26.
    if (ks_val == 0)
        throw std::logic_error("Keystream generator created an invalid value");

    return ks_val;
}

vector<uint8_t> convert_string_to_uint8(string input_string)
{
    vector<uint8_t> output;
    transform(input_string, input_string.begin(), ::toupper);
    auto e = remove_if(input_string,
        [](const auto& x) {
            return (x < 'A' || x > 'Z');
        });
    input_string.erase(e.begin(), e.end());
    while (input_string.length() % 5)
        input_string.append("X"); // input_string += "X"; would also work
    transform(input_string,
        back_inserter(output),
        [](const auto& x) {
            return (static_cast<uint8_t>(x - 'A') + 1);
        });
    return output;
}

string convert_uint8_to_string(const vector<uint8_t>& input_numbers)
{
    string output;
    transform(input_numbers,
        back_inserter(output),
        [](const auto& x) {
            return static_cast<char>(x - 1) + 'A';
        });
    return output;
}

string crypt(const string& input, Deck deck, const Opmode mode)
{
    string output;
    stl_crypt(input.cbegin(),
        input.cend(),
        back_inserter(output),
        deck,
        mode);
    return output;
}

string encrypt(const string& plaintext, const Deck& deck)
{
    return crypt(plaintext, deck, Opmode::ENCRYPT);
}

string decrypt(const string& ciphertext, const Deck& deck)
{
    return crypt(ciphertext, deck, Opmode::DECRYPT);
}

void solitaire(std::istream&& input, std::ostream& output, const Deck& deck, Opmode mode)
{
    auto i = std::istream_iterator<uint8_t>(input);
    auto eof = std::istream_iterator<uint8_t>();
    auto o = std::ostream_iterator<uint8_t>(output);
    solitaire(i, eof, o, deck, mode);
}

void solitaire(std::istream& input, std::ostream& output, const Deck& deck, Opmode mode)
{
    auto i = std::istream_iterator<uint8_t>(input);
    auto eof = std::istream_iterator<uint8_t>();
    auto o = std::ostream_iterator<uint8_t>(output);
    solitaire(i, eof, o, deck, mode);
}

void solitaire(std::istream&& input, std::ostream&& output, const Deck& deck, Opmode mode)
{
    auto i = std::istream_iterator<uint8_t>(input);
    auto eof = std::istream_iterator<uint8_t>();
    auto o = std::ostream_iterator<uint8_t>(output);
    solitaire(i, eof, o, deck, mode);
}

void solitaire(std::istream& input, std::ostream&& output, const Deck& deck, Opmode mode)
{
    auto i = std::istream_iterator<uint8_t>(input);
    auto eof = std::istream_iterator<uint8_t>();
    auto o = std::ostream_iterator<uint8_t>(output);
    solitaire(i, eof, o, deck, mode);
}