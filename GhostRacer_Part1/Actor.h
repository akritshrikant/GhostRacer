#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class StudentWorld;
class Actor: public GraphObject {
public:
    Actor(StudentWorld* thisWorld, int imageID, int x, int y, int dir, double size, unsigned int depth)
    : GraphObject(imageID, x, y, dir, size, depth)
    {
        m_dir = dir;
        m_depth = depth;
        m_x = x;
        m_y = y;
        m_size = size;
        m_world = thisWorld;
    }
    virtual bool canAct() const {return true;}
    virtual bool isActivatable() const {return false;}
    virtual bool isDamagable() const {return true;}
    virtual void doSomething() = 0;
    virtual bool collisionWorthy() const {return true;}
    StudentWorld* getWorld() const {return m_world;}
    virtual bool outOfBounds() const;
    virtual bool isAlive() const = 0;
private:
    int m_x;
    int m_y;
    int m_dir;
    int m_depth;
    int m_size;
    StudentWorld* m_world;
};

class GhostRacer: public Actor {
public:
    GhostRacer(StudentWorld* thisWorld)
    : Actor(thisWorld, IID_GHOST_RACER, 128, 32, 90, 4, 0)
    {
        m_health = 100;
        m_speedForward = 0;
        holyWaterCount = 10;
        m_world = thisWorld;
    }
    void doSomething();
    int getSpeed(){return m_speedForward;}
    int getHealth(){return m_health;}
    void damageRacer(int amt){m_health -= amt;}
    bool isAlive() const {return m_health > 0;}
    void changeSpeed(int amt) {m_speedForward += amt;}
    void dirFormula();
private:
    int m_health;
    int m_speedForward;
    int m_x;
    int m_y;
    int holyWaterCount;
    int m_dir;
    int m_depth;
    int m_size;
    StudentWorld* m_world;
};

class BorderLine: public Actor {
public:
    BorderLine(StudentWorld* thisWorld, int imageID, int x, int y)
    : Actor(thisWorld, imageID, x, y, 0, 2, 1)
    {
        m_vertSpeed = -4;
        m_alive = true;
    }
    bool isDamagable(){return false;}
    bool collisionWorthy(){return true;}
    bool isAlive(){return m_alive;}
    void doSomething();
    int getSpeed(){return m_vertSpeed;}
    void kill(){m_alive = false;}
    bool isAlive() const{return m_alive;}
private:
    int m_x;
    int m_y;
    bool m_alive;
    int m_vertSpeed;
    StudentWorld* m_world;
    
};
#endif // ACTOR_H_

