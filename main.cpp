#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>
#include <string>

using namespace std;

const string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const string INPUT_SEED = "Enter a seed value: ";
const string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const string INPUT_CARDS = "Enter two cards (x1, y1, x2, y2), or q to quit: ";
const string INVALID_CARD = "Invalid card.";
const string FOUND = "Pairs found.";
const string NOT_FOUND = "Pairs not found.";
const string GIVING_UP = "Why on earth you are giving up the game?";
const string GAME_OVER = "Game over!";

using Game_row_type = vector<Card>;
using Game_board_type = vector<vector<Card>>;

// Muuntaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan

unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for (unsigned int i = 0; i < str.length(); ++i)
    {
        if (not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if (is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}


// Tayttaa pelilaudan tyhjilla korteilla
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Etsii seuraavan tyhjaan kohdan pelilaudalta (g_board) aloittamalla
// annetusta kohdasta start ja jatkamalla tarvittaessa alusta.
// (Kutsutaan vain funktiosta init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{
    // Selvitetaan annetun pelilaudan rivien ja sarakkeiden maarat
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Aloitetaan annetusta arvosta
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }
    // Jatketaan alusta

    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }

    // tanne ei pitaisi paatya
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}


// Alustaa annetun pelilaudan (g_board) satunnaisesti arvotuilla korteilla
// annetun siemenarvon (seed) perusteella.
void init_with_cards(Game_board_type& g_board, int seed)
{
    // Selvitetaan annetun pelilaudan rivien ja sarakkeiden maarat
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Arvotaan taytettava sijainti
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);

    // Hylataan ensimmainen satunnaisluku (joka on aina jakauman alaraja)
    distr(randomEng);

    // Jos arvotussa sijainnissa on jo kortti, valitaan siita seuraava tyhjaa paikka.
    // (Seuraava tyhjaa paikka haetaan kierteisesti funktion next_free avulla.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Lisataan kaksi samaa korttia (parit) pelilaudalle
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}


// Tulostaa annetusta merkista c koostuvan rivin,
// jonka pituus annetaan parametrissa line_length.
// (Kutsutaan vain funktiosta print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Tulostaa vaihtelevankokoisen pelilaudan reunuksineen.
void print(const Game_board_type& g_board)
{
    // Selvitetaan annetun pelilaudan rivien ja sarakkeiden maarat
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    print_line_with_char('=', columns);
    cout << "|   | ";
    for(unsigned int i = 0; i < columns; ++i)
    {
        cout << i + 1 << " ";
    }
    cout << "|" << endl;
    print_line_with_char('-', columns);
    for(unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < columns; ++j)
        {
            g_board.at(i).at(j).print();
            cout << " ";
        }
        cout << "|" << endl;
    }
    print_line_with_char('=', columns);
}


// Kysyy kayttajaö?ta tulon ja sellaiset tulon tekijat, jotka ovat
// mahdollisimman lahella toisiaan.
void ask_product_and_calculate_factors(unsigned int& smaller_factor,
                                       unsigned int& bigger_factor)
{
    unsigned int product = 0;
    while(not (product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);
        product = stoi_with_check(product_str);
    }

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}


// Muutetaan pelilautaa sinne tulleiden koordinaattien avulla
Game_board_type changeBoard(Game_board_type game_board,
                            unsigned int c1,
                            unsigned int c2,
                            unsigned int c3,
                            unsigned int c4) {

    Card& firstcard = game_board.at(c2-1).at(c1-1);
    Card& secondcard = game_board.at(c4-1).at(c3-1);
    firstcard.set_visibility(EMPTY);
    secondcard.set_visibility(EMPTY);

    return game_board;
}

// Virhetarkastelu koordinaateille
bool isCoordinatesTrue(unsigned int c1,
                       unsigned int c2,
                       unsigned int c3,
                       unsigned int c4,
                       Game_board_type board) {

    unsigned int size = 0;
    for(auto i : board){
        size++;
    }
    // jos kortit ovat samoja
    if(c1 == c3 && c2 == c4){
        return false;
    }

    // jos koordinaatit yli laudan
    if(c2 > size or c4 > size){
        return false;
    }

    if(c1-1 >= board.at(0).size() or c3-1 >= board.at(0).size()){
        return false;
    }

    // jos korttia ei ole
    if(board.at(c2-1).at(c1-1).get_visibility() == EMPTY or
            board.at(c4-1).at(c3-1).get_visibility() == EMPTY){
        return false;
    }

    return true;
}

