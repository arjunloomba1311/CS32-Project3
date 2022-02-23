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
    virtual void setDir(int dir);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    virtual bool canPassThrough(); //i.e. can the actor pass through the object or not.
    virtual void bonk() {};
    
private:
    bool m_isalive = true; //true is for alive and false is for dead.
    StudentWorld *m_sw;
};

class Block: public Actor {
public:
    Block(StudentWorld* p_sw, int startX, int startY, char power);
    virtual void doSomething();
    virtual bool canPassThrough();
    virtual void bonk();
    char getPower();
private:
    char m_power = 'n';
};

class Pipe: public Actor {
public:
    Pipe(StudentWorld* psw, int startX, int startY);
    virtual void doSomething();
    virtual bool canPassThrough();
};

//Goodies - decalaration//
class Goodies: public Actor {
public:
    Goodies(StudentWorld *p_sw, int startX, int startY, int ImageID);
    virtual void doSomething() = 0;
    ~Goodies() {};
private:
};

class Mushroom: public Goodies {
public:
    Mushroom(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething();
private:
};

//--------------------//

class Peach: public Actor {
public:
    Peach(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
private:
    int m_hitPoints = 1;
    int m_invincibilityTicker = 0;
    bool m_hasStarPower = false;
    bool m_hasFirePower = false;
    bool m_hasJumpPower = false;
    
    int m_jumpDist;
    bool m_isJumping = false;

};


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


#endif // ACTOR_H_
