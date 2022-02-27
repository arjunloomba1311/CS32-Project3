#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//------------Actor Implementation------------//

Actor::Actor(StudentWorld *p_sw, int ImageID, int startX, int startY, int dir, int depth, int size) :GraphObject(ImageID, startX, startY, dir, depth, size), m_sw(p_sw)
{
};

bool Actor::getAliveStatus() {
    return m_isalive;
}

void Actor::killActor() {
    m_isalive = false;
}

//by default this function is true.
bool Actor::canPassThrough() {
    return true;
}

//return the student world pointer
StudentWorld* Actor::getWorld() {
    return m_sw;
}

//-----------Projectiles Implementation ----------//

Projectiles::Projectiles(StudentWorld *p_sw, int startX, int startY,  int ImageID, int dir)
:Actor(p_sw, ImageID, startX, startY,dir, 0, 1) {};

void Projectiles::doSomething() {
    this->move();
    
    if (this->getWorld()->bonkAt(getX()+2, getY())) {
        
        this->killActor();
        
    }

};

void Projectiles::bonk() {};

void Projectiles::move() {
    
    //if intersects with a damageable element OR a wall then damage.
    
    //if either statements are true then projectile MUST DIE!
    
    int dir = this->getDirection();
        
    if (dir == 0) {
        if (getWorld()->canMoveThroughObject(getX()+2, getY())) {
            moveTo(getX() + 2, getY());
        }
        
        else {
            this->killActor();
        }
    }
    
    else if (dir == 180) {
        if (getWorld()->canMoveThroughObject(getX()-2, getY())) {
        moveTo(getX()-2, getY());
            
        }
        
        else {
            this->killActor();
        }
    }
    
    //implement falling
    
    if (getWorld()->canMoveThroughObject(getX(), getY() - SPRITE_HEIGHT/2)) {
        moveTo(getX(), getY() - 2);
    }

};

//-----------Peach Fireball Implementation ----------//
peachFireball::peachFireball(StudentWorld *p_sw, int startX, int startY, int dir)
:Projectiles(p_sw, startX, startY, IID_PEACH_FIRE, dir) {};

void peachFireball::bonk() {};

//-----------Piranha Fireball Implementation ----------//

piranhaFireball::piranhaFireball(StudentWorld *p_sw, int startX, int startY, int dir)
:Projectiles(p_sw, startX, startY, IID_PIRANHA_FIRE, dir) {};

void piranhaFireball::doSomething() {
    
    this->move();
    
    if (this->getWorld()->isIntersecting(getX()+2, getY())) {
        this->getWorld()->getPeach()->bonk();
        this->killActor();
    }
    
};

void piranhaFireball::bonk() {};

//-----------Shell Implementation ----------//

Shell::Shell(StudentWorld* p_sw, int startX, int startY, int dir)
:Projectiles(p_sw, startX, startY, IID_SHELL, dir){}

void Shell::bonk() {}

//-----------Enemies Implementation ----------//

Enemies::Enemies(StudentWorld *p_sw, int startX, int startY,  int ImageID)
:Actor(p_sw, ImageID, startX, startY,0, 0, 1) {}

void Enemies::doSomething() {};

void Enemies::bonk() {};

void Enemies::damage() {
    this->killActor();
};

void Enemies::move() {
    int dir = this->getDirection();
    
    if (dir == 0) {
        
        if (getWorld()->canMoveThroughObject(getX() + SPRITE_WIDTH/2, getY() - 1) || !getWorld()->canMoveThroughObject(getX() + 1, getY())) {
            this->setDirection(180);
        }
        
        else {
            
            moveTo(getX()+1, getY());

        }
    }
    
    else if (dir == 180) {
        
        if (getWorld()->canMoveThroughObject(getX() - SPRITE_WIDTH/2, getY() - 1) || !getWorld()->canMoveThroughObject(getX() -1 , getY())) {
            this->setDirection(0);
        }
        
        else {
            
            moveTo(getX()-1, getY());

        }
    }
    
};

//-----------Goomba Implementation ----------//

