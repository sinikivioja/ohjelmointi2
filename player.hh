#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>

class Player
{
public:
    // Rakentaja: luo annetun nimisen pelaajan.

    Player(const std::string& name);

    // Palauttaa pelaajan nimen.

    std::string get_name() const;

    // Palauttaa pelaajan tahan asti keranneiden parien maaran

    unsigned int number_of_pairs() const;

    // Siirtaa annetun kortin pelilaudalta pelaajalle,
    // eli lisaa kortin pelaajan keraamiin kortteihin
    // ja poistaa sen pelilaudalta.
    void add_card(Card& card1,Card& card2);

    // Tulostaa pelaajan tilanteen: nimen ja t???h???n asti ker???ttyjen parien m??????r???n.
    void print() const;

private:
    // pelaaja
    std::string player_;
    // parien lkm
    unsigned int number_of_pairs_;

};

#endif // PLAYER_HH
