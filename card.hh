#ifndef CARD_HH
#define CARD_HH

enum Visibility_type {OPEN, HIDDEN, EMPTY};
const char HIDDEN_CHAR = '#';
const char EMPTY_CHAR = '.';

class Card
{
public:
    // Oletusrakentaja: luo tyhjan kortin.

    Card();

    // Rakentaja: luo piilotetun kortin, jonka kirjaimeksi tulee annettu merkki.

    Card(const char c);

    // Ilmeiset setter- ja getter-metodit.
    void set_letter(const char c);
    void set_visibility(const Visibility_type visibility);
    char get_letter() const;
    Visibility_type get_visibility() const;

    // Kaantaa kortin: vaihtaa nakyvyyden avoimesta piilotettuun ja pain vastoin.
    void turn();

    // Tulostaa kortin sen tamannhetkisen tilanteen (nakyvyyden) mukaisesti.
    void print() const;

    // Poistaa kortin pelilaudalta: muuttaa nakyvyyden tyhjaksi.
    void remove_from_game_board();

private:
    char letter_;
    Visibility_type visibility_;
};

#endif // CARD_HH
