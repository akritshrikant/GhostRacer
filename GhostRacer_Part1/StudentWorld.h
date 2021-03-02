#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    int getLeftEdge(){return LEFT_EDGE;}
    int getRightEdge(){return RIGHT_EDGE;}
    void addBorderLine(int imageID, int x, int y);
    GhostRacer* getGhosty(){return m_ghosty;}
    int getN() const {return N;}
private:
    std::vector<Actor*> actorList;
    GhostRacer* m_ghosty;
    int LEFT_EDGE;
    int RIGHT_EDGE;
    int N;
    int m_level;
    int M;
    int m_lastY;
};

#endif // STUDENTWORLD_H_
