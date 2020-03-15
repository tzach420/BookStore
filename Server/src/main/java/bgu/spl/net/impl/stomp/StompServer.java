package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.newsfeed.NewsFeed;
import bgu.spl.net.srv.Server;
//15.1.20 00:05
public class StompServer {
    public static void main(String[] args) {//args[0] port. args[1] tcp/reactor

// you can use any server...
        if(args[1].equals("tpc")) {
            Server server = Server.threadPerClient(Integer.parseInt(args[0]), () -> new stompMessageProtocolImp(), () -> new stompEncoderDecoder<String>() {
            });
            server.serve();
        }
        else if(args[1].equals("reactor")) {
        Server.reactor(Runtime.getRuntime().availableProcessors(), Integer.parseInt(args[0]), //port
                () -> new stompMessageProtocolImp(), //protocol factory
                () -> new stompEncoderDecoder<>() //message encoder decoder factory
        ).serve();

        }
    }

}
