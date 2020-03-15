//
// Created by magenro@wincs.cs.bgu.ac.il on 10/01/2020.
//





#include "../include/outputFromServerHandler.h"

using namespace std;

outputFromServerHandler::outputFromServerHandler(ConnectionHandler &_connectionHandler):connectionHandler(_connectionHandler){}
void outputFromServerHandler::process() {
    while(connectionHandler.getStatus()!=TERMINATE){
        while(connectionHandler.getStatus()!=DISCONNECTED && connectionHandler.getStatus()!=TERMINATE){
            string msg;
            bool frameAscii=connectionHandler.getFrameAscii(msg,'\0');
            if(!frameAscii) cout<<"fail to connect to the server"<<endl;
            else handleMsgFromServer(msg);
        }
    }

}
void outputFromServerHandler::handleMsgFromServer(string msg){
    Frame frame(stringToFrame(msg));
    string command=frame.getCommand();
    if(command=="CONNECTED") handleConnectedFrame(frame);
    else if(command=="MESSAGE") handleMessageFrame(frame);
    else if(command=="RECEIPT") handleReceiptFrame(frame);
    else if(command=="ERROR") handleErrorFrame(frame);
}

void outputFromServerHandler::handleReceiptFrame (Frame &frame) {
    printContent(frame,"RECEIPT");
    string s=frame.getHeaders()[0].second;
    int numberOfReciept= stoi(s);
    User* user=connectionHandler.getUser();
    string action= user->getActionByNumberOfReciept(numberOfReciept);
    if(action=="DISCONNECT"){
        user->cleanUser();
        connectionHandler.close();
        connectionHandler.setStatus(TERMINATE);
    }
    else cout<<action<<endl;
}

void outputFromServerHandler::handleErrorFrame (Frame &frame) {
    printContent(frame,"ERROR");
    string errorMessage=frame.getHeaders()[0].second;
    connectionHandler.close();
    connectionHandler.setStatus(DISCONNECTED);
    cout<<errorMessage<<endl;
}

void outputFromServerHandler::handleConnectedFrame (Frame &frame) {
    printContent(frame,"CONNECTED");
    cout<<"Login successful"<<endl;
    connectionHandler.setStatus(CONNECTED);
}
Frame outputFromServerHandler::stringToFrame(string msg){
    vector<pair<string,string>> headers;
    string stompCommand;
    string frameBody;
    vector<string> frameVector=splitString(msg,"\n");
    stompCommand=frameVector[0];
    int i=1;
    while(frameVector[i]!=""){
        vector<string> temp=splitString(frameVector[i],":");
        headers.push_back(pair<string,string>(temp[0],temp[1]));
        i++;

    }
    frameBody=frameVector[i+1];
    return Frame(stompCommand,headers,frameBody);
}


void outputFromServerHandler::handleMessageFrame(Frame &frame) {
    printContent(frame,frame.getHeaders()[2].second);
    string body = frame.getBody();
    User *user = connectionHandler.getUser();
    vector<string> bodyVector = splitString(body, " ");
    string action = getActionToExecute(frame.getBody());
    if (action == "WISH TO BORROW") { handleWishToBorrow(frame); }
    else if (action == "RETURN BOOK") {
        if (bodyVector[bodyVector.size()-1] == user->getUserName()){
            string bookName=bodyVector[1];
            string genre=frame.getHeaders()[2].second;
            for(unsigned int i=2; i<bodyVector.size()-2;i++) bookName+=" "+bodyVector[i];
            user->setBookBorrowedStatus(bookName,genre,false );
        }
    }
    else if (action == "STATUS") { handleStatus(frame); }
    else if (action == "TAKE BOOK") { handleTakeBook(frame); }
    else if (action == "HAS BOOK") { handleHasBook(frame); }
    else{ return;}
}

void outputFromServerHandler::handleWishToBorrow(Frame &frame){
    User* user = connectionHandler.getUser();
    vector<string> bodyVector = splitString(frame.getBody(), " ");
    string bookName=bodyVector[4];
    for(unsigned int i=5; i<bodyVector.size();i++) bookName+=" "+bodyVector[i];//set book status to "borrowed"/
    string genre=frame.getHeaders()[2].second;
    if(user->bookInInventory(bookName,genre)){
        //create frame
        string body=user->getUserName()+" has "+bookName;
        vector<pair<string,string>> headers;
        string topic=frame.getHeaders()[2].second;
        headers.push_back(pair<string,string>("destination",topic));
        Frame f("SEND",headers,body);
        connectionHandler.sendFrameAscii(f.toString(),'\0');
    }
}


void outputFromServerHandler::handleStatus(Frame &frame){//print only the books in the genre.
    User* user = connectionHandler.getUser();
    vector<pair<string,string>> headers;
    string topic=frame.getHeaders()[2].second;
    headers.push_back(pair<string,string>("destination",topic));
    Frame f("SEND",headers,user->getStringOfBookInTheInventory(topic));
    connectionHandler.sendFrameAscii(f.toString(),'\0');
}


void outputFromServerHandler::handleHasBook(Frame &frame){
    User *user = connectionHandler.getUser();
    vector<string> bodyVector = splitString(frame.getBody(), " ");
    string bookName = bodyVector[2];
    for(unsigned int i=3; i<bodyVector.size();i++) bookName+=" "+bodyVector[i];//set book status to "borrowed"/
    string ownerName = bodyVector[0];
    string genre=frame.getHeaders()[2].second;
    if (user->searchInWishToBorrow(bookName,genre)) {
        connectionHandler.getUser()->deleteFromWishToBorrow(bookName,genre);
        Book* book=new Book(bookName,genre);
        book->setBorrowToSomeoneStatus(false);
        book->setOwnerName(ownerName);
        connectionHandler.getUser()->addBook(book);//now the book is mine.
        vector<pair<string,string>> headers;
        headers.push_back(pair<string,string>("destination",genre));
        Frame f("SEND",headers,"Taking "+bookName+" from "+ownerName);
        connectionHandler.sendFrameAscii(f.toString(),'\0');

    }
}


void outputFromServerHandler::handleTakeBook(Frame &frame){
    User *user = connectionHandler.getUser();
    vector<string> bodyVector = splitString(frame.getBody(), " ");
    string ownerName=bodyVector[bodyVector.size()-1];
    string bookName=bodyVector[1];
    string genre=frame.getHeaders()[2].second;
    for(unsigned int i=2; i<bodyVector.size()-2;i++) bookName+=" "+bodyVector[i];//set book status to "borrowed"/
    if (ownerName==user->getUserName()){
        user->setBookBorrowedStatus(bookName,genre,true);//now the book is not mine.
    }

}


string outputFromServerHandler::getActionToExecute(string body){
    vector<string> bodyVector=splitString(body," ");
         if(bodyVector.size()>1) {
             if (bodyVector[1] == "wish")return "WISH TO BORROW";
             else if (bodyVector[0] == "Returning") return "RETURN BOOK";
             else if (bodyVector[1] == "status") return "STATUS";
             else if (bodyVector[0] == "Taking") return "TAKE BOOK";
             else if (bodyVector[1] == "has"){
                 if(bodyVector[2] != "added")return "HAS BOOK";
                 else return"do nothing";
             }
             else return "do nothing";
         }
        else return "do nothing";
}
void outputFromServerHandler::printContent(Frame frame,string topic){
    cout<<frame.toString()+"^@\n";
}

vector<string> outputFromServerHandler::splitString(string s, string delimiter){
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