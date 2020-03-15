//
// Created by tzachu@wincs.cs.bgu.ac.il on 13/01/2020.
//

#include "../include/Book.h"

Book::Book(string bookName,string _genre):name(bookName),genre(_genre),borrowToSomeone(false),originalOwner(""){
}
Book::~Book(){}
string Book::getGenre(){return genre;}
string Book::getName(){return name;};
string Book::getOriginOwnerName(){return originalOwner;}
bool Book::isBorrowed() {return borrowToSomeone;}
void Book::setOwnerName(string name){originalOwner=name;}
void Book::setBorrowToSomeoneStatus(bool flag){borrowToSomeone=flag;}