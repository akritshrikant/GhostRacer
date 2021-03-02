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
        m_alive = true;
    }
    virtual void doSomething() = 0;
    virtual bool collisionWorthy() const {return false;}
    virtual StudentWorld* getWorld() const {return m_world;}
    virtual bool outOfBounds() const;
    virtual bool isAlive() const{return m_alive;}
    virtual bool holyWaterWeak() const = 0;
    virtual void hitWithWater() = 0;
    virtual void kill(){m_alive = false;}
private:
    bool m_isKiller;
    bool m_alive;
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
        m_waterCount = 10;
        m_world = thisWorld;
        m_alive = true;
        m_isKiller = false;
    }
    void doSomething();
    int getSpeed(){return m_speedForward;}
    int getHealth(){return m_health;}
    void damageRacer(int amt){m_health -= amt;}
    void addWater(int amt){m_waterCount += amt;}
    bool isAlive() const {return m_health > 0;}
    void changeSpeed(int amt) {m_speedForward += amt;}
    void dirFormula();
    bool holyWaterWeak() const{return false;}
    void hitWithWater(){}
    void spin();
    int getWater() const {return m_waterCount;}
    void getHealed(int amt);
    void killer(){m_isKiller = true;}
    bool isKiller(){return m_isKiller;}
    bool collisionWorthy() const {return true;}
private:
    bool m_isKiller;
    int m_alive;
    int m_health;
    int m_speedForward;
    int m_x;
    int m_y;
    int m_waterCount;
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
    void doSomething();
    int getSpeed(){return m_vertSpeed;}
    bool holyWaterWeak() const{return false;}
    void hitWithWater() {}
private:
    int m_x;
    int m_y;
    bool m_alive;
    int m_vertSpeed;
    StudentWorld* m_world;
    
};

class HolyWaterProj: public Actor {
public:
    HolyWaterProj(StudentWorld* thisWorld, int x, int y, int dir)
    : Actor(thisWorld, IID_HOLY_WATER_PROJECTILE, x, y, dir, 1, 1)
    {
        m_x = x;
        m_y = y;
        m_world = thisWorld;
        m_dir = dir;
        m_dist = 0;
        m_alive = true;
    }
    void doSomething();
    void hitWithWater(){}
    bool holyWaterWeak() const {return false;}
    
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_dir;
    int m_dist;
    StudentWorld* m_world;
};

class OilSlick: public Actor {
public:
    OilSlick(StudentWorld* thisWorld, int x, int y)
    :Actor(thisWorld, IID_OIL_SLICK, x, y, 0, randInt(2, 5), 2)
    {
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
        m_alive = true;
    }
    void doSomething();
    bool holyWaterWeak() const{return false;}
    void hitWithWater(){}
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
};

class Goodie: public Actor {
public:
    Goodie(StudentWorld* thisWorld, int imageID, int dir, int x, int y, int size, unsigned int depth)
    :Actor(thisWorld, imageID, x, y, dir, size, depth)
    {
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
        m_alive = true;
    }
    virtual void doSomething();
    virtual void uniqueEffect() = 0;
    virtual void hitWithWater(){kill();}
    virtual bool holyWaterWeak() const{return true;}
    virtual bool isSoul() const{return false;}
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
};

class SoulGoodie: public Goodie {
public:
    SoulGoodie(StudentWorld* thisWorld, int x, int y)
    :Goodie(thisWorld, IID_SOUL_GOODIE, 0, x, y, 4, 2)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
    }
    void uniqueEffect();
    bool isSoul() const {return true;}
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
};

class WaterGoodie: public Goodie {
public:
    WaterGoodie(StudentWorld* thisWorld, int x, int y)
    :Goodie(thisWorld, IID_HOLY_WATER_GOODIE, 90, x, y, 2, 2)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
    }
    void uniqueEffect();
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
};

class HealGoodie: public Goodie {
public:
    HealGoodie(StudentWorld* thisWorld, int x, int y)
    :Goodie(thisWorld, IID_HEAL_GOODIE, 0, x, y, 1, 2)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
    }
    void uniqueEffect();
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
};

class Pedestrian: public Actor {
public:
    Pedestrian(StudentWorld* thisWorld, int imageID, int x, int y, int size, unsigned int depth)
    :Actor(thisWorld, imageID, x, y, 0, size, depth)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
        m_health = 2;
        m_movePlan = 0;
        m_horizSpeed = 0;
    }
    virtual void doSomething();
    virtual bool holyWaterWeak() const {return true;}
    virtual void overLapEffect() = 0;
    virtual void newMovePlan();
    virtual void zombieExtra(){return;}
    bool collisionWorthy() const{return true;}
    virtual bool hasHeart() const = 0;
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
    int m_health;
    int m_movePlan;
    int m_horizSpeed;
};

class HumanPed: public Pedestrian {
public:
    HumanPed(StudentWorld* thisWorld, int x, int y)
    :Pedestrian(thisWorld, IID_HUMAN_PED, x, y, 2, 0)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
        m_health = 2;
        m_movePlan = 0;
        m_horizSpeed = 0;
    }
    void overLapEffect();
    void hitWithWater();
    bool hasHeart() const{return true;}
private:
    bool m_alive;
    int m_x;
    int m_y;
    int m_vertSpeed;
    int m_health;
    int m_movePlan;
    int m_horizSpeed;
};

class ZombiePed: public Pedestrian {
public:
    ZombiePed(StudentWorld* thisWorld, int x, int y)
    :Pedestrian(thisWorld, IID_ZOMBIE_PED, x, y, 3, 0)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_vertSpeed = -4;
        m_health = 2;
        m_movePlan = 0;
        m_horizSpeed = 0;
        m_ticks = 0;
    }
    void hitWithWater();
    void overLapEffect();
    bool isAlive(){return m_health > 0;}
    void zombieExtra();
    bool hasHeart() const {return false;}
private:
    bool m_alive;
    int m_health;
    int m_x;
    int m_y;
    int m_vertSpeed;
    int m_horizSpeed;
    int m_movePlan;
    int m_ticks;
};

class ZombieCab: public Actor {
public:
    ZombieCab(StudentWorld* thisWorld, int x, int y, int leftEdge, int rightEdge, double speed)
    :Actor(thisWorld, IID_ZOMBIE_CAB, x, y, 90, 4, 0)
    {
        m_alive = true;
        m_x = x;
        m_y = y;
        m_horizSpeed = 0;
        m_movePlan = 0;
        m_health = 3;
        m_damagedRacer = false;
        m_leftEdgeLane = leftEdge;
        m_rightEdgeLane = rightEdge;
        m_vertSpeed = speed;
    }
    void doSomething();
    bool isAlive(){return m_health > 0 && m_alive;}
    bool aheadWorthyInLane(int leftEdge, int rightEdge, int& closest);
    bool behindWorthyInLane(int leftEdge, int rightEdge, int& closest);
    bool holyWaterWeak() const {return true;}
    void hitWithWater();
    bool collisionWorthy() const {return true;}
private:
    bool m_damagedRacer;
    int m_health;
    int m_alive;
    int m_x;
    int m_y;
    int m_horizSpeed;
    int m_movePlan;
    double m_vertSpeed;
    int m_leftEdgeLane;
    int m_rightEdgeLane;
};

#endif // ACTOR_H_

