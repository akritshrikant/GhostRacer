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
    bool overLaps(Actor* a1, Actor* a2);
    void addHolyWaterProj(int x, int y);
    void addOilSlick(int x, int y);
    void checkBorderLines();
    void takeOutTrash();
    void tryOilSlicks();
    void saveSoul(){soulsSaved++; soulsLeft--;};
    void trySouls();
    void addSoulGoodie(int x, int y);
    void addHealGoodie(int x, int y);
    void tryWaterGoodies();
    void addWaterGoodies(int x, int y);
    Actor* getListNum(int index);
    int listSize(){return actorList.size();}
    void addHumanPed(int x, int y);
    void tryHumanPeds();
    void addZombiePed(int x, int y);
    void tryZombiePeds();
    void addZombieCab(int x, int y, int leftEdge, int rightEdge, double speed);
    void tryZombieCabs();
private:
    std::vector<Actor*> actorList;
    GhostRacer* m_ghosty;
    int LEFT_EDGE;
    int RIGHT_EDGE;
    int N;
    int M;
    int m_lastY;
    int m_bonus;
    int souls2save;
    int soulsLeft;
    int soulsSaved;
};

#endif // STUDENTWORLD_H_
