//
// Created by tzachu@wincs.cs.bgu.ac.il on 08/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_INPUTFROMKEYBOARDHANDLER_H
#define BOOST_ECHO_CLIENT_INPUTFROMKEYBOARDHANDLER_H


#include "connectionHandler.h"
using namespace std;
class inputFromKeyboardHandler {
private:
    ConnectionHandler & connectionHandler;


    vector<string> getCommand(string s, string delimiter);
    void handleCommand(vector<string> command);
    void handleLoginCommand(vector<string> command);
    void handleJoinCommand(vector<string> command);
    void handleExitCommand(vector<string> command);
    void handleAddCommand(vector<string> command);
    void handleBorrowCommand(vector<string> command);
    void handleReturnCommand(vector<string> command);
    void handleStatusCommand(vector<string> command);
    void handleLogoutCommand(vector<string> command);

public:
    inputFromKeyboardHandler(ConnectionHandler &_connectionHandler);
    void process();
};


#endif //BO`OST_ECHO_CLIENT_INPUTFROMKEYBOARDHANDLER_H
