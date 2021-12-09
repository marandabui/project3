#include <iostream>
#pragma once
using namespace std;

struct Artists{
    string genre;
    int popularity;

    Artists(string name, string genre, int popularity){
        this->name = name;
        this->genre = genre;
        this->popularity = popularity;
    }

    Artists(){
        this->name = "";
        this->genre = "";
        this->popularity = 0;
    }

    string name;
};