// Kaantaa kortteja
bool turnCards(Game_board_type game_board,
               Player* player,
               int c1, int c2,
               int c3, int c4) {

    bool pairFound = false;
    Card& firstcard = game_board.at(c2-1).at(c1-1);
    Card& secondcard = game_board.at(c4-1).at(c3-1);

    firstcard.turn();
    secondcard.turn();
    print(game_board);

    // jos pari loytyy
    if (firstcard.get_letter() == secondcard.get_letter()) {
        player->add_card(firstcard,secondcard);
        pairFound = true;
        return pairFound;
    }
    else{
        firstcard.turn();
        secondcard.turn();
        return pairFound;
    }

}


// Tutkii, ovatko kaikki paikat tyhjia
// eli onko game over
bool checkIfComplete(Game_board_type game_board) {

    for(auto y : game_board){

        for(auto x : y){
            if(x.get_visibility() != EMPTY){
                return false;
            }
        }
    }
    return true;

}


int main()
{
    Game_board_type game_board;

    unsigned int factor1 = 1;
    unsigned int factor2 = 1;
    ask_product_and_calculate_factors(factor1, factor2);
    init_with_empties(game_board, factor1, factor2);

    // siemenluku
    string seed_str = "";
    std::cout << INPUT_SEED;
    std::getline(std::cin, seed_str);
    int seed = stoi_with_check(seed_str);
    init_with_cards(game_board, seed);

    // montako pelaajaa
    string players_number = "";
    std::cout << INPUT_AMOUNT_OF_PLAYERS;
    std::cin >> players_number;

    // testataan oikeellisuus
    while(!stoi_with_check(players_number)){
        std::cout << INPUT_AMOUNT_OF_PLAYERS;
        std::cin >> players_number;
    }
    int playersnumber = stoi_with_check(players_number);

    string players_names = "";
    // pelaajien nimet vektoriin
    vector<Player*> players = {};
    std::cout << "List " << playersnumber << " players: ";

    for (int i = 1; i <= playersnumber; ++i) {
        std::cin >> players_names;
        Player* newPlayer = new Player(players_names);
        players.push_back(newPlayer);
        continue;
    }

    print(game_board);
    int playernumber = 0;
    int maxnumber = players.size()-1;

    // neljä? haluttua lukua, x1, y1, x2, y2
    std::string coordinate_1, coordinate_2,
            coordinate_3, coordinate_4;


    while(!checkIfComplete(game_board)){
        if(playernumber>maxnumber){
            playernumber=0;
        }

        std::cout << players.at(playernumber)->get_name()
                  << ": " << INPUT_CARDS;
        std::cin >> coordinate_1;
        if (coordinate_1 == "q") {
            cout << GIVING_UP << endl;
            return EXIT_SUCCESS;
        }

        // kaydaan lapi koordinaatit ja muutetaan ne inteiksi
        std::cin >> coordinate_2;
        std::cin >> coordinate_3;
        std::cin >> coordinate_4;
        int c_1, c_2, c_3, c_4;
        c_1 = stoi_with_check(coordinate_1);
        c_2 = stoi_with_check(coordinate_2);
        c_3 = stoi_with_check(coordinate_3);
        c_4 = stoi_with_check(coordinate_4);

        // virhetarkastelu
        if (isCoordinatesTrue(c_1,c_2,c_3,c_4,game_board) == false){
            std::cout << INVALID_CARD << std::endl;
        }
        else{
            // jos pari loytyy
            if(turnCards(game_board,players.at(playernumber),c_1,c_2,c_3,c_4)){
                game_board = changeBoard(game_board,c_1,c_2,c_3,c_4);
                std::cout << FOUND << std::endl;
            }
            // jos paria ei loydy
            else{
                std::cout << NOT_FOUND << std::endl;
                playernumber++;

            }

            // tulostetaan montako paria pelaajilla on
            for(unsigned long int i=0; i<players.size();i++){
                std::cout << "*** " << players.at(i)->get_name() << " has "
                             << players.at(i)->number_of_pairs() << " pair(s)."
                             << std::endl;
            }
            // tulostetaan pelilaudan tilanne
            print(game_board);


        }
    }
    // kun peli paattyy
    std::cout << GAME_OVER << std::endl;
    unsigned int maxpoints = 0;
    std::string maxname = "";
    unsigned int amount = 0;
    vector<unsigned int> points = {};

    for (unsigned long int i = 0; i<players.size(); i++) {
        if( players.at(i)->number_of_pairs()>=maxpoints){
            maxpoints = players.at(i)->number_of_pairs();
            maxname = players.at(i)->get_name();
            points.push_back(maxpoints);
        }
    }
    for (unsigned long int i=0;i<points.size();i++){
        if(points.at(i) == maxpoints){
            amount++;
        }
    }
    // jos vain yksi voittaja
    if(amount == 1){
        std::cout << maxname << " has won with " << maxpoints << " pairs." << std::endl;
    }
    // jos tasapeli
    else{
        std::cout << "Tie of " << amount << " players with " << maxpoints << " pairs." << std::endl;
    }
    return EXIT_SUCCESS;
}
