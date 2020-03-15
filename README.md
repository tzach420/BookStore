# BookStore
Implementation of a book club. Users will be able to signup for reading clubs and borrow books from each other. Implementation of both a server, which will provide STOMP server services, and a client, which a user can use in order to interact with the rest of the users. The server is implemented in Java and support both Thread-Per-Client(TPS) and the Reactor,choosing which one according to arguments given on startup. The client is implemented in C++, and holds the requiredlogic as described below. All communication between the clients and the server are according to the STOMP protocol.
# STOMP protocol
STOMP is a simple interoperable protocol designed for asynchronous message passing between clients via mediating servers. It defines a text based wire-format for messages passed between these clients and servers. We will use the STOMP protocol in the project for passing messages between the client and the server. For a complete specification of the STOMP protocol, read: [STOMP 1.2](https://stomp.github.io/stomp-specification-1.2.html)
# User commands:
- Login Command - for this command a CONNECT frame is sent to the server.<br/>
Syntax - login {host:port} {username} {password}<br/>
Example - login 1.1.1.1:2000 bob alice
- Join Genre Reading Club Command - for this command a SUBSCRIBE frame is sent to the {genre} topic.<br/>
Syntax - join {genre}<br/>
Example - join sci-fi
- Exit Genre Reading Club Command - for this command a UNSUBSCRIBE frame is sent to the {genre} topic.<br/>
Syntax - exit {genre}<br/>
Example - exit sci-fi
- Add Book Command - add book to the client inventory.<br/>
Syntax - add {genre} {book name}
Example - add Fantasy Lord of the rings
- BorrowBook Command - informs all the users that this client user wish to borrow a book.<br/>
Syntax - borrow {genre} {book name}<br/>
Example - borrow Fantasy Lord of the rings
- Return Book Command - This will result in removing the book from the borrower inventory,  and adding it back to the lender.<br/>
Syntax - return {genre} {book name}<br/>
Example - return Fantasy Lord of the rings
- Genre Book Status Command - all the subscribed users will send a SEND frame, each with its current inventory.<br/>
Syntax - status {genre}<br/>
Example - status Fantasy
- Logout Command - This command tells the client that the user wants to log out from the library. The client will send a DISCONNECT to the server.<br/>
Syntax - logout
# Compilation
- server:<br>
1.open terminal in Server directory<br/>
2.type 'mvn compile'<br/>
3.choose the server you want:<br/>
Thread per client server: mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="{port} tpc"<br/>
Reactor server: mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="{port} reactor"<br/>
- Client:<br/>
NOTE: currently the server is running on port 7777. in order to change that modify StompServer.java.
make sure you have boost install.<br/>
  1.open terminal in Client directory.<br/>
  2. type 'make'
  
