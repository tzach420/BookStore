//
// Created by tzachu@wincs.cs.bgu.ac.il on 08/01/2020.
//


#include "../include/Frame.h"

Frame::Frame(string command, vector<pair<string, string>> header, string body):stompCommand(command),headers(header),frameBody(body) {}
string Frame::getBody(){return frameBody;}
string Frame::getCommand(){return stompCommand;}
vector<pair<string,string>> Frame::getHeaders(){return headers;}
string Frame::toString(){
    string toReturn="";
    toReturn=toReturn+stompCommand+"\n";
    for(pair<string,string> header: headers){ //may be zero
        toReturn=toReturn+header.first+":"+header.second+"\n";
    }
    toReturn=toReturn+"\n";//blank line indicate end of headers and begining of body
    if(frameBody=="")
        toReturn=toReturn+"\0";
    else
        toReturn=toReturn+frameBody+"\n"+"\0";
    return toReturn;
}


