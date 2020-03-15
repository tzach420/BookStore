//
// Created by tzachu@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include <string>

using namespace std;

class Book {
private:
    string name;
    string genre;
    bool borrowToSomeone;//true<=>i borrow the book to someone else. i dont have acess to the book.
    string originalOwner;



public:
    Book(string bookName,string genre);
    virtual ~Book();
    string getGenre();
    string getName();
    string getOriginOwnerName();
    bool isBorrowed();
    void setOwnerName(string name);
    void setBorrowToSomeoneStatus(bool flag);
};


#endif //BOOST_ECHO_CLIENT_BOOK_H
