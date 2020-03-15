CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: StompBookClubClient
	g++ -pthread -o bin/StompBookClubClient bin/connectionHandler.o bin/User.o bin/outputFromServerHandler.o bin/inputFromKeyboardHandler.o bin/Frame.o bin/stompClient.o bin/Book.o $(LDFLAGS) 

StompBookClubClient: bin/connectionHandler.o bin/User.o bin/outputFromServerHandler.o bin/inputFromKeyboardHandler.o bin/Frame.o bin/stompClient.o bin/Book.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/User.o: src/User.cpp
	g++ -pthread $(CFLAGS) -o bin/User.o src/User.cpp

bin/outputFromServerHandler.o: src/outputFromServerHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/outputFromServerHandler.o src/outputFromServerHandler.cpp

bin/inputFromKeyboardHandler.o: src/inputFromKeyboardHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/inputFromKeyboardHandler.o src/inputFromKeyboardHandler.cpp

bin/Frame.o: src/Frame.cpp
	g++ -pthread $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/stompClient.o: src/stompClient.cpp
	g++ -pthread $(CFLAGS) -o bin/stompClient.o src/stompClient.cpp

bin/Book.o: src/Book.cpp
	g++ -pthread $(CFLAGS) -o bin/Book.o src/Book.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
