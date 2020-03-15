//
// Created by tzachu@wincs.cs.bgu.ac.il on 08/01/2020.
//


#include "../include/inputFromKeyboardHandler.h"
#include "../include/Frame.h"


inputFromKeyboardHandler::inputFromKeyboardHandler(ConnectionHandler &_connectionHandler):connectionHandler(_connectionHandler){}

void inputFromKeyboardHandler::process() {
    while(connectionHandler.getStatus()!=TERMINATE){
        string command;
        getline(cin, command);
        vector<string> commandVector = getCommand(command, " ");
        connectionStatus status=connectionHandler.getStatus();
        if(status==CONNECTED){//any command is possible, except login.
            if(commandVector[0]=="login") cout<<"User is already login."<<endl;
            else handleCommand(commandVector);
        }
        else if(status==DISCONNECTED) {//wait for login command.
            if (commandVector[0] == "login") {
                handleLoginCommand(commandVector);
            }
            else cout<<"ERROR: there is no active user, please login."<<endl;
        }
    }
}

void inputFromKeyboardHandler::handleCommand(vector<string> command){
    if(command[0]=="login") handleLoginCommand(command);//to remove.
    else if(command[0]=="join") handleJoinCommand(command);
    else if(command[0]=="exit") handleExitCommand(command);
    else if(command[0]=="add") handleAddCommand(command);
    else if(command[0]=="borrow") handleBorrowCommand(command);
    else if(command[0]=="return") handleReturnCommand(command);
    else if(command[0]=="status") handleStatusCommand(command);
    else if(command[0]=="logout") handleLogoutCommand(command);
}

void inputFromKeyboardHandler::handleLoginCommand(vector<string> command){

        //open socket and send login frame.
        connectionHandler.connect();
        connectionHandler.setStatus(PENDING);
        vector<pair<string, string>> headers;
        headers.push_back(pair<string, string>("accept-version", "1.2"));
        headers.push_back(pair<string, string>("host", "stomp,cs,bgu,ac,il"));
        headers.push_back(pair<string, string>("login", command[2]));
        connectionHandler.getUser()->setUserName(command[2]);
        headers.push_back(pair<string, string>("password", command[3]));
        Frame frame("CONNECT", headers, "");
        connectionHandler.sendFrameAscii(frame.toString(), '\0');

}

void inputFromKeyboardHandler::handleJoinCommand(vector<string> command){
    vector<pair<string,string>> headers;
    string genre=command[1];
    headers.push_back(pair<string,string>("destination",genre));
    int subscriptionId=connectionHandler.getUser()->getAndIncSubId(genre);
    headers.push_back(pair<string,string>("id",to_string(subscriptionId)));
    int recepitId=connectionHandler.getUser()->getAndIncRecepitId();
    connectionHandler.getUser()->addReceipt(recepitId,"joined club "+genre);
    headers.push_back(pair<string,string>("receipt-id",to_string(recepitId)));
    Frame frame("SUBSCRIBE",headers,"");
    connectionHandler.sendFrameAscii(frame.toString(),'\0');
}
void inputFromKeyboardHandler::handleExitCommand(vector<string> command){
    vector<pair<string,string>> headers;
    int recepitId=connectionHandler.getUser()->getAndIncRecepitId();
    connectionHandler.getUser()->addReceipt(recepitId,"Exit club "+command[1]);
    headers.push_back(pair<string,string>("id",to_string(connectionHandler.getUser()->getSubscriptionId(command[1]))));
    headers.push_back(pair<string,string>("receipt-id",to_string(recepitId)));
    Frame frame("UNSUBSCRIBE",headers,"");
    connectionHandler.sendFrameAscii(frame.toString(),'\0');

}
void inputFromKeyboardHandler::handleAddCommand(vector<string> command) {
    vector<pair<string,string>> headers;
    string bookName=command[2];
    for(unsigned int i=3; i<command.size();i++) bookName+=" "+command[i];
    connectionHandler.getUser()->addBook(new Book(bookName,command[1]));
    headers.push_back(pair<string,string>("destination",command[1]));
    string body=connectionHandler.getUser()->getUserName()+" has added the book "+ bookName;
    Frame frame("SEND",headers,body);
    connectionHandler.sendFrameAscii(frame.toString(),'\0');
}
void inputFromKeyboardHandler::handleBorrowCommand(vector<string> command) {
    vector<pair<string,string>> headers;
    string bookName=command[2];
    for(unsigned int i=3; i<command.size();i++) bookName+=" "+command[i];
    connectionHandler.getUser()->insertToWishToBorrow(bookName,command[1]);
    headers.push_back(pair<string,string>("destination",command[1]));
    string body=connectionHandler.getUser()->getUserName()+" wish to borrow "+ bookName;
    Frame frame("SEND",headers,body);
    connectionHandler.sendFrameAscii(frame.toString(),'\0');
}
void inputFromKeyboardHandler::handleReturnCommand(vector<string> command) {
    string bookName=command[2];
    string genre=command[1];
    for(unsigned int i=3; i<command.size();i++) bookName+=" "+command[i];
    if(connectionHandler.getUser()->bookInInventory(bookName,genre)) {
       vector<pair<string, string>> headers;
       headers.push_back(pair<string, string>("destination", genre));
       string body = "Returning " + bookName + " to " + connectionHandler.getUser()->getBookOwnerName(bookName,genre);
       Frame frame("SEND", headers, body);
       connectionHandler.getUser()->deleteFromInventory(bookName);
       connectionHandler.sendFrameAscii(frame.toString(), '\0');
   }
}
void inputFromKeyboardHandler::handleStatusCommand(vector<string> command) {
    vector<pair<string,string>> headers;
    headers.push_back(pair<string,string>("destination",command[1]));
    string body="book status";
    Frame frame("SEND",headers,body);
    connectionHandler.sendFrameAscii(frame.toString(),'\0');
}
void inputFromKeyboardHandler::handleLogoutCommand(vector<string> command) {
    vector<pair<string,string>> headers;
    int recepitId=connectionHandler.getUser()->getAndIncRecepitId();
    connectionHandler.getUser()->addReceipt(recepitId,"DISCONNECT");
    headers.push_back(pair<string,string>("recepit",to_string(recepitId)));
    Frame frame("DISCONNECT",headers,"");
    connectionHandler.sendFrameAscii(frame.toString(),'\0');
    connectionHandler.setStatus(TERMINATE);
}
vector<string> inputFromKeyboardHandler::getCommand(string s, string delimiter) {//split the command typed by the user into words. return a vector of words.(each position is a word)
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