Goomba::Goomba(StudentWorld *p_sw, int startX, int startY)
:Enemies(p_sw, startX, startY, IID_GOOMBA) {};

void Goomba::doSomething() {
    this->move();
    if (getWorld()->isIntersecting(getX(), getY())) {
        
        if (getWorld()->getPeach()->hasStarPower()) {
            bonk();
        } else {
        getWorld()->getPeach()->bonk();
        }
        
    }
};

//for when goomba is hit SPECIFICALLY BY PEACH!
void Goomba::bonk() {
    getWorld()->getPeach()->increasePlayerScore(100);
    getWorld()->playSound(SOUND_PLAYER_KICK);
    this->killActor();

};

//-----------Koopa Implementation ----------//

Koopa::Koopa(StudentWorld *p_sw, int startX, int startY)
:Enemies(p_sw, startX, startY, IID_KOOPA) {};

void Koopa::doSomething() {
    this->move();
    if (getWorld()->isIntersecting(getX(), getY())) {
        
        if (getWorld()->getPeach()->hasStarPower()) {
            bonk();
        } else {
            
        getWorld()->getPeach()->bonk();
            return;
        }
        
    }
};

//for when koopa is hit by player with star power
void Koopa::bonk() {
    
    //same as goomba temporarily until shell isn't made.
    getWorld()->getPeach()->increasePlayerScore(100);
    getWorld()->playSound(SOUND_PLAYER_KICK);
    
    this->killActor();

};

//when koopa is hit by anything else
void Koopa::damage() {
    //create a new shell
    this->killActor();
    getWorld()->addActor(new Shell(getWorld(),getX(),getY(),getDirection()));

};

//-----------Piranha Implementation ----------//

Piranha::Piranha(StudentWorld *p_sw, int startX, int startY)
:Enemies(p_sw, startX, startY, IID_PIRANHA) {}

void Piranha::doSomething() {
    
    if (!this->getAliveStatus()) {
        return;
    }
    
    this->increaseAnimationNumber(); //as per spec animate the Pirahna.
    
    if (getWorld()->isIntersecting(getX(), getY())) {
        
        if (getWorld()->getPeach()->hasStarPower()) {
            bonk();
        }
        
        else {
            
        getWorld()->getPeach()->bonk();
            
        return;
            
        }
        
    }
    
    if (!(abs(this->getWorld()->getPeach()->getY() - this->getY()) < SPRITE_HEIGHT*1.5)) {
        return;
    }
    
    if (this->getWorld()->getPeach()->getDirection() == 0) {
        this->setDirection(180);
    }
    
    else if (this->getWorld()->getPeach()->getDirection() == 180) {
        this->setDirection(0);
    }
    
    if (m_firingDelay > 0) {
        m_firingDelay--;
        return;
    }
    
    if ((abs(this->getWorld()->getPeach()->getX() - this->getX()) < SPRITE_HEIGHT*8)) {
        getWorld()->addActor(new piranhaFireball(getWorld(), getX(), getY(), this->getDirection()));
        getWorld()->playSound(SOUND_PIRANHA_FIRE);
        this->setFiringDelay(40);
                
    }

    
};

void Piranha::setFiringDelay(int num) {
    m_firingDelay = num;
}

void Piranha::bonk() {
    
    getWorld()->getPeach()->increasePlayerScore(100);
    getWorld()->playSound(SOUND_PLAYER_KICK);
    
    this->killActor();
    
};

void Piranha::damage() {
    this->killActor();
};









//-----------Goodies Implementation ----------//

Goodies::Goodies(StudentWorld *p_sw, int startX, int startY, int ImageID)
:Actor(p_sw, ImageID, startX, startY,0, 1, 1) // depth, & size are constant

{}

void Goodies::move() {
    int dir = this->getDirection();
        
    if (dir == 0) {
        if (getWorld()->canMoveThroughObject(getX()+2, getY())) {
            moveTo(getX() + 2, getY());
        }
        
        else {
            
            this->setDirection(180);
            return;

        }
    }
    
    else if (dir == 180) {
        if (getWorld()->canMoveThroughObject(getX()-2, getY())) {
        moveTo(getX()-2, getY());
            
        }
        
        else {
            
            this->setDirection(0);
            return;
        }
    }
}

