/*
 * COMP.CS.110 PROGRAMMING 2: structures
 *
 * Name 1: Iiris Palviainen
 * Name 2: Sini Kivioja

 *
 * The program gives information about
 * the Tampere's tram and gives access
 * to changing the tramlines & stops.
 * The stops and tramlines can be deleted
 * & added, the distances between two
 * stops can be fetched and the stops &
 * lines can be listed.
 *
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <set>

using namespace std;

// Struct that contains all the information of the stops
struct Tramstop {
    std::string name;
    double distance;
};

using Data_structure = map<string, vector<Tramstop>>;


// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// split-function
vector<string> split(const string& line,
                     char separator,
                     bool ignoreEmpty = false) {
    vector<string> parts;

    size_t begin = 0;
    size_t end = 0;

    while (end != line.size()) {

        end = line.find(separator, begin);

        if (end == string::npos) {
            end = line.size();
        }

        if (ignoreEmpty || begin != end) {
            parts.push_back(line.substr(begin, end - begin));
        }
        begin = end + 1;
    }

    for(long unsigned int k = 0; k < parts.size()-1; k++){
        if(parts.at(k)[0] == '"'){
            parts.at(k) = parts.at(k) + " " + parts.at(k+1);
            parts.erase(parts.begin()+k+1);

            break;
        }
    }
    return parts;
}

// Prints all the lines in ASCII order
void printLines(const Data_structure& data) {
    for (const auto & pair : data) {
        cout << pair.first << endl;
    }
}

// Printing a single line
void printSingleLine(Data_structure& data, string line){
    for(auto stop : data.at(line) ){
        cout << "- " << stop.name << " : " << stop.distance << endl;
    }
}

// Printing all the stops in ASCII order
void printStops(Data_structure& data) {
    vector<string> stops = {};
    for(auto line : data){
        for(auto stop : line.second){
            stops.push_back(stop.name);
        }

    }
    set<string> stopNames(stops.begin(),stops.end());
    stops.assign(stopNames.begin(), stopNames.end());

    //Printing all the stops from a vector.
    for(long unsigned int index=0; index<stops.size(); index++){
        cout << stops.at(index) << endl;

    }
}

// Adding a new line
void addLine(Data_structure& data, string line) {
    vector<Tramstop> empty_line = {};
    data.insert(make_pair(line, empty_line));
}

// Printing a single stop
void printSingleStop(Data_structure& data,string stop) {
    vector<string> lines = {};

    for(auto line : data){
        for(auto stopInData : line.second){
            if(stopInData.name == stop){
                lines.push_back(line.first);
            }
        }
    }
    // If the stop does not exist
    if(lines.empty()){
        cout << "Error: Stop could not be found." << endl;
        return;
    }
    // If the stop exists
    cout << "Stop " << stop << " can be found on "
                               "the following lines: " << endl;

    for(unsigned long int i = 0; i < lines.size(); i++){
        cout << "- " << lines.at(i) << endl;
    }
}

// Calculating the distance between two stops
void calculateDistance(Data_structure& data, string line,
                       string firstStop, string secondStop) {
    if(data.find(line) ==  data.end()){
        cout << "Error: Line could not be found." << endl;
        return;
    }

    // initializing the needed variables
    bool stop1Found = false;
    bool stop2Found = false;
    double distance_1 = 0;
    double distance_2 = 0;
    double distance = 0;

    for(auto stop : data.at(line)){
        if(stop.name == firstStop){
            distance_1 = stop.distance;
            stop1Found = true;
        }
        if(stop.name == secondStop){
            distance_2 = stop.distance;
            stop2Found = true;
        }
    }
    // if the stops cannot be found
    if(stop1Found == false or stop2Found == false){
        cout << "Error: Stop could not be found" << endl;
        return;
    }
    if(firstStop == secondStop){
        cout << "Distance between " << firstStop
             << " and " << secondStop << " is 0" << endl;
        return;
    }
    if(distance_2 > distance_1){
        distance = distance_2 - distance_1;
        cout << "Distance between " << firstStop
             << " and " << secondStop << " is "
             << distance << endl;
        return;
    }
    distance = distance_1 - distance_2;
    cout << "Distance between " << firstStop
         << " and " << secondStop << " is "
         << distance << endl;
}

// Adding a new stop
void addStop(Data_structure& data, string line,
             string stop, double distance) {
    bool lineFound = false;

    for(auto lineData : data) {
        if(lineData.first == line) {
            lineFound = true;
        }
    }
    if(!lineFound){
        cout << "Error: Line could not be found." << endl;
        return;
    }

    for(unsigned long int index = 0; index
        < data.at(line).size(); index++) {
        if(data.at(line).at(index).name == stop or
                data.at(line).at(index).distance == distance) {
            cout << "Error: Stop/line already exists." << endl;
            return;
        }
    }
    for (long unsigned int i = 0; i < data.at(line).size(); i++) {
        if(data.at(line).at(i).distance > distance) {

            Tramstop temp = {};
            temp.name = stop;
            temp.distance = distance;
            data.at(line).insert(data.at(line).begin() + i, temp);
            cout << "Stop was added." << endl;
            return;
        }
    }
    Tramstop temp = {};
    temp.name = stop;
    temp.distance = distance;
    data.at(line).insert(data.at(line).end(), temp);
    cout << "Stop was added." << endl;
}

// Removes the wanted stop
void removeStop(Data_structure& data, string stop) {
    bool stopFound = false;
    for (auto line : data) {
        for (long unsigned int i = 0;
             i < line.second.size(); i++) {
            if (data.at(line.first).at(i).name == stop) {
                data.at(line.first).erase
                        (data.at(line.first).begin()+i);
                stopFound = true;
                break;
            }
        }
    }
    if (stopFound == false) {
        cout << "Error: Stop could not be found." << endl;
        return;
    }
    cout << "Stop was removed from all lines." << endl;
}

// Checks the stops
bool checkStops(const Data_structure &data,
                string line, string stop,
                double distance) {
    for (auto stopInData : data.at(line)) {
        if (stopInData.name == stop or stopInData.distance == distance){
            return false;
        }
    }
    return true;
}

// Checks invalid arguments
bool is_stod(const string& s) {
    try {
        stod(s);
    }
    catch(invalid_argument) {
        return false;
    }
    return true;
}

// Parses the input file and stores the
// information into the right data structures
Data_structure parseFile(string filename,
                         Data_structure& data){
    // Creates a list from the file input by user.
    // Data from open file is stored into map,
    // which consists of string and another map.

    const char* delimiter = ";";
    // Delimiter is used in input file to separate fields.
    string row = "";

    ifstream file;
    file.open(filename, ios::in);

    if(file.is_open()){

        while(getline(file, row)) // splits rows into variables
        {
            string line = row.substr(0,row.find(delimiter));
            string nextpart = row.substr(row.find(delimiter)+1);
            string stopname = nextpart.substr(0, nextpart.find(delimiter));
            nextpart = nextpart.substr(nextpart.find(delimiter)+1);
            string distance = nextpart.substr(0, nextpart.find(delimiter));

            if(line == "" or stopname == ""){
                // If the file has empty fields, prints error
                data.clear();
                cout << "Error: Invalid format in file." << endl;
                return data;
            }
            if(distance==stopname){
                distance = "";
            }
            int delimiter_amount = 0;
            for(unsigned int i=0; i < row.size(); i++){
                if (row.at(i) == *delimiter){
                    delimiter_amount++;
                }
            }

            if(delimiter_amount > 2){
                // If the file doesn't have exactly 3 fields, prints error
                data.clear();
                cout << "Error: Invalid format in file." << endl;
                return data;
            }

            // checks for any invalid formats in file
            if (!is_stod(distance) && distance != "") {
                cout << "Error: Invalid format in file" << endl;
                data.clear();
                return data;
            }

            vector<Tramstop> Tramstops = {};

            if (data.find(line) == data.end()){
                Tramstop stop = {};
                stop.name = stopname;
                if (distance==""){
                    stop.distance = 0;
                    Tramstops.push_back(stop);
                    data[line] = Tramstops;
                }
                else {
                    stop.distance = stod(distance);
                    Tramstops.push_back(stop);
                    data[line] = Tramstops;
                }
            }

            else {
                if(checkStops(data,line,stopname,stod(distance))){
                    Tramstop stop = {};
                    stop.name = stopname;
                    if(distance==""){
                        stop.distance = 0;
                        data.at(line).push_back(stop);
                    }
                    else{
                        stop.distance = stod(distance);
                        data.at(line).push_back(stop);
                    }
                }
                else{
                    cout << "Error: Stop/line already exists." << endl;
                    data.clear();
                    return data;
                }
            }

        }
        file.close();
    }
    else {
        cout << "Error: File could not be read." << endl;
        data.clear();
        return data;
    }
    return data;
}

bool userInterface() {
    Data_structure data = {};

    // asking user the file's name
    std::string filename = "";
    std::cout << "Give a name for input file: ";
    getline(std::cin, filename);

    parseFile(filename, data);

    if(data.empty()){
        return false;
    }

    while (true) {
        std::string userCommand = "";
        std::cout << "tramway> ";
        getline(std::cin, userCommand);

        vector<string> commands = split(userCommand, ' ', true);

        transform(commands.at(0).begin(),commands.at(0).end(),
                  commands.at(0).begin(), ::toupper);
        string command = commands.at(0);

        // QUIT ends the program
        if (commands.at(0) == "QUIT") {
            return true;
        }

        // LINES prints the tramlines in alphapethical order
        else if (commands.at(0) == "LINES") {
            std::cout << "All tramlines in alphabetical order: " << std::endl;
            printLines(data);

        }

        // LINE prints the wanted tramline (stops, distances)
        // in the order they are listed by
        else if (command == "LINE") {
            if (commands.size() != 2) {
                cout << "Error: Invalid input." << endl;
                continue;

            } else {
                string input = "";
                input = commands.at(1);
                // Removing "'s
                input.erase(remove(input.begin(),
                                   input.end(),'"'),
                            input.end());

                // if the line is not found from data
                if (data.find(input) == data.end()) {
                    cout << "Error: Line could not be found." << endl;
                    continue;
                }
                else {
                    std::cout << "Line " << commands.at(1) <<
                                 " goes through these stops in the order they are listed:"
                              << std::endl;
                    printSingleLine(data, commands.at(1));
                    continue;
                }
            }
        }

        // STOPS prints stops in alphabetical order
        else if (commands.at(0) == "STOPS") {

            // if the input is invalid
            if (commands.size() != 1) {
                cout << "Error: Invalid input." << endl;
                continue;
            } else {
                cout << "All stops in alphabetical order:" << endl;
                printStops(data);
            }
        }

        // STOP prints the lines that the stop is included in
        else if (command == "STOP") {
            if(commands.size() != 2){
                cout << "Error: Invalid input." << endl;
                continue;
            }

            // Removing "'s
            string input = commands.at(1);
            input.erase(remove(input.begin(),input.end(), '"'), input.end());

            printSingleStop(data, input);
        }

        // DISTANCE calculates the distance between two stops
        else if (commands.at(0) == "DISTANCE" and commands.size() > 3) {
            string line = "";
            string firstStop = "";
            string secondStop = "";
            line = commands.at(1);
            firstStop = commands.at(2);
            secondStop = commands.at(3);

            // Removing "'s
            line.erase(remove(line.begin(),line.end(), '"'), line.end());
            firstStop.erase(remove(firstStop.begin(),firstStop.end(), '"'),
                            firstStop.end());
            secondStop.erase(remove(secondStop.begin(),secondStop.end(), '"'),
                             secondStop.end());

            calculateDistance(data, line, firstStop, secondStop);
        }

        // ADDLINE adds a line to the tramway
        else if (commands.at(0) == "ADDLINE") {
            if (commands.size() < 2) {
                cout << "Error: Invalid input." << endl;
                continue;
            }
            if (commands.size() == 2) {
                string newLine = "";
                newLine = commands.at(1);
                // Erasing "'s
                newLine.erase(remove(newLine.begin(),
                                     newLine.end(), '"'), newLine.end());

                // if line already exists
                if (data.find(newLine) != data.end()) {
                    cout << "Error: Stop/line already exists." << endl;

                } else {
                    addLine(data, newLine);
                    cout << "Line was added." << endl;
                }
            } else {
                cout << "Error: Invalid input." << endl;
            }

        }

        // ADDSTOP adds a stop to the line
        else if (commands.at(0) == "ADDSTOP" and commands.size() > 3) {
            string line = "";
            string newStop = "";
            double distance = 0;
            line = commands.at(1);
            newStop = commands.at(2);
            distance = stod(commands.at(3));

            // Removing "'s
            line.erase(remove(line.begin(),line.end(), '"'), line.end());
            newStop.erase(remove(newStop.begin(),newStop.end(), '"'), newStop.end());

            addStop(data, line, newStop, distance);
        }

        // REMOVE removes the wanted stop
        else if (commands.at(0) == "REMOVE" and commands.size() > 1) {
            string input = "";
            input = commands.at(1);

            // Removing "'s
            input.erase(remove(input.begin(),input.end(), '"'),
                        input.end());
            removeStop(data, input);

        } else {
            cout << "Error: Invalid input." << endl;
        }
    }
}

// Short and sweet main.
int main()
{
    print_rasse();
    if (userInterface() == true) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

