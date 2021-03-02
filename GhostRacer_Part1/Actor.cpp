#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <math.h>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::outOfBounds() const
{
    //improvements later
    if (getY() < 0 || getY() > VIEW_HEIGHT || getX() < 0 || getX() > VIEW_WIDTH)
        return true;
    return false;
}

void GhostRacer::dirFormula()
{
    double dir = getDirection();
    double delta_x = cos(dir * (M_PI/ 180)) * 4.0;
    double cur_x = getX();
    double cur_y = getY();
    std::cout << dir << std::endl;
    moveTo(cur_x + delta_x, cur_y);
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
