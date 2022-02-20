#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

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
                        break;
                    case Level::goomba:
                        break;
                    case Level::peach:
                        actors.push_back(new Peach(this, IID_PEACH, x, y));
                        break;
                    case Level::flag:
                        break;
                    case Level::block:
                        actors.push_back(new Block(this, IID_BLOCK, x, y));
                        break;
                    case Level::star_goodie_block:
                        break;
                    case Level::piranha:
                        break;
                    case Level::mushroom_goodie_block:
                        break;
                    case Level::flower_goodie_block:
                        break;
                    case Level::pipe:
                        break;
                    case Level::mario:
                        break;
                }
            }
        }
    
        
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    // to iterator over the *Actor vector
    vector<Actor*>::iterator it;
        
    for (it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->getAliveStatus()) {
            (*it)->doSomething();
        }
    }

    decLives();
    return GWSTATUS_CONTINUE_GAME; //need to change to GWSTATUS DIED//
}

bool StudentWorld::isVirtualBlockingObjectAt(double x, double y) {
    vector<Actor*>::iterator it;
    
    //check if a block is blocking ahead then should not pass it.
    for (it = actors.begin(); it != actors.end(); it++) {
        if (((*it)->getX() == x)|| (*it)->getX() < 0) {
            if ((*it)->getY() == y) {
            return true;
            }
        }
    }

    return false;
}

void StudentWorld::cleanUp()
{
    
    vector<Actor*>::iterator it;
    
    for (it = actors.begin(); it != actors.end(); it++) {
        delete *it;
    }
    
    actors.clear();
    
}
