package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;

public class User {
    private String userName;
    private String password;
    private Map<String,Integer> subscriptionIdPerTopic;
    private int connectionId;
    private boolean isConnected=false;



    public User(String name,String pass,int connectionId){
        userName=name;
        password=pass;
        this.connectionId=connectionId;
        subscriptionIdPerTopic=new HashMap<>();
    }

    public  int getConnHandlerId(){return connectionId;}
    public Integer getSubscriptionId(String topic){
        return subscriptionIdPerTopic.get(topic);
    }
    public String getTopicBySubscriptionId(Integer subscriptionId){
        for(String topic: subscriptionIdPerTopic.keySet()){
            if(subscriptionIdPerTopic.get(topic).equals(subscriptionId))
                return topic;
        }
        return "";
    }
    public void addSubscription(String topic, Integer subscriptionId){
        subscriptionIdPerTopic.putIfAbsent(topic,subscriptionId);
    }
    public String getLoginName(){return userName;}
    public String getPassword(){return password;}
    public boolean isActive(){return isConnected;}
    public void logedIn(){isConnected=true;}
    public void logedOut(){isConnected=false;}
    public String getName(){return  userName;}
    public void unsubscribeFromTopic(String topic){subscriptionIdPerTopic.remove(topic); }

}
