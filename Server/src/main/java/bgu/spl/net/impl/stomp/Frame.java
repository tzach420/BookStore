package bgu.spl.net.impl.stomp;

import bgu.spl.net.Pair;
import java.util.HashMap;
import java.util.List;

public class Frame {
    private String stompCommand;
    private List<Pair<String,String>> headers;//<HeaderName, HeaderValue>
    private String frameBody;

    public Frame(String command,List<Pair<String,String>> headers, String body){
        stompCommand=command;
        this.headers=headers;
        frameBody=body;
    }
    public String getCommand(){return stompCommand;}
    public List<Pair<String,String>> getHeaders(){return headers;}
    public String getBody(){return frameBody;}

    public String toString(){
        String toReturn="";
        toReturn=toReturn+stompCommand+"\n";
        for(Pair<String,String> pair: headers){ //may be zero
            toReturn=toReturn+pair.first()+":"+pair.second()+"\n";
        }
        toReturn=toReturn+"\n";//blank line indicate end of headers and begining of body
        if(frameBody.equals(""))
            toReturn=toReturn+"\u0000";
        else
            toReturn=toReturn+frameBody+"\n"+"\u0000";

        return toReturn;
    }



}
