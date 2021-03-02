#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

void StudentWorld::addBorderLine(int imageID, int x, int y)
{
    actorList.push_back(new BorderLine(this, imageID, x, y));
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    N = VIEW_HEIGHT / SPRITE_HEIGHT;
    M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    m_level = 1;
    m_lastY = VIEW_HEIGHT - SPRITE_HEIGHT;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_ghosty = new GhostRacer(this);
    actorList.push_back(m_ghosty);
    for (int i = 0; i < N; i++)
    {
        addBorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, SPRITE_HEIGHT* (N - (i + 1)));
        addBorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, SPRITE_HEIGHT * (N - (i + 1)));
    }
    for (int i = 0; i < M; i++)
    {
        addBorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, (M - (i + 1))*(4*SPRITE_HEIGHT));
        addBorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, (M - (i + 1))*(4*SPRITE_HEIGHT));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (vector<Actor*>::iterator it = actorList.begin();it != actorList.end(); it++)
        if ((*it) -> isAlive())
        {
            (*it) -> doSomething();
            if (!m_ghosty -> isAlive())
            {
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
        }
    m_lastY += (-4 - m_ghosty -> getSpeed());
    vector<Actor*>::iterator it = actorList.begin();
    while(it != actorList.end())
    {
        if (!(*it) -> isAlive())
        {
            Actor* p = (*it);
            it = actorList.erase(it);
            delete p;
        }
        else it++;
    }
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - m_lastY;
    if (delta_y >= SPRITE_HEIGHT)
    {
        addBorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y);
        addBorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y);
    }
    if (delta_y >= 4*SPRITE_HEIGHT)
    {
        addBorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 +
                      ROAD_WIDTH/3, new_border_y);
        addBorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 -
                      ROAD_WIDTH/3, new_border_y);
        m_lastY = new_border_y;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = actorList.begin();
    while (it != actorList.end())
    {
        Actor* p = (*it);
        it = actorList.erase(it);
        delete p;
    }
}
