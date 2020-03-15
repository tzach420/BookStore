# BookStore
Implementation of a book club. Users will be able to signup for reading clubs and borrow books from each other. Implementation of both a server, which will provide STOMP server services, and a client, which a user can use in order to interact with the rest of the users. The server is implemented in Java and support both Thread-Per-Client(TPS) and the Reactor,choosing which one according to arguments given on startup. The client is implemented in C++, and holds the requiredlogic as described below. All communication between the clients and the server are according to the STOMP protocol.
# STOMP protocol
STOMP is a simple interoperable protocol designed for asynchronous message passing between clients via mediating servers. It defines a text based wire-format for messages passed between these clients and servers. We will use the STOMP protocol in the project for passing messages between the client and the server. For a complete specification of the STOMP protocol, read: [STOMP 1.2](https://stomp.github.io/stomp-specification-1.2.html)
