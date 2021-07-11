#include "card.hh"
#include <iostream>

Card::Card():
    letter_(EMPTY_CHAR), visibility_(EMPTY)
{

}

Card::Card(const char c):
    letter_(c), visibility_(HIDDEN)
{

}

void Card::set_letter(const char c)
{
    letter_ = c;
}

void Card::set_visibility(const Visibility_type visibility)
{
    visibility_ = visibility;
}

char Card::get_letter() const
{
    return letter_;
}

// nakyvyys
Visibility_type Card::get_visibility() const
{
    return visibility_;
}

// kortin kaanto, onko oikein vai vaarin pain
void Card::turn()
{
    if(visibility_ == HIDDEN)
    {
        visibility_ = OPEN;
    }
    else if(visibility_ == OPEN)
    {
        visibility_ = HIDDEN;
    }
    else
    {
        std::cout << "Cannot turn an empty place."
                  << std::endl;
    }
}

// tulostuksia
void Card::print() const
{
    if(visibility_ == HIDDEN){
        std::cout << "#";
    }else if(visibility_ == OPEN){
        std::cout << letter_;
    }else if(visibility_ == EMPTY){
        std::cout << ".";
    }

}

// poistaa kortin pelilaudalta
void Card::remove_from_game_board()
{
    visibility_ = EMPTY;
}
