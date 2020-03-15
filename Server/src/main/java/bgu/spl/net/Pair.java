package bgu.spl.net;


public class Pair<L,R>  {
    private L first;
    private R second;

    public Pair(L first,R second) {
       this.first=first;
        this.second=second;
    }

    public L first() {
        return first;
    }

    public R second() {
        return second;
    }
}

