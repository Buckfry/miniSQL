// name: cd.h
// author: amrzs
// date: 20140427

#ifndef CD_H
#define CD_H

#include <iostream>

#include "media.h"

using namespace std;

class Cd : public Media{

public:
    Cd(string title, string artist, int trackNum):
        Media(title),
        artist(artist),
        trackNum(trackNum){
    }
    ~Cd(){}

private:
    string artist;
    int trackNum;

public:
    virtual void showPros(){

        cout << "CD" << endl;
        cout << "Titile:\t" << getTitle() << endl;
        cout << "Artist:\t" << artist << endl;
        cout << "Num of tracks:\t" << trackNum << endl; 
    }

};

#endif // CD_H
