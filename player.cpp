#include "player.hh"
#include <iostream>

// Luokan alustaja
Player::Player(const std::string& name):
    player_(name){

}

// hakee oikean pelaajan nimen
std::string Player::get_name() const
{
    return player_;
}

// montako paria on pelaajalla
unsigned int Player::number_of_pairs() const
{
    return number_of_pairs_;
}

// lisaa parin pelaajalle
void Player::add_card(Card &card1,Card &card2)
{
    number_of_pairs_ += 1;
    card1.remove_from_game_board();
    card2.remove_from_game_board();
}

// tulostukset
void Player::print() const
{
    std::cout << "*** " << player_ << " has " << number_of_pairs()
              << " pair(s)." << std::endl;
}
