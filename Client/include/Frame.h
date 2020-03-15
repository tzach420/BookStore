//
// Created by tzachu@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H


#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Frame {
private:
    string stompCommand;
    vector<pair<string,string>> headers;
    string frameBody;

public:
    Frame(string command,vector<pair<string,string>> header,string body);
    string getCommand();
    vector<pair<string,string>> getHeaders();
    string getBody();
    string toString();

};
#endif