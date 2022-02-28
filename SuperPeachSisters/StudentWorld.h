#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include "Actor.h"
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Peach* getPeach();
    void increaseLevel();
    void wonGame();
    
    //extra helper functions -
    void addActor(Actor *newActor);
    bool bonkAt(double x, double y);
    bool damageAt(double x, double y);
    bool canMove(double x,double y);
    bool isIntersecting(double x, double y);
    
    virtual ~StudentWorld();
    
private:
    vector<Actor*> actors;
    Peach* m_peach;
    bool m_completed_level = false;
    bool m_wonGame = false;
};

#endif // STUDENTWORLD_H_
