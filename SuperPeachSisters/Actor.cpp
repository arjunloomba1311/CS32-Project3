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

bool Actor::canPassThrough() {
    return true;
}

//return the student world pointer
StudentWorld* Actor::getWorld() {
    return m_sw;
}

//-----------Goodies Implementation ----------//

Goodies::Goodies(StudentWorld *p_sw, int startX, int startY, int ImageID)
:Actor(p_sw, ImageID, startX, startY, 2, 1.0) // depth, & size are constant

{
    
}

Mushroom::Mushroom(StudentWorld *p_sw, int startX, int startY)
:Goodies(p_sw, startX, startY, IID_MUSHROOM) {}

void Mushroom::doSomething() {};

//------------Block Implementation------------//

Block::Block(StudentWorld *p_sw, int startX, int startY, char power)
:Actor(p_sw, IID_BLOCK, startX, startY, 0, 1.0) //  depth, & size are constant
{
    m_power = power;
}

bool Block::canPassThrough() {
    return false;
}

char Block::getPower() {
    return m_power;
}

void Block::doSomething() {}

void Block::bonk() {
    getWorld()->playSound(SOUND_PLAYER_BONK);
    
    if (m_power != 'n') {
        getWorld()->AppendToActors(m_power, getX(), getY()+SPRITE_HEIGHT);
    }
}

//------------Pipe Implementation------------//
Pipe::Pipe(StudentWorld *p_sw, int startX, int startY) :Actor(p_sw, IID_PIPE, startX, startY, 0, 1.0) {}

bool Pipe::canPassThrough() {
    return false;
}

void Pipe::doSomething() {}

//------------Peach Implementation------------//

Peach::Peach(StudentWorld *p_sw, int startX, int startY) :Actor(p_sw, IID_PEACH, startX, startY, 0, 1.0) {
    m_jumpDist = 0; // no mushroom power on initialization
}

void Peach::bonk() {}

void Peach::doSomething() {
    
    if (m_jumpDist > 0) {
        
        if ((getWorld()->canMoveThroughObject(getX(), getY() + SPRITE_HEIGHT/2))) {
            moveTo(getX(), getY() + SPRITE_HEIGHT/2);
            m_jumpDist -= 1; //reduce jump dist by 1
        }
        
        else {
            getWorld()->bonkAt(getX(), getY() + SPRITE_HEIGHT/2);
            m_jumpDist = 0;
        }
        
    } else if (m_jumpDist <= 0) {
        
        //check to see if falling
        if (getWorld()->canMoveThroughObject(getX(), getY() - SPRITE_HEIGHT/2)) {
            moveTo(getX(), getY() - SPRITE_HEIGHT/2);
        }
    }
    
    int ch;
    if (getWorld()->getKey(ch)) {
    // user hit a key during this tick!
        double currX = getX();
        double currY = getY();
        switch (ch)
    {
    case KEY_PRESS_LEFT:
        
        setDirection(180); //set direction to 180 degrees
        if ((getWorld()->canMoveThroughObject(currX - SPRITE_WIDTH/2, currY))) {
            moveTo(currX - (SPRITE_WIDTH/2), currY);
        }
            
        break;
    case KEY_PRESS_RIGHT:
        setDirection(0);
        if ((getWorld()->canMoveThroughObject(currX + SPRITE_WIDTH/2, currY))) {
            moveTo(currX + (SPRITE_WIDTH/2), currY);
        }
        
        break;
    case KEY_PRESS_SPACE:
        break;
            
    case KEY_PRESS_UP:
            
            //play jump sound
            getWorld()->playSound(SOUND_PLAYER_JUMP);
            
            //check if jump is possible
            if (!getWorld()->canMoveThroughObject(currX, currY - SPRITE_HEIGHT/2)) {
                //check if there's something below
                m_jumpDist = 8;
               
                if ((getWorld()->canMoveThroughObject(currX, currY + SPRITE_HEIGHT/2))) {
                    moveTo(currX, currY + SPRITE_HEIGHT/2);
                    m_jumpDist -= 1; //reduce jump dist by 1
                }
                
                else {
                    //we need to bonk the block.
                    
                    getWorld()->bonkAt(currX, currY);
                    
                    //can no longer keep jumping
                    m_jumpDist = 0;
                }
                
            }
            

        break;
            
    case KEY_PRESS_DOWN:
            
            if ((getWorld()->canMoveThroughObject(currX, currY - SPRITE_HEIGHT/2))) {
                moveTo(currX, currY - SPRITE_HEIGHT/2);
            }
            break;
        }
    
    }
}

