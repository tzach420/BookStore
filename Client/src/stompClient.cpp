//
// Created by tzachu@wincs.cs.bgu.ac.il on 08/01/2020.
//
//updated 15.1.20 00:05
#include <thread>
#include "../include/connectionHandler.h"
#include "../include/inputFromKeyboardHandler.h"
#include "../include/outputFromServerHandler.h"


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/

vector<string> getCommand(string s, string delimiter);
ConnectionHandler* handleFirstLogin();


  int main (int argc, char *argv[]) {
    vector<string> commandVector;
    ConnectionHandler *connectionHandler;
    connectionHandler=handleFirstLogin();
    while(connectionHandler== nullptr){
        connectionHandler=handleFirstLogin();
    }
    inputFromKeyboardHandler inputFromKeyboardHandler(*connectionHandler);
    std::thread inputFromKeyboardThread(&inputFromKeyboardHandler::process, &inputFromKeyboardHandler);

    outputFromServerHandler outputFromServerHandler(*connectionHandler);
    std::thread outputFromServerThread(&outputFromServerHandler::process,&outputFromServerHandler);

    inputFromKeyboardThread.join();
    outputFromServerThread.join();
    delete connectionHandler;
}

ConnectionHandler* handleFirstLogin(){//this function handle the first login command.
    short port;//7777
    string host;
    vector<string> commandVector;
    ConnectionHandler *connectionHandler;
    string command;
    getline(cin, command);
    commandVector = getCommand(command, " ");
    while(commandVector[0] != "login"){//if the first command is not "login".
        commandVector.clear();
        getline(cin, command);
        commandVector = getCommand(command, " ");
        }
    vector<string> hostAndPort=getCommand(commandVector[1],":");
    host=hostAndPort[0];
    port=stoi(hostAndPort[1]);
    User *user=new User();
    connectionHandler=new ConnectionHandler(host, port,user);
    if (!connectionHandler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return nullptr;
    }
    else{
        //create and send connect frame.
        vector<pair<string,string>> headers;
        headers.push_back(pair<string,string>("accept-version","1.2"));
        headers.push_back(pair<string,string>("host","stomp,cs,bgu,ac,il"));
        headers.push_back(pair<string,string>("login",commandVector[2]));
        connectionHandler->getUser()->setUserName(commandVector[2]);
        headers.push_back(pair<string,string>("password",commandVector[3]));
        Frame frame("CONNECT",headers,"");
        connectionHandler->sendFrameAscii(frame.toString(),'\0');
        connectionHandler->setStatus(PENDING);
        return connectionHandler;
    }

  }

vector<string> getCommand(string s, string delimiter) {//split the command typed by the user into words. return a vector of words.(each position is a word)
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}
