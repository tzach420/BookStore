package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

public  class Database {
    private AtomicInteger currMessageId=new AtomicInteger(1);
    private static Database singleInstance = new Database();
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<User>> topicsTable=new ConcurrentHashMap<>();//<topic,all the users subscribed to topic>
    private ConcurrentHashMap<Integer,User> users=new ConcurrentHashMap<>();//<userID,User>
    Object userLock=new Object();


    public static  Database getInstance() {
        return singleInstance;
    }
    public ConcurrentLinkedQueue<User> getSubscribesOfTopic(String topic){
        return topicsTable.get(topic);
    }
    public Integer incMessageId(){return currMessageId.getAndIncrement();}
    public synchronized void subscribeUserToTopic(String topic,Integer connectionId,Integer subscriptionId){
        User user=users.get(connectionId);
        ConcurrentLinkedQueue<User> usersPerTopic = new ConcurrentLinkedQueue<>();
        topicsTable.putIfAbsent(topic, usersPerTopic); //if the topic is empty
        if(!topicsTable.get(topic).contains(user)) { //if user dont subscribe to the topic
            topicsTable.get(topic).add(user); //add user to the topic
            user.addSubscription(topic,subscriptionId); // add the Subscription to the user
        }
    }
    public void unSubscribeUserToTopic(String topic,Integer subscriptionId) { //delete from one topic
        if (topicsTable.get(topic)!=null && !topicsTable.get(topic).isEmpty() ) {
            for (User user : topicsTable.get(topic)) {
                if (user.getSubscriptionId(topic).intValue() == subscriptionId.intValue())
                    topicsTable.get(topic).remove(user);
            }
        }
    }
    public void addUser(String userName,String password, Integer connectionId) {
        synchronized (userLock) { //prevenr 2 user with same name to log in
            User user = new User(userName, password, connectionId);
            user.logedIn();
            users.put(connectionId, user);
        }
    }
    public Integer userExists(String name){
        synchronized (userLock) { //prevenr 2 user with same name to log in
            for (ConcurrentHashMap.Entry<Integer, User> pair : users.entrySet()) {
                if (pair.getValue().getLoginName().equals(name)) {
                    return pair.getKey();
                }
            }
            return null;
        }
    }

    public boolean passwordIsCorrect(String password,Integer userId){
        return users.get(userId).getPassword().equals(password);
    }
    public boolean userIsLoggedIn(Integer userId){
        synchronized (users.get(userId)) {
            return users.get(userId).isActive();
        }
    }
    public User getUserById(Integer userId){
        return users.get(userId);
    }
    public void updateUserId(String userName, Integer newUserId){
        for(Integer id:users.keySet()){
            if (users.get(id).getName().equals(userName)){
                String password=users.get(id).getPassword();
                users.remove(id);
                User addUser=new User(userName,password,newUserId);
                users.putIfAbsent(newUserId,addUser);
            }
        }
    }
    public User getUserByName(String userName){
        for(Integer id:users.keySet()){
            if (users.get(id).getName().equals(userName)) {
               return users.get(id);
            }
        }
        return null;
    }
    public void deleteUserFromAllTheTopic(String userName){
        for (ConcurrentLinkedQueue<User> userList: topicsTable.values()){
            for(User user:userList){
                if(user.getName().equals(userName)){
                    userList.remove(user);
                }

            }
        }
    }
    public String getTopicBySubscriptionId(Integer connectionId,Integer subscriptionId){
        User user= users.get(connectionId);
        return user.getTopicBySubscriptionId(subscriptionId);

    }



}

