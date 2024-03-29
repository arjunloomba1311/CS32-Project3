#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//Class declarations

class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(StudentWorld *p_sw, int imageID, int startX, int startY, int dir, int depth, int size);
    bool getAliveStatus();
    void killActor();
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    virtual bool canPassThrough(); //i.e. can the actor pass through the object or not.
    virtual void bonk() {};
    virtual void damage() {};
    virtual void falling(double x, double y);
    virtual bool isDamageable() {return false;};
    
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
    
    //set powers:
    void setJumpPower();
    void setFirePower();
    void setStarPower();
    
    //change other attributes
    void setHitPoints(int num);
    void setTempInvicibility(int num);
    void setRechargeTime(int num);
    
    //output power status
    bool hasStarPower();
    bool hasJumpPower();
    bool hasFirePower();
    
private:
    
    //check for attributes
    int m_hitPoints = 1;
    int m_tempInvincibilityTicker = 0;
    int m_time_to_recharge_before_next_fire = 0;

    //check for powers
    int m_starPowerTicker = 0;
    bool m_hasFirePower = false;
    bool m_hasJumpPower = false;
        
    int m_jumpDist = 0;

};

//-------Static Objects Declaration-----//

class staticObj: public Actor {
public:
    staticObj(StudentWorld *p_sw, int startX, int startY, int ImageID);
    virtual void doSomething();
    virtual bool canPassThrough();
    virtual void bonk();
private:
};

class Block: public staticObj {
public:
    Block(StudentWorld* p_sw, int startX, int startY, char power);
    virtual void bonk();
    void removePower();
private:
    char m_power = 'n';
};

class Pipe: public staticObj {
public:
    Pipe(StudentWorld* psw, int startX, int startY);
    virtual void bonk();
};

//-------Goodies Declaration-----//

class Goodies: public Actor {
public:
    Goodies(StudentWorld *p_sw, int startX, int startY, int ImageID);
    virtual void doSomething() = 0;
    void move();
    
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

//------Declaration of Enemies-------//

class Enemies: public Actor {
public:
    Enemies(StudentWorld *p_sw, int startX, int startY,  int ImageID);
    virtual void doSomething();
    virtual void bonk();
    void move();
    virtual void damage();
    virtual bool isDamageable(){return true;};
    ~Enemies() {};
private:
};

class Goomba: public Enemies {
public:
    Goomba(StudentWorld *p_sw, int startX, int startY);
    ~Goomba() {};
private:
};

class Koopa: public Enemies {
public:
    Koopa(StudentWorld *p_sw, int startX, int startY);
    virtual void damage();
    virtual void bonk();
    ~Koopa() {};
private:
};

class Piranha: public Enemies {
public:
    Piranha(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething();
    void setFiringDelay(int num);
    ~Piranha() {};
private:
    int m_firingDelay = 0;
    
};

//------Projectiles Declaration------//

class Projectiles: public Actor {
public:
    Projectiles(StudentWorld *p_sw, int startX, int startY,  int ImageID, int dir);
    virtual void doSomething();
    virtual void move();
private:

};

class peachFireball: public Projectiles {
public:
    peachFireball(StudentWorld *p_sw, int startX, int startY, int dir);
private:
};

class Shell: public Projectiles {
public:
    Shell(StudentWorld* p_sw, int startX, int startY, int dir);
private:
};

class piranhaFireball: public Projectiles {
public:
    piranhaFireball(StudentWorld* p_sw, int startX, int startY, int dir);
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
    virtual void doSomething();
};

class Mario: public Checkpoint {
public:
    Mario(StudentWorld *p_sw, int startX, int startY);
    virtual void doSomething();
private:
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


#endif // ACTOR_H_
