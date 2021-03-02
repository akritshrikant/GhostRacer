#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

using namespace std;

bool Actor::outOfBounds() const
{
    //improvements later
    if (getY() < 0 || getY() > VIEW_HEIGHT || getX() < 0 || getX() > VIEW_WIDTH)
        return true;
    return false;
}

void GhostRacer::getHealed(int amt)
{
    if (m_health + amt >= 100)
        m_health = 100;
    m_health += amt;
}

void GhostRacer::dirFormula()
{
    double dir = getDirection();
    double delta_x = cos(dir * (M_PI/ 180)) * 4.0;
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::spin()
{
    int i = randInt(1, 2);
    if (i == 1)
    {
        int newDir = randInt(5, 20);
        if (getDirection() + newDir > 120)
            setDirection(120);
        else setDirection(getDirection() + newDir);
        return;
    }
    int newDir = randInt(-20, -5);
    if (getDirection() + newDir < 60)
        setDirection(60);
    else setDirection(getDirection() + newDir);
}
void GhostRacer::doSomething()
{
    if (getHealth() <= 0) return;
    int ch;
    if (getX() <= getWorld() -> getLeftEdge())
    {
        if (getDirection() > 90)
            damageRacer(10);
        setDirection(82);
        getWorld() -> playSound(SOUND_VEHICLE_CRASH);
    }
    else if(getX() >= getWorld() -> getRightEdge())
    {
        getWorld() -> playSound(SOUND_VEHICLE_CRASH);
        if (getDirection() < 90)
             damageRacer(10);
        setDirection(98);
    }
    else if (getWorld() -> getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_SPACE:
                if (m_waterCount > 0)
                {
                    getWorld() -> playSound(SOUND_PLAYER_SPRAY);
                    getWorld() -> addHolyWaterProj(getX() + (SPRITE_HEIGHT * cos(getDirection() * (M_PI / 180))), getY() + (SPRITE_HEIGHT * sin(getDirection() * (M_PI / 180))));
                    m_waterCount--;
                }
                break;
            case KEY_PRESS_LEFT:
                if (getDirection() < 114)
                    setDirection(getDirection() + 8);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() > 66)
                    setDirection(getDirection() - 8); 
                break;
            case KEY_PRESS_UP:
                if (getSpeed() < 5)
                    changeSpeed(1);
                break;
            case KEY_PRESS_DOWN:
                if (getSpeed() > -1)
                    changeSpeed(-1);
            default:
                break;
        }
    }
    dirFormula();
}
void BorderLine::doSomething()
{
    int delta_y = (m_vertSpeed - getWorld() -> getGhosty() -> getSpeed());
    moveTo(getX(), getY() + delta_y);
    if (outOfBounds())
        kill();
}

void HolyWaterProj::doSomething()
{
    if (!isAlive()) return;
    for (int i = 0; i < getWorld() -> listSize(); i++)
    {
        Actor* a2 = getWorld() -> getListNum(i);
        if (a2 -> holyWaterWeak() && getWorld() -> overLaps(this, a2))
        {
            a2 -> hitWithWater();
            kill();
            return;
        }
    }
    moveForward(SPRITE_HEIGHT);
    m_dist += SPRITE_HEIGHT;
    if (outOfBounds())
    {
        kill();
        return;
    }
    if (m_dist >= 160)
    {
        kill();
        return;
    }
}

void OilSlick::doSomething()
{
    if (!isAlive()) return;
    int vert_speed = m_vertSpeed - getWorld() -> getGhosty() -> getSpeed();
    int new_y = getY() + vert_speed;
    int new_x = getX();
    moveTo(new_x, new_y);
    if (outOfBounds())
        kill();
    if (getWorld() -> overLaps(this, getWorld() -> getGhosty()))
    {
        getWorld() -> playSound(SOUND_OIL_SLICK);
        getWorld() -> getGhosty() -> spin();
    }
}