//-----------Mushroom Implementation ----------//

Mushroom::Mushroom(StudentWorld *p_sw, int startX, int startY)
:Goodies(p_sw, startX, startY, IID_MUSHROOM) {}

void Mushroom::doSomething() {
    //check for intersection
    if (getWorld()->isIntersecting(getX(), getY())) {
        
        getWorld()->getPeach()->increasePlayerScore(75);
        getWorld()->getPeach()->setHitPoints(2);
        getWorld()->getPeach()->setJumpPower();
        this->killActor();
        
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
                
        return;
    }
    
    //implement falling
    if (getWorld()->canMoveThroughObject(getX(), getY() - SPRITE_HEIGHT/2)) {
        moveTo(getX(), getY() - 2);
    }
    
    //implement moving
    this->move();
    
};

//-----------Flower Implementation ----------//

Flower::Flower(StudentWorld *p_sw, int startX, int startY) :Goodies(p_sw, startX, startY, IID_FLOWER) {}

void Flower::doSomething() {
    
    if (getWorld()->isIntersecting(getX(), getY())) {
        getWorld()->getPeach()->increasePlayerScore(50);
        getWorld()->getPeach()->setHitPoints(2);
        getWorld()->getPeach()->setFirePower();
        this->killActor();
        
        getWorld()->playSound(SOUND_PLAYER_POWERUP);

    }
    
    if (getWorld()->canMoveThroughObject(getX(), getY() - SPRITE_HEIGHT/2)) {
        moveTo(getX(), getY() - 2);
    }
    
    this->move();
}

//-----------Star Implementation ----------//

Star::Star(StudentWorld *p_sw, int startX, int startY) :Goodies(p_sw, startX, startY, IID_STAR) {}

void Star::doSomething() {
    
    if (getWorld()->isIntersecting(getX(), getY())) {
        getWorld()->getPeach()->increasePlayerScore(100);
        getWorld()->getPeach()->setHitPoints(2);
        getWorld()->getPeach()->setStarPower();
        this->killActor();
        
        getWorld()->playSound(SOUND_PLAYER_POWERUP);

    }
    
    else if (getWorld()->canMoveThroughObject(getX(), getY() - SPRITE_HEIGHT/2)) {
        moveTo(getX(), getY() - 2);
    }
    
    else {
        this->move();
    }
}

//-----------Checkpoint Implementation ----------//
Checkpoint::Checkpoint(StudentWorld *p_sw, int startX, int startY, int ImageID)
:Actor(p_sw, ImageID, startX, startY,0, 1, 1) {}


//-----------Flag Implementation ----------//
Flag::Flag(StudentWorld *p_sw, int startX, int startY)
:Checkpoint(p_sw, startX, startY, IID_FLAG) {}

void Flag::doSomething() {
    if (getWorld()->isIntersecting(getX(), getY())) {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->getPeach()->increasePlayerScore(1000);
        
        getWorld()->increaseLevel();

        this->killActor();
    }
}

//-----------Mario Implementation ---------//

Mario::Mario(StudentWorld *p_sw, int startX, int startY)
:Checkpoint(p_sw, startX, startY, IID_MARIO) {}

//------------Static Object Implementation------------//

staticObj::staticObj(StudentWorld *p_sw, int startX, int startY, int ImageID)
:Actor(p_sw, ImageID, startX, startY, 0, 0, 1.0) {};

void staticObj::doSomething() {}

bool staticObj::canPassThrough() {
    return false;
}

void staticObj::bonk() {}


//------------Block Implementation------------//


Block::Block(StudentWorld *p_sw, int startX, int startY, char power)
:staticObj(p_sw, startX, startY, IID_BLOCK) //  depth, & size are constant
{
    m_power = power;
}

char Block::getPower() {
    return m_power;
}

void Block::removePower() {
    m_power = 'n';
}

