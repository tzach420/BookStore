//
// Created by magenro@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <algorithm>
#include <iostream>
#include "../include/User.h"





User::User():userName(""),subscriptionId(0),recepitId(0),subIdPerTopic(),inventory(),wishToBorrow()
,recieptPerAction(),inventoryLock(),receiptLock(),wishListLock(),subscriptionIdLock(){
}
User::~User() {
    cleanUser();
}
int User::getAndIncSubId(string topic) {
    std::lock_guard<std::mutex> lock(subscriptionIdLock);
    subscriptionId++;
    subIdPerTopic.insert(pair<string,int>(topic,subscriptionId));
    return (++subscriptionId);
}
int User::getAndIncRecepitId() {
    std::lock_guard<std::mutex> lock(receiptLock);
    return(++recepitId);
}
int User::getSubscriptionId(string topic) {
    std::lock_guard<std::mutex> lock(subscriptionIdLock);
    return subIdPerTopic.find(topic)->second;

}
void User::addBook(Book* book) {
    std::lock_guard<std::mutex> lock(inventoryLock);
    inventory.push_back(book);
}
void User::setUserName(string _userName) {userName=_userName;}
string User::getUserName() {return userName;}
string User::getBookOwnerName(string bookName,string genre) {
    std::lock_guard<std::mutex> lock(inventoryLock);
    for (unsigned i=0; i<inventory.size(); i++)
        if(inventory.at(i)->getName()==bookName && inventory.at(i)->getGenre()==genre) return inventory.at(i)->getOriginOwnerName() ;
    return nullptr;
}
bool User::bookInInventory(string bookName,string genre){
    std::lock_guard<std::mutex> lock(inventoryLock);
    for (unsigned i=0; i<inventory.size(); i++) {
        if (inventory.at(i)->getName() == bookName && inventory.at(i)->getGenre() == genre) {
            if (!inventory.at(i)->isBorrowed()) {
                return true;
            }
        }
    }
    return false;
}
string User::getStringOfBookInTheInventory(string genre){
    std::lock_guard<std::mutex> lock(inventoryLock);
    string toReturn=userName+":";
    for (unsigned i=0; i<inventory.size(); i++){
        if(inventory.at(i)->getGenre()==genre && !inventory.at(i)->isBorrowed())
            toReturn=toReturn+inventory.at(i)->getName()+", ";
    }
    toReturn=toReturn.substr(0,toReturn.size()-2);
    return toReturn;
}

string User::getActionByNumberOfReciept(int num){
    std::lock_guard<std::mutex> lock(receiptLock);
    for (unsigned i=0; i<recieptPerAction.size(); i++){
        if (recieptPerAction.at(i).first==num)
            return recieptPerAction.at(i).second;
    }
    return "receipt no valid";
}

void User::addReceipt(int receiptNumber,string action){
    std::lock_guard<std::mutex> lock(receiptLock);
    recieptPerAction.push_back(pair<int,string>(receiptNumber,action));
}

void User::cleanUser(){
    userName="noBody";
    subIdPerTopic.clear();
    for (unsigned i=0; i<inventory.size(); i++){
        free((inventory.at(i)));
    }
    recieptPerAction.clear();
    wishToBorrow.clear();

}

void User::insertToWishToBorrow(string bookName,string genre){
    std::lock_guard<std::mutex> lock(wishListLock);
    wishToBorrow.push_back(pair<string,string>(bookName,genre));
}
bool User::searchInWishToBorrow(string bookName,string genre){
    std::lock_guard<std::mutex> lock(wishListLock);
    for (unsigned i=0; i<wishToBorrow.size(); i++) {
       if(wishToBorrow.at(i).first==bookName && wishToBorrow.at(i).second==genre) return true;
    }
    return  false;
}
void User::deleteFromWishToBorrow(string bookName,string genre){
    std::lock_guard<std::mutex> lock(wishListLock);
    for (unsigned i=0; i<wishToBorrow.size(); i++) {
        if(wishToBorrow.at(i).first==bookName && wishToBorrow.at(i).second==genre)
            wishToBorrow.erase(wishToBorrow.begin()+i);
    }
}
void User::deleteFromInventory(string bookName){
    std::lock_guard<std::mutex> lock(inventoryLock);
    for (unsigned i=0; i<inventory.size(); i++){
        if(inventory.at(i)->getName()==bookName) {
            Book* temp=inventory.at(i);
            inventory.erase(inventory.begin() + i);
            delete temp;
        }
    }
}
void User::setBookBorrowedStatus(string bookName,string genre,bool flag){
    std::lock_guard<std::mutex> lock(inventoryLock);
    for (unsigned i=0; i<inventory.size(); i++){
        if(inventory.at(i)->getGenre()==genre && inventory.at(i)->getName()==bookName) {
            inventory.at(i)->setBorrowToSomeoneStatus(flag);
        }
    }
}

