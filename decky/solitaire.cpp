#include <print>
#include "decky.h"

uint32_t get_keystream_value(Deck& deck)
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

    if(ks_val > 26)
        ks_val -= 26;

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
        std::remove_if(input_string.begin(), input_string.end(), [](const auto& x){return (x < 'A' || x > 'Z');}), // C-style check is easiest for anything other than a capital char
        input_string.end()
    );
    // remove_if() doesn't remove anything, but instead moves it all to the back and returns an iterator to the beginning of the data to be removed
    // erase() takes that iterator and erases that data
    
    std::transform(input_string.cbegin(), input_string.cend(), std::back_inserter(output), [](const auto& x){return (static_cast<uint8_t>(x-'A')+1);});

    return output;
}


int main(void)
{
    auto deck = Deck(1);

    for (int i = 0; i < 10; i++)
        std::print("{} ", get_keystream_value(deck));
    std::print("\n");
}