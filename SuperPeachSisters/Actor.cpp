#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//------------Actor Implementation------------//

Actor::Actor(StudentWorld *p_sw, int ImageID, int startX, int startY, int depth, int size) :GraphObject(ImageID, startX, startY, depth, size = 1.0), m_sw(p_sw)
{
};

void Actor::setDir(int dir) {
    setDirection(0);
}

bool Actor::getAliveStatus() {
    return m_isalive;
}

void Actor::killActor() {
    m_isalive = false;
}

//return the student world pointer
StudentWorld* Actor::getWorld() {
    return m_sw;
}

//------------Block Implementation------------//

Block::Block(StudentWorld *p_sw, int ImageID, int startX, int startY)
:Actor(p_sw, IID_BLOCK, startX, startY, 0, 1.0) // dir, depth, & size are constant
{}

void Block::doSomething() {}

void Block::bonk() {
    if (getAliveStatus() == true) {
        killActor();
    }
}

//------------Peach Implementation------------//

Peach::Peach(StudentWorld *p_sw, int ImageID, int startX, int startY) :Actor(p_sw, IID_PEACH, startX, startY, 0, 1.0) {
}

void Peach::doSomething() {
    
    int ch;
    if (getWorld()->getKey(ch)) {
    // user hit a key during this tick!
        double currX = getX();
        double currY = getY();
        switch (ch)
    {
    case KEY_PRESS_LEFT:
        
        setDirection(180); //set direction to 180 degrees
            
        if (!(getWorld()->isVirtualBlockingObjectAt(currX - 8, currY))) {
            moveTo(currX - 4, currY);
        }
            
        break;
    case KEY_PRESS_RIGHT:
        setDirection(0);
        if (!(getWorld()->isVirtualBlockingObjectAt(currX + 8, currY))) {
            moveTo(currX + 4, currY);
        }
        
        break;
    case KEY_PRESS_SPACE:
        break;
            
    case KEY_PRESS_UP:
        break;
            
    case KEY_PRESS_DOWN:
            break;
        }
    
    }
}