void Goodie::doSomething()
{
    if (!isAlive()) return;
    int vert_speed = m_vertSpeed - getWorld() -> getGhosty() -> getSpeed();
    int new_y = getY() + vert_speed;
    moveTo(getX(), new_y);
    if (outOfBounds())
        kill();
    if (getWorld() -> overLaps(this, getWorld() -> getGhosty()))
        uniqueEffect();
    if (isSoul())
        setDirection(getDirection() + 10);
}

void Pedestrian::doSomething()
{
    if (!isAlive()) return;
    if (getWorld() -> overLaps(this, getWorld() -> getGhosty()))
    {
        overLapEffect();
        return;
    }
    zombieExtra();
    double vert_speed = m_vertSpeed - getWorld() -> getGhosty() -> getSpeed();
    double horizSpeed = m_horizSpeed;
    double new_y = getY() + vert_speed;
    double new_x = getX() + horizSpeed;
    moveTo(new_x, new_y);
    if (outOfBounds()) kill();
    m_movePlan--;
    if (m_movePlan > 0) return;
    newMovePlan();
}

void Pedestrian::newMovePlan()
{
    int p1 = randInt(-3, -1);
    int p2 = randInt(1, 3);
    int rando = randInt(1, 2);
    if (rando == 1) m_horizSpeed = p1;
    else m_horizSpeed = p2;
    m_movePlan = randInt(4, 32);
    int ghost_x = getWorld() -> getGhosty() -> getX();
    int ghost_y = getWorld() -> getGhosty() -> getY();
    if (!hasHeart() && abs(ghost_x - getX()) <= 30 && getY() > ghost_y)
    {}
    else
    if (m_horizSpeed < 0) setDirection(180);
    else setDirection(0);
}

void HumanPed::hitWithWater()
{
    m_horizSpeed *= -1;
    setDirection(getDirection() + 180);
    getWorld() -> playSound(SOUND_PED_HURT);
}

void SoulGoodie::uniqueEffect()
{
    getWorld() -> saveSoul();
    kill();
    getWorld() -> playSound(SOUND_GOT_SOUL);
    getWorld() -> increaseScore(100);
}

void WaterGoodie::uniqueEffect()
{
    getWorld() -> getGhosty() -> addWater(10);
    kill();
    getWorld() -> playSound(SOUND_GOT_GOODIE);
    getWorld() -> increaseScore(50);
}

void HealGoodie::uniqueEffect()
{
    getWorld() -> getGhosty() -> getHealed(10);
    kill();
    getWorld() -> playSound(SOUND_GOT_GOODIE);
    getWorld() -> increaseScore(250);
}

void HumanPed::overLapEffect()
{
    getWorld() -> getGhosty() -> killer();
    kill();
}

void ZombiePed::overLapEffect()
{
    getWorld() -> getGhosty() -> damageRacer(5);
    m_health -= 2;
    getWorld() -> playSound(SOUND_PED_DIE);
    kill();
    getWorld() -> playSound(SOUND_PED_DIE);
    int rando = randInt(1, 5);
    if (rando == 1)
        getWorld() -> addHealGoodie(getX(), getY());
    getWorld() -> increaseScore(150);
}

void ZombiePed::hitWithWater()
{
    m_health--;
    if (m_health <= 0)
    {
        kill();
        getWorld() -> playSound(SOUND_PED_DIE);
        int rando = randInt(1, 5);
        if (rando == 1)
            getWorld() -> addHealGoodie(getX(), getY());
        getWorld() -> increaseScore(150);
        return;
    }
    getWorld() -> playSound(SOUND_PED_HURT);
}

