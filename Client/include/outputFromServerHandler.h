//
// Created by magenro@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_OUTPUTFROMSERVERHANDLER_H
#define BOOST_ECHO_CLIENT_OUTPUTFROMSERVERHANDLER_H
#include "connectionHandler.h"
#include "Frame.h"

using namespace std;
class outputFromServerHandler {
private:
    ConnectionHandler & connectionHandler;
    void handleMessageFrame(Frame &frame);
    void handleConnectedFrame(Frame &frame);
    void handleReceiptFrame(Frame &frame);
    void handleErrorFrame(Frame &frame);
    void handleMsgFromServer(string msg);
    void printContent(Frame frame,string topic);
    void handleWishToBorrow(Frame &frame);
    void handleStatus(Frame &frame);
    void handleHasBook(Frame &frame);
    void handleTakeBook(Frame &frame);
    Frame stringToFrame(string msg);
    string getActionToExecute(string body);
    vector<string> splitString(string s, string delimiter);

public:
    outputFromServerHandler(ConnectionHandler &_connectionHandler);
    void process();
};


#endif //BOOST_ECHO_CLIENT_OUTPUTFROMSERVERHANDLER_H