void Block::doSomething() {}

void Block::bonk() {
    getWorld()->playSound(SOUND_PLAYER_BONK);
    
    if (m_power != 'n') {
        
        if (m_power == 'm') {
            getWorld()->addActor(new Mushroom(getWorld(), getX(), getY() + SPRITE_HEIGHT));
        }
        
        if (m_power == 's') {
            getWorld()->addActor(new Star(getWorld(), getX(), getY() + SPRITE_HEIGHT));
        }
        
        if (m_power == 'f') {
            getWorld()->addActor(new Flower(getWorld(), getX(), getY() + SPRITE_HEIGHT));
        }
        
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        
        this->removePower();
    }
}

//------------Pipe Implementation------------//

Pipe::Pipe(StudentWorld *p_sw, int startX, int startY) :staticObj(p_sw, startX, startY, IID_PIPE) {}

void Pipe::doSomething() {}

void Pipe::bonk() {
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

//------------Peach Implementation------------//

Peach::Peach(StudentWorld *p_sw, int startX, int startY) :Actor(p_sw, IID_PEACH, startX, startY,0, 0, 1.0) {
    m_jumpDist = 0; // no mushroom power on initialization
}

bool Peach::hasStarPower() {
    if (m_starPowerTicker > 0) {
        return true;
    }
    
    return false;
}

void Peach::bonk() {
    
    if (m_tempInvincibilityTicker > 0 || m_starPowerTicker > 0) {
        return;
    }
    
    m_hitPoints--;
    m_tempInvincibilityTicker = 10;
    m_hasFirePower = false;
    m_hasJumpPower = false;
    
    if (m_hitPoints > 0) {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    
    if (m_hitPoints <= 0) {
        this->killActor();
    }
    
}

void Peach::setJumpPower() {
    m_hasJumpPower = true;
}

void Peach::setFirePower() {
    m_hasFirePower = true;
}

void Peach::setStarPower() {
    m_starPowerTicker = 150;
}

void Peach::increasePlayerScore(int num) {
    m_score += num;
}

void Peach::setHitPoints(int num) {
    m_hitPoints = num;
}
    
void Peach::setTempInvicibility(int num) {
    m_tempInvincibilityTicker = 10;
}

void Peach::setRechargeTime(int num) {
    m_time_to_recharge_before_next_fire = 8;
}

void Peach::doSomething() {
        
    //decrease star power
    if (m_starPowerTicker > 0) {
        m_starPowerTicker--;
    }
    
    //decrease temporary invincibility
    if (m_tempInvincibilityTicker > 0) {
        m_tempInvincibilityTicker--;
    }
    
    if (m_time_to_recharge_before_next_fire > 0) {
        m_time_to_recharge_before_next_fire--;
    }

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
            
        else {
            getWorld()->bonkAt(getX() - SPRITE_WIDTH/2, getY());

        }
            
        break;
    case KEY_PRESS_RIGHT:
        setDirection(0);
        if ((getWorld()->canMoveThroughObject(currX + SPRITE_WIDTH/2, currY))) {
            moveTo(currX + (SPRITE_WIDTH/2), currY);
        }
            
        else {
            getWorld()->bonkAt(getX() + SPRITE_WIDTH/2, getY());
        }
        
        break;
    case KEY_PRESS_SPACE:
            
        if (m_hasFirePower && m_time_to_recharge_before_next_fire <= 0) {
            getWorld()->addActor(new peachFireball(getWorld(), getX(), getY(),getDirection()));
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            setRechargeTime(8);
        }
        
        break;
            
    case KEY_PRESS_UP:
            
            
            //check if jump is possible
            if (!getWorld()->canMoveThroughObject(currX, currY - SPRITE_HEIGHT/2)) {
                //check if there's something below
                
                //check if jump power is active
                
                //play jump sound
                getWorld()->playSound(SOUND_PLAYER_JUMP);
                
                if (m_hasJumpPower) {
                
                m_jumpDist = 12;
                    
                }
                
                else {
                    m_jumpDist = 8;
                }
               
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