void ZombiePed::zombieExtra()
{
    int ghost_x = getWorld() -> getGhosty() -> getX();
    int ghost_y = getWorld() -> getGhosty() -> getY();
    if (abs(getX() - ghost_x) <= 30 && getY() > ghost_y)
    {
        setDirection(270);
        if (getX() < ghost_x)
            m_horizSpeed = 1;
        else if (getX() > ghost_x)
            m_horizSpeed = -1;
        else m_horizSpeed = 0;
    }
    m_ticks--;
    if (m_ticks <= 0)
    {
        getWorld() -> playSound(SOUND_ZOMBIE_ATTACK);
        m_ticks = 20;
    }
}

void ZombieCab::doSomething()
{
    if (!isAlive()) return;
    if (getWorld() -> overLaps(this, getWorld() -> getGhosty()))
    {
        if (!m_damagedRacer)
        {
            GhostRacer* ghosty = getWorld() -> getGhosty();
            getWorld() -> playSound(SOUND_VEHICLE_CRASH);
            getWorld() -> getGhosty() -> damageRacer(20);
            if (getX() <= ghosty -> getX())
            {
                m_horizSpeed = -5;
                setDirection(120 + randInt(0, 20));
            }
            else
            {
                m_horizSpeed = 5;
                setDirection(60 + randInt(0, 20));
            }
            m_damagedRacer = true;
        }
    }
    double ghost_speed = getWorld() -> getGhosty() -> getSpeed();
    double vert_speed = m_vertSpeed - ghost_speed;
    double horiz_speed = m_horizSpeed;
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, getY() + vert_speed);
    if (outOfBounds())
    {
        kill();
        m_health -= 20;
    }
    if (m_vertSpeed > ghost_speed)
    {
        int closest = 99;
        if (aheadWorthyInLane(m_leftEdgeLane, m_rightEdgeLane, closest)
            && closest < 96)
        {
            m_vertSpeed -= 0.5;
            return;
        }
    }
    if (m_vertSpeed <= ghost_speed)
    {
        int closest = 99;
        if (behindWorthyInLane(m_leftEdgeLane, m_rightEdgeLane, closest)
            && closest < 96)
        {
            m_vertSpeed += 0.5;
            return;
        }
    }
    m_movePlan--;
    if (m_movePlan > 0)
        return;
    m_movePlan = randInt(4, 32);
    m_vertSpeed += randInt(-2, 2);
}

bool ZombieCab::aheadWorthyInLane(int leftEdge, int rightEdge, int& closest)
{
    closest = 300;
    bool foundOne = false;
    for (int i = 0; i < getWorld() -> listSize(); i++)
    {
        Actor* cur = getWorld() -> getListNum(i);
        if (cur == this) continue;
        if (cur -> isAlive() && cur -> collisionWorthy() && cur -> getX() >= leftEdge && cur -> getX() < rightEdge)
        {
            if (cur -> getY() > getY() && cur -> getY() - getY() < closest)
            {
                closest = cur -> getY() - getY();
                foundOne = true;
            }
        }
    }
    return foundOne;
}

bool ZombieCab::behindWorthyInLane(int leftEdge, int rightEdge, int& closest)
{
    closest = 300;
    bool foundOne = false;
    for (int i = 0; i < getWorld() -> listSize(); i++)
    {
        Actor* cur = getWorld() -> getListNum(i);
        if (cur == this) continue;
        if (cur -> isAlive() && cur -> collisionWorthy() && cur -> getX() >= leftEdge && cur -> getX() < rightEdge)
        {
            if (cur -> getY() < getY() && getY() - cur -> getY() < closest)
            {
                closest = getY() - cur -> getY();
                foundOne = true;
            }
        }
    }
    return foundOne;
}

void ZombieCab::hitWithWater()
{
    m_health--;
    if (m_health <= 0)
    {
        kill();
        getWorld() -> playSound(SOUND_VEHICLE_DIE);
        int rando = randInt(1, 5);
        if (rando == 1)
            getWorld() -> addOilSlick(getX(), getY());
        getWorld() -> increaseScore(200);
        return;
    }
    getWorld() -> playSound(SOUND_VEHICLE_HURT);
}
