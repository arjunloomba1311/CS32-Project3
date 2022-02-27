#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include "math.h"

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    
    //set completed level to false
    m_completed_level = false;
    
    //creating the level file
    Level lev(assetPath());
    int curr_lev = getLevel();
    ostringstream oss;
    oss.fill('0');
    oss << setw(2) << curr_lev;
    string level_file = "level" + oss.str() + ".txt";
    
    //generating the level and creating the objects into the vector
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
    cerr << "Could not find level data file" << endl;
    } else if (result == Level::load_fail_bad_format) {
    cerr << "level01.txt is improperly formatted" << endl;
    } else if (result == Level::load_success) {
    cerr << "Successfully loaded level" << endl;
        
    Level::GridEntry ge;
        
    //two nested loops needed to get all the elements from level{num}.txt
        
        for (int rowIter = 0; rowIter < GRID_HEIGHT; rowIter++) {
            for (int colIter = 0; colIter < GRID_WIDTH; colIter++) {
                ge = lev.getContentsOf(rowIter, colIter);
                int x = rowIter * SPRITE_WIDTH;
                int y = colIter * SPRITE_HEIGHT;
                switch(ge) {
                    
                    case Level::empty:
                        break;
                    case Level::koopa:
                        actors.push_back(new Koopa(this, x, y));
                        break;
                    case Level::goomba:
                        actors.push_back(new Goomba(this, x, y));
                        break;
                    case Level::peach:
                        m_peach = new Peach(this, x, y);
                        actors.push_back(m_peach);
                        break;
                    case Level::flag:
                        actors.push_back(new Flag(this, x, y));
                        break;
                    case Level::block:
                        actors.push_back(new Block(this, x, y, 'n'));
                        break;
                    case Level::star_goodie_block:
                        actors.push_back(new Block(this, x, y, 's'));
                        break;
                    case Level::piranha:
                        actors.push_back(new Piranha(this, x, y));
                        break;
                    case Level::mushroom_goodie_block:
                        actors.push_back(new Block(this, x, y, 'm'));
                        break;
                    case Level::flower_goodie_block:
                        actors.push_back(new Block(this, x, y, 'f'));
                        break;
                    case Level::pipe:
                        actors.push_back(new Pipe(this, x, y));
                        break;
                    case Level::mario:
                        actors.push_back(new Mario(this, x, y));
                        break;
                }
            }
        }
    
        
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
        
    if (!getPeach()->getAliveStatus()) {
        decLives();
        
        if (getLives() <= 0) {
            playSound(SOUND_GAME_OVER);
        }
        
        else {
        
        playSound(SOUND_PLAYER_DIE);
            
        }
        
        return GWSTATUS_PLAYER_DIED;
    }
    
    vector<Actor*>::iterator del_it;
    
    del_it = actors.begin();
    
    while (del_it != actors.end()) {
        if (!(*del_it)->getAliveStatus()) {
            delete (*del_it);
            del_it = actors.erase(del_it);
        }
        
        else {
            del_it++;
        }
    }
    
    vector<Actor*>::iterator it;
            
    for (it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->getAliveStatus()) {
            (*it)->doSomething();
        }
    }
    
    
    if (m_completed_level) {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME; //need to change to GWSTATUS DIED eventually//
}

void StudentWorld::increaseLevel() {
    m_completed_level = true;
}

//check for peach intersecting
bool StudentWorld::isIntersecting(double x, double y) {
    
    vector<Actor*>::iterator it;
    
    for (it = actors.begin(); it != actors.end(); it++) {
        if ((abs(x - (*it)->getX()) < SPRITE_WIDTH/2) && abs(y - (*it)->getY()) < SPRITE_HEIGHT/2) {
            if ((*it)->isPlayer()) { //check if it's intersecting with peach or not.
                return true;
            }
        }
    }
    
    return false;

}

Peach* StudentWorld::getPeach() {
    return m_peach;
}

//need to bonk or damage the damageables
bool StudentWorld::bonkAt(double x, double y) {
    vector<Actor*>::iterator it;
    
    for (it = actors.begin(); it != actors.end(); it++) {
        if ((abs(x - (*it)->getX()) < SPRITE_WIDTH) && abs(y - (*it)->getY()) < SPRITE_HEIGHT) {
            if (!(*it)->canPassThrough()) {
                (*it)->bonk();
                return true;
            }
            
            if ((*it)->isDamageable() && (*it)->getAliveStatus()) {
            (*it)->damage();
                return true;
            }
        }
    }
    
    return false;
}

//check if there's a block that's blocking 
bool StudentWorld::canMoveThroughObject(double x, double y) {
    vector<Actor*>::iterator it;
    
    //check if a block is blocking ahead then should not pass it.
    for (it = actors.begin(); it != actors.end(); it++) {
        
        if ((abs(x - (*it)->getX()) < SPRITE_WIDTH) && abs(y - (*it)->getY()) < SPRITE_HEIGHT) {
    
            if (!(*it)->canPassThrough()) {
                return false;
            }
        }
    }
    
    return true;
}

void StudentWorld::cleanUp()

{
    
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        delete *it;
    }
    
    actors.clear();
    
    }

void StudentWorld::addActor(Actor *newActor) {
    actors.push_back(newActor);
}


StudentWorld::~StudentWorld() {
    cleanUp();
}
