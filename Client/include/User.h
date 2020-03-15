//
// Created by magenro@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#include <map>
#include <vector>
#include <mutex>
#include "Book.h"

using namespace std;
class User {
private:
    string userName;
    int subscriptionId;
    int recepitId;
    std::map<string,int> subIdPerTopic;//<genre,subscriptionID to that genre>
    vector<Book*> inventory;//current books owned by this user.
    vector<pair<string,string>> wishToBorrow;//list of books the user wish to borrow, <bookName,bookGenre>
    vector<pair<int,string>> recieptPerAction;//list of actions done by the user. <receiptID, actionName>
    std::mutex inventoryLock;
    std::mutex receiptLock;
    std::mutex wishListLock;
    std::mutex subscriptionIdLock;

public:
    User();
    virtual ~User();
    int getAndIncSubId(string topic);
    int getAndIncRecepitId();
    int getSubscriptionId(string topic);//get the subscription id of user for specific topic,
    void cleanUser();//remove all components of the user, called upon termination.
    void addBook(Book* book);//adding book to the inventory.
    void setUserName(string userName);
    void addReceipt(int receiptNumber,string action);//upon recieving receipt from the server, saves it.
    void insertToWishToBorrow(string bookName,string genre);//add a book the user wish to borrow.
    void deleteFromWishToBorrow(string bookName,string genre);//delete book from the wishList when the user borrowed it.
    void deleteFromInventory(string bookName);//delete book from user inventory.
    void setBookBorrowedStatus(string bookName,string genre,bool flag);//when the user borrow book to someone, update its status to "borrowd".
    bool searchInWishToBorrow(string bookName,string genre);//check if a book exist in the wishList.
    bool bookInInventory(string bookName,string genre);//check if a book available(exist+not borrowed) in the inventory.
    string getUserName();
    string getBookOwnerName(string bookName,string genre);//
    string getStringOfBookInTheInventory(string genre);// return a string of all the books in the user inventory, not including the borrowed books.
    string getActionByNumberOfReciept(int num);//return the action that the receiptID preformed.


};


#endif //BOOST_ECHO_CLIENT_USER_H
