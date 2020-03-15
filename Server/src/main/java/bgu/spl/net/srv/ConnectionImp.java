package bgu.spl.net.srv;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

import bgu.spl.net.Pair;
import bgu.spl.net.impl.stomp.Frame;

public class ConnectionImp<T> implements Connections<T> {//ALL THIS CLASS IS OUR IMP.

    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connectionHandlers;

    public ConnectionImp() {
        connectionHandlers = new ConcurrentHashMap<>();
    }

    @Override
    public boolean send(int connectionId, T msg) {
        if(connectionHandlers.containsKey(connectionId)) {
            ConnectionHandler<T> connectionHandler = connectionHandlers.get(connectionId);
            connectionHandler.send(msg);
            return true;
        }
        return false;//in case there is not connection under that Id.
    }

    @Override
    public void send(String channel, T msg) {
       ConcurrentLinkedQueue<User> users=Database.getInstance().getSubscribesOfTopic(channel);
        for(User user: users){
           int connectionId=user.getConnHandlerId();
           send(connectionId,msg);
        }

    }

    @Override
    public void disconnect(int connectionId) {

        connectionHandlers.remove(connectionId);

    }
    public void addConnection(int connectionId,ConnectionHandler<T> toAdd){connectionHandlers.putIfAbsent(connectionId,toAdd);}

}
