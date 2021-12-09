#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "Artists.h"
#include "Graph.h"
#include <SFML/Graphics.hpp>
using namespace std;

void visual(string input, string list1, string list2){

    sf::RenderWindow window(sf::VideoMode(1800, 1400), "sound geeks");
    sf::Color boxes(232, 232, 207);
    sf::Color title(102, 102, 150);
    sf::Color text(102, 43, 50);
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(1500, 150));
    rectangle.setFillColor(boxes);
    rectangle.setPosition(150, 70);
    sf::RectangleShape rectangle2;
    rectangle2.setSize(sf::Vector2f(800, 800));
    rectangle2.setFillColor(boxes);
    rectangle2.setPosition(50, 575);
    sf::RectangleShape rectangle3;
    rectangle3.setSize(sf::Vector2f(800, 800));
    rectangle3.setFillColor(boxes);
    rectangle3.setPosition(950, 575);

    sf::Font font;
    if (!font.loadFromFile("Facetype - Moki-Mono copy.otf")){
        cout << "file not loaded";
    }

    sf::Text head("Rhythm Recommender", font);
    head.setCharacterSize(70);
    head.setStyle(sf::Text::Bold);
    head.setFillColor(title);
    head.setPosition(300, 100);

    sf::Text prompt("You wanted artists like:", font);
    prompt.setCharacterSize(50);
    prompt.setStyle(sf::Text::Bold);
    prompt.setFillColor(boxes);
    sf::FloatRect textRect = prompt.getLocalBounds();
    prompt.setOrigin(textRect.left + textRect.width/2.0f,
                     textRect.top + textRect.height/2.0f);
    prompt.setPosition(900, 300);

    sf::Text artist(input, font);
    artist.setCharacterSize(80);
    artist.setStyle(sf::Text::Bold);
    artist.setFillColor(boxes);
    sf::FloatRect textRect2 = artist.getLocalBounds();
    artist.setOrigin(textRect2.left + textRect2.width/2.0f,
                     textRect2.top + textRect2.height/2.0f);
    artist.setPosition(900, 400);

    sf::Text sub1("artists other people love:", font);
    sub1.setCharacterSize(30);
    sub1.setStyle(sf::Text::Bold);
    sub1.setFillColor(boxes);
    sub1.setPosition(50, 500);

    sf::Text sub2("underground artists you should try:", font);
    sub2.setCharacterSize(23);
    sub2.setStyle(sf::Text::Bold);
    sub2.setFillColor(boxes);
    sub2.setPosition(930, 510);

    sf::Text reset("Reset", font);
    reset.setCharacterSize(30);
    reset.setStyle(sf::Text::Bold);
    reset.setFillColor(boxes);
    sf::FloatRect textRect4 = reset.getLocalBounds();
    reset.setOrigin(textRect4.left + textRect4.width/2.0f,
                    textRect4.top + textRect4.height/2.0f);
    reset.setPosition(900, 300);

    sf::Text recommendations1(list1, font);
    recommendations1.setCharacterSize(25);
    recommendations1.setStyle(sf::Text::Bold);
    recommendations1.setLineSpacing(3.3);
    recommendations1.setFillColor(text);
    recommendations1.setPosition(75, 615);

    sf::Text recommendations2(list2, font);
    recommendations2.setCharacterSize(25);
    recommendations2.setStyle(sf::Text::Bold);
    recommendations2.setLineSpacing(3.3);
    recommendations2.setFillColor(text);
    recommendations2.setPosition(975, 615);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                window.close();
                return;
            }


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { //left button click on tile field
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (position.x < 900 && position.x > 300 &&
                    position.y < 200 && position.y > 100)
                    return;
            }
        }

        window.clear();
        window.draw(rectangle);
        window.draw(rectangle2);
        window.draw(rectangle3);
        window.draw(head);
        window.draw(prompt);
        window.draw(artist);
        window.draw(sub1);
        window.draw(sub2);
        window.draw(recommendations1);
        window.draw(recommendations2);
        window.display();
    }
}
int findArtist(vector<Artists> artList, string& input) {
    int ret = 0;
    for (int i = 0; i < artList.size(); i++) {
        string artTemp = artList[i].name;
        transform(artTemp.begin(), artTemp.end(), artTemp.begin(), ::toupper);
        if (artTemp == input)
            return i;
        if (i == artList.size()-1) {
            cout << "Sorry, we couldn't find your artist. Please check the spelling or try another artist." << endl;
            getline(cin, input);
            transform(input.begin(), input.end(), input.begin(), ::toupper);
            int ret = findArtist(artList, input);
        }
    }
    return ret;
}
void readFile(vector<Artists>& artists){
    string row, name, genre, waste, tempPop;
    int popularity, count = 0;

    ifstream file1("artists.csv");
    if (!file1.is_open()) {
        cout << "Could not open file artists.csv" << endl;
    }
    getline(file1,row);
    while (getline(file1,row)) {
        if (row == ",,") //if empty cell row, skip
            continue;
        if (artists.size() >= 100000) //if hit 100,000 tuples, begin code
            break;

        stringstream rowStream(row);
        getline(rowStream, name, ','); //read in name
        if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ .*!@#$%^&-?") != string::npos)
            continue;
        if (count > 0)
            if (name == artists[artists.size()-1].name) //if name is the same as previous artist's name, skip
                continue;

        getline(rowStream, waste, ','); //take entire cell b
        stringstream tags(waste);
        getline(tags, genre, ';'); //read in first tag as artist's genre
        if (genre.empty()) //if no tags exist, move on to next artists
            continue;

        getline(rowStream, tempPop, ','); //read in scrobbles for popularity
        popularity = stoi(tempPop);

        artists.push_back(Artists(name, genre, popularity));
        count++;
    }
    file1.close();
}
int main() {
    //loading in artists
    cout << "Booting up Rhythm Recommender" << endl;
    vector<Artists> artists;
    cout << "Loading in artists..." << endl;
    readFile(artists);
    cout << "Done!" << endl;

    //constructing graph
    cout << "Constructing the graph, it'll take a minute (sorry)..." << endl;
    Graph graph(artists, artists.size());
    cout << "All done! Ready at your convenience!" << '\n' << endl;

    //variables
    string input, list1, list2;
    int artistNum;
    vector<int> recommended;
    //Start user interaction
    cout << "Welcome to the Rhythm Recommender!" << endl;
    cout << "What’s an artist that you enjoy listening to?" << endl;
    cout << "(Insert artist's name without commas below, or type exit to exit)" << endl;

    getline(cin, input);
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    while (input != "EXIT"){
        //search to find index that holds artist name
        artistNum = findArtist(artists, input);

        cout << "First, we'll conduct a Breadth First Search to show you artists like " << input
             << " (" << artists[artistNum].popularity << " scrobbles)" << endl;
        recommended = graph.bfs(graph, artistNum);
        cout << "== Recommended Artists ==" << endl;
        list1 = "";
        for (int i = 0; i < recommended.size(); i++){
            list1 += artists[recommended[i]].name + '\n';
            //cout << list1;
            /*cout << i+1 << ". " << artists[recommended[i]].name <<
                 " - " << artists[recommended[i]].popularity << " total scrobbles" << endl;*/
        }

        cout << endl;
        recommended.clear();

        cout << "Now, we'll conduct a Depth First Search to compare and see which list is more accurate" << endl;
        cout << "(" << input << " has " << artists[artistNum].popularity << " total scrobbles)" << endl;
        recommended = graph.dfs(graph, artistNum);
        cout << "== Recommended Artists ==" << endl;
        list2 = "";
        for (int i = 0; i < recommended.size(); i++){
            list2 += artists[recommended[i]].name + '\n';
            cout << i+1 << ". " << artists[recommended[i]].name <<
                 " - " << artists[recommended[i]].popularity << " total scrobbles" << endl;
        }
        recommended.clear();

        visual(input, list1, list2);

        cout << '\n' << "What’s an artist that you enjoy listening to?" << endl;
        cout << "(Insert artist's name without commas below, or type exit to exit)" << endl;
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::toupper);
    }

    cout << "Thank you and come again!" << endl;
    return 0;
}
