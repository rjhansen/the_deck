#include "decky.h"
#include <print>
#include <iostream>
#include <fstream>


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

    if (ks_val > 26)
        ks_val -= 26;
    
    if (ks_val < 1 || ks_val > 26)
        throw std::logic_error("Keystream generator created an invalid value");

    return ks_val;
}

std::vector<uint8_t> convert_string_to_uint8(std::string input_string)
{
    // when doing a non-reference pass by copy, C++ will make a deep copy of the underlying data structure
    // This means that we have our own copy of input_string to play with however we need.

    std::vector<uint8_t> output;

    // uppercase every character and remove punctuation and spaces
    // when a function starts with a bare :: (such as ::toupper) it denotes, by convention that we
    // are calling out to the standard C library, which has been integrated into C++.
    // This is also the reason why you should NEVER do "using namespace::std;", as it would cause
    // the std::toupper to override the ::toupper function.
    std::transform(input_string.cbegin(), input_string.cend(), input_string.begin(), ::toupper);
    input_string.erase(
        // remove_if() doesn't remove anything, but instead moves it all to the back and returns an iterator to the beginning of the data to be removed
        // erase() takes that iterator and erases that data
        std::remove_if(input_string.begin(), input_string.end(), [](const auto& x) { return (x < 'A' || x > 'Z'); }), // C-style check is easiest for anything other than a capital char
        input_string.end());

    // Schneier's website specifies that inputs must have a length divisible by 5 and recommends padding with the letter X if they are not
    while (input_string.length() % 5)
        input_string.append("X"); // input_string += "X"; would also work

    std::transform(input_string.cbegin(), input_string.cend(), std::back_inserter(output), [](const auto& x) { return (static_cast<uint8_t>(x - 'A') + 1); });

    return output;
}

std::string convert_uint8_to_string(const std::vector<uint8_t>& input_numbers)
{
    std::string output;

    std::transform(input_numbers.cbegin(),
        input_numbers.cend(),
        std::back_inserter(output),
        [](const auto& x) {
            return static_cast<char>(x - 1) + 'A';
        });

    return output;
}

std::string crypt(const std::string& input, Deck deck, const Opmode mode)
{
    // We pass 'deck' by value so that our manipulations don't mess up the state of the caller's deck.

    // Lambda notation for "no access to enclosing scope"
    //auto Pred = mode==Opmode::ENCRYPT?
    //    [](uint8_t c, uint8_t k){ uint8_t v = c + k; if(v > 26) v-=26; return v;} :
    //    [](uint8_t c, uint8_t k){ int8_t v = static_cast<int8_t>(c) - static_cast<int8_t>(k); if (v < 0) v+=26; return static_cast<uint8_t>(v);};

    std::vector<uint8_t> output;
    auto numbers = convert_string_to_uint8(input);
    // Lambda notation for "inherit enclosing scope by reference".
    // If we start the lambda with [=], it would pass enclosing scope by value, so it would have its own copy
    auto Pred = [&](uint8_t c) {
        auto deck_val = get_keystream_value(deck);
        if (mode == Opmode::ENCRYPT) {
            uint8_t v = c + deck_val;
            while (v > 26)
                v-=26; 
            return v;
        }
        else {
            int8_t v = static_cast<int8_t>(c) - static_cast<int8_t>(deck_val); 
            while (v < 0) 
                v+=26; 
            return static_cast<uint8_t>(v);
        }
    };

    std::transform(numbers.cbegin(), numbers.cend(), std::back_inserter(output), Pred);
    
    return convert_uint8_to_string(output);
}

std::string encrypt(const std::string& plaintext, const Deck& deck)
{
    return crypt(plaintext, deck, Opmode::ENCRYPT);
}

std::string decrypt(const std::string& ciphertext, const Deck& deck)
{
    return crypt(ciphertext, deck, Opmode::DECRYPT);
}

void process(std::istream& stream, const Deck& deck, const Opmode mode)
{
    std::string line;
    std::string message;

    while(stream){
        std::getline(stream, line);
        message += line;
    }
    std::print(crypt(deck, message, mode));
}