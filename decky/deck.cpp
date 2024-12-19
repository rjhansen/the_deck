#include <stdexcept>

#include "decky.h"

const Card Deck::JOKER_A(Suit::NONE, Rank::JOKER_A);
const Card Deck::JOKER_B(Suit::NONE, Rank::JOKER_B);

Card& Deck::operator[](const size_t index)
{
    return deck.at(index);
}

const Card& Deck::operator[](const size_t index) const
{ 
    return deck.at(index);
}

void Deck::shuffle()
{
    std::shuffle(deck.begin(), deck.end(), gen);
}

void Deck::sort()
{
    std::sort(deck.begin(), deck.end());
}

Card Deck::deal(const size_t position)
{
    Card card(deck.at(position));
    deck.erase(deck.begin() + position);
    return card;
}

void Deck::insert(const Card& card, const size_t position)
{
    if(position >= deck.size())
        // If someone gives us a position that's too large, just stick the card at the back of the deck
        deck.push_back(card);
    else
        deck.insert(deck.cbegin() + position, card);
}

void Deck::triple_cut()
{
    auto first_joker = std::find_if(deck.begin(),
                                    deck.end(),
                                    [](const auto& card){ return (card.RANK == Rank::JOKER_A || card.RANK == Rank::JOKER_B);}
                                    );
    if(first_joker == deck.cend())  // Test if there are not enough jokers in the deck
        throw std::logic_error("No jokers found while trying to perform a triple cut. We need two.");

    auto second_joker = std::find_if(first_joker + 1,
                                        deck.end(),
                                        [](const auto& card){ return (card.RANK == Rank::JOKER_A || card.RANK == Rank::JOKER_B);}
                                    );
    if(second_joker == deck.cend())
        throw std::logic_error("Only one joker found while trying to perform a triple cut. We need two.");
    

    // If you don't want to have to type std:: in front of every std function,
    // you can import it at the top of the file using a command like
    // using std::vector;
    // Do this for every std:: name/function/object you intend to use a lot
    std::vector<Card> to_first_joker(deck.begin(), first_joker);
    std::vector<Card> after_second_joker(second_joker+1, deck.end());

    // Remove the parts outside the jokers from the deck
    deck.erase(second_joker+1, deck.end()); // Erase the second half first to avoid making the first iterator invalid
    deck.erase(deck.begin(), first_joker);

    // Append the middle portion after the original last portion, and then append the first portion to that
    std::copy(deck.cbegin(), deck.cend(), std::back_inserter(after_second_joker));
    std::copy(to_first_joker.cbegin(), to_first_joker.cend(), std::back_inserter(after_second_joker));

    deck = after_second_joker;
}

void Deck::bury_1_with_wraparound(const Card& card)
{
    auto card_location = std::find(deck.begin(), deck.end(), card);

    if(card_location == deck.end())
        throw std::logic_error("Card not found");

    // More terse, less legible
    //std::swap(*card_location, card_location == deck.end()-1 ? *(deck.begin()) : *(card_location+1));

    if(card_location == (deck.end()-1))
    {
        auto last_card = *(deck.end()-1);
        deck.pop_back();
        deck.insert(deck.begin(), last_card);
    }
    else
        std::swap(*card_location, *(card_location+1));
}

void Deck::bury_with_wraparound(const Card& card, const size_t slots_down)
{
    for(size_t i=0; i<slots_down; i++)
        bury_1_with_wraparound(card);
}

void Deck::bury_joker_a()
{
    //down by 1 slot unless last card, then down by 2
    auto joker_a_location = std::find(deck.begin(), deck.end(), JOKER_A);
    auto joker_a = Card(Suit::NONE, Rank::JOKER_A);
    
    if(joker_a_location == (deck.end()-1))
        bury_1_with_wraparound(joker_a);
    bury_1_with_wraparound(joker_a);
}

void Deck::bury_joker_b()
{
    //down by 1 slot unless last card, then down by 2
    auto joker_b_location = std::find(deck.begin(), deck.end(), JOKER_B);
    auto joker_b = Card(Suit::NONE, Rank::JOKER_B);
    
    if(joker_b_location == (deck.end()-2) || joker_b_location == (deck.end()-1))
        bury_1_with_wraparound(joker_b);
    bury_with_wraparound(joker_b, 2);
}

void Deck::count_cut()
{
    const Card& last_card = *(deck.end()-1);
    auto index = last_card.card_as_int() + 1; // The cards ALWAYS move by at least 1 position

    if(index == deck.size())
        return;

    std::vector<Card> temp_cards(deck.begin(), deck.begin() + index);
    deck.erase(deck.begin(), deck.begin()+index);
    deck.insert(deck.end()-1, temp_cards.begin(), temp_cards.end());
}

uint32_t Deck::get_keystream_value()
{
    auto index = deck.begin()->card_as_int() + 1; // Solitaire's value of a card is 1 more than ours, so count an extra card down
    return static_cast<uint32_t>(deck.at(index).card_as_int()) + 1; // return solitaire's value rather than ours
}