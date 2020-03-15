package bgu.spl.net.impl.stomp;

import bgu.spl.net.Pair;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.Database;
import bgu.spl.net.srv.User;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentLinkedQueue;

public class stompMessageProtocolImp implements StompMessagingProtocol {
    private Connections<String> connections;
    private boolean shouldTerminate = false;
    private  int connectionId;
    private Object lock;



    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connections = connections;
        this.connectionId= connectionId;
    }

    @Override
    public void process(Object msg) {
        Frame frame = StringToFrame(msg);
        if(frame.getCommand().equals("CONNECT")) handleConnectFrame(frame);
        else if(frame.getCommand().equals("SEND")) handleSendFrame(frame);
        else if(frame.getCommand().equals("SUBSCRIBE")) handleSubscribeFrame(frame);
        else if(frame.getCommand().equals("UNSUBSCRIBE")) handleUnSubscribeFrame(frame);
        else if(frame.getCommand().equals("DISCONNECT")) handleDisconnectFrame(frame);

    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    private void handleConnectFrame(Frame frame) {
        //create new user
        String userName=frame.getHeaders().get(2).second();
        String password=frame.getHeaders().get(3).second();
        Integer oldUserId=Database.getInstance().userExists(userName);// if userId=null it means userId not exsit, else userId equal to old destinationId
        List<Pair<String,String>> headers= new LinkedList<>();
        if(oldUserId!=null){ //user is already exsist in the system
            if(Database.getInstance().passwordIsCorrect(password,oldUserId)){ //the password match
                if(!Database.getInstance().userIsLoggedIn(oldUserId)){ //not logged in
                    headers.add(new Pair<>("version",frame.getHeaders().get(0).second()));
                    Frame frameReceipt=new Frame("CONNECTED",headers,"");
                    Database.getInstance().updateUserId(userName,connectionId); //update userId
                    Database.getInstance().getUserByName(userName).logedIn();
                    connections.send(connectionId,frameReceipt.toString());//send connected frame to the client.
                }
                else{
                    //send error: user is already logged-In
                        headers.add(new Pair<>("message","User is already connected"));
                        Frame frameReceipt = new Frame("ERROR", headers, frame.toString());
                        connections.send(connectionId, frameReceipt.toString());//send connected frame to the client.
                        connections.disconnect(connectionId);
                        shouldTerminate=true;

                }

            }
            else{
                //send error:PASSWORD IS NOT CORRECT.
                headers.add(new Pair<>("message","Wrong password"));
                Frame frameReceipt=new Frame("ERROR",headers,frame.toString());
                connections.send(connectionId,frameReceipt.toString());//send connected frame to the client.
                connections.disconnect(connectionId);
                shouldTerminate=true;

            }


        }
        else{
            //create new user
            Database.getInstance().addUser(frame.getHeaders().get(2).second(), frame.getHeaders().get(3).second(),connectionId);
            headers.add(new Pair<>("version",frame.getHeaders().get(0).second()));
            Frame frameReceipt=new Frame("CONNECTED",headers,"");
            connections.send(connectionId,frameReceipt.toString());//send connected frame to the client
        }

    }

    private void handleSendFrame(Frame frame){//send the frame body to all of the subscribers of the topic
        String channel= frame.getHeaders().get(0).second();
        ConcurrentLinkedQueue<User> users=Database.getInstance().getSubscribesOfTopic(channel);
        for(User user: users) {
            if (user.isActive()){ //send only to user that active
            int connectionId = user.getConnHandlerId();
            List<Pair<String, String>> headers = new LinkedList<>();
            headers.add(new Pair("subscription", user.getSubscriptionId(channel).toString()));
            headers.add(new Pair("Message-id", Database.getInstance().incMessageId().toString()));//CHECK IT
            headers.add(new Pair("destination", channel));
            Frame frameToClient = new Frame("MESSAGE", headers, frame.getBody());
            connections.send(connectionId, frameToClient.toString());
            }
        }
    }
    private void handleSubscribeFrame(Frame frame){
        String topic=frame.getHeaders().get(0).second();
        String subscriptionId = frame.getHeaders().get(1).second();
        String receiptId=frame.getHeaders().get(2).second();
        Database.getInstance().subscribeUserToTopic(topic,connectionId ,Integer.parseInt(subscriptionId));
        List<Pair<String,String>> headers= new LinkedList<>();
        headers.add(new Pair<>("receipt-id",receiptId));
        Frame frameReceipt=new Frame("RECEIPT",headers,"");
        connections.send(connectionId,frameReceipt.toString());//send recipt frame to the client.
    }
    private void handleUnSubscribeFrame(Frame frame){//remove subscription from one topic
        String subscriptionId=frame.getHeaders().get(0).second();
        String receiptId=frame.getHeaders().get(1).second();
        String topic= Database.getInstance().getTopicBySubscriptionId(connectionId,Integer.parseInt(subscriptionId));
        Database.getInstance().unSubscribeUserToTopic(topic,Integer.parseInt(subscriptionId));// delete user from topic list of user
        Database.getInstance().getUserById(connectionId).unsubscribeFromTopic(topic);//remove topic from the topic list of the user
        List<Pair<String,String>> headers= new LinkedList<>();
        headers.add(new Pair<>("receipt-id",receiptId));
        Frame frameReceipt=new Frame("RECEIPT",headers,"");
        connections.send(connectionId,frameReceipt.toString());//send recipt frame to the client.

    }
    private void handleDisconnectFrame(Frame frame){
        String receiptId=frame.getHeaders().get(0).second();
        List<Pair<String,String>> headers= new LinkedList<>();
        headers.add(new Pair<>("receipt-id",receiptId));
        Frame frameReceipt=new Frame("RECEIPT",headers,"");
        User currentUser=Database.getInstance().getUserById(connectionId);//get the active user.
        currentUser.logedOut();//change status of user
        Database.getInstance().deleteUserFromAllTheTopic(currentUser.getName());//remove the user from all the topics.
        connections.send(connectionId,frameReceipt.toString());//send recipt frame to the client, assuring gracefull shutdown.
        connections.disconnect(connectionId);
        shouldTerminate=true;
    }
    public Frame StringToFrame(Object msg){//creates a frame from string.
        String message= (String)msg;
        String command,body;
        List<Pair<String,String>> headers= new LinkedList<>();
        String[] toFrame=message.split("\n",-1);
        command=toFrame[0];
        int i=1;
        while(i<toFrame.length && !toFrame[i].equals("")){
            String[] header=toFrame[i].split(":");
            headers.add(new Pair<>(header[0],header[1]));
            i++;
        }
        if(i<toFrame.length-1) body=toFrame[i+1];
        else body="";
        return new Frame(command,headers,body);
    }

     public Connections<String> getConnections(){return connections;}
     public int getConnectionId(){return  connectionId;}
}

