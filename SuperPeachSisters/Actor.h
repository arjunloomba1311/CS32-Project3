#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//Class declarations

class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(StudentWorld *p_sw, int imageID, int startX, int startY, int depth, int size);
    bool getAliveStatus();
    void killActor();
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    virtual bool canPassThrough(); //i.e. can the actor pass through the object or not.
    virtual void bonk() {};
    
    virtual bool isPlayer() {return false;};
    
private:
    bool m_isalive = true; //true is for alive and false is for dead.
    StudentWorld *m_sw;
};

//--------Peach Declaration ------------//

class Peach: public Actor {
public:
    Peach(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
    virtual bool isPlayer() {return true;};
    
    //set powers:
    void setJumpPower();
    void setFirePower();
    void setStarPower();
    
    //change other attributes
    void increasePlayerScore(int num);
    void setHitPoints(int num);
    
    
private:
    int m_hitPoints = 1;
    int m_invincibilityTicker = 0;
    bool m_hasStarPower = false;
    bool m_hasFirePower = false;
    bool m_hasJumpPower = false;
    
    int m_score = 0;
    
    int m_jumpDist;

};

//-------Static Objects Declaration-----//

class Block: public Actor {
public:
    Block(StudentWorld* p_sw, int startX, int startY, char power);
    virtual void doSomething();
    virtual bool canPassThrough();
    virtual void bonk();
    char getPower();
    void removePower();
private:
    char m_power = 'n';
};

class Pipe: public Actor {
public:
    Pipe(StudentWorld* psw, int startX, int startY);
    virtual void doSomething();
    virtual bool canPassThrough();
};

//-------Goodies Declaration-----//

class Goodies: public Actor {
public:
    Goodies(StudentWorld *p_sw, int startX, int startY, int ImageID);
    virtual void doSomething() = 0;
    virtual void move();
    
    ~Goodies() {};
private:
};

class Mushroom: public Goodies {
public:
    Mushroom(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething();
private:
};

class Flower: public Goodies {
public:
    Flower(StudentWorld *p_sw, int startX, int StartY);
    virtual void doSomething();
private:
};

class Star: public Goodies {
public:
    Star(StudentWorld *p_sw, int startX, int StartY);
    virtual void doSomething();
private:
};

//------CheckPoints Declaration------//

class Checkpoint: public Actor {
public:
    Checkpoint(StudentWorld *p_sw, int startX, int startY,  int ImageID);
    virtual void doSomething() = 0;
};

class Flag: public Checkpoint {
public:
    Flag(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething() {};
}; 


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


#endif // ACTOR_H_
