#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
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

void StudentWorld::addOilSlick(int x, int y)
{
    actorList.push_back(new OilSlick(this, x, y));
}

void StudentWorld::checkBorderLines()
{
    m_lastY += (-4 - m_ghosty -> getSpeed());
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
}

Actor* StudentWorld::getListNum(int index)
{
    if (index < 0 || index >= actorList.size())
        return nullptr;
    return actorList[index];
}

void StudentWorld::takeOutTrash()
{
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
}

void StudentWorld::tryOilSlicks()
{
    int chanceOil = max(150 - (getLevel() * 10), 40);
    int rando = randInt(0, chanceOil);
    if (rando == 0)
        addOilSlick(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
}

void StudentWorld::trySouls()
{
    int chance = randInt(0, 100);
    if (chance == 0)
        addSoulGoodie(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
}

void StudentWorld::tryHumanPeds()
{
    int chanceOfPed = max(200 - getLevel()* 10, 30);
    int rando = randInt(0, chanceOfPed);
    if (rando == 0)
        addHumanPed(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
}

void StudentWorld::tryWaterGoodies()
{
    int chance = 100 + 10 * getLevel();
    int rando = randInt(0, chance);
    if (rando == 0)
        addWaterGoodies(randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
}

void StudentWorld::tryZombiePeds()
{
    int chance = max(100 - getLevel() * 10, 20);
    int rando = randInt(0, chance);
    if (rando == 0)
        addZombiePed(randInt(0, VIEW_WIDTH), VIEW_HEIGHT);
}

void StudentWorld::tryZombieCabs()
{
    int chance = max(100 - getLevel() * 10, 20);
    int rando = randInt(0, chance);
    if (rando != 0) return;
    int y_axis = 12;
    int rightEdge;
    int leftEdge;
    double speed = 0;
    int index = 0;
    while (index < 3)
    {
        if (index == 0)
        {
            leftEdge = LEFT_EDGE;
            rightEdge = LEFT_EDGE + ROAD_WIDTH / 3;
        }
        if (index == 1)
        {
            leftEdge = LEFT_EDGE + ROAD_WIDTH / 3;
            rightEdge = RIGHT_EDGE - ROAD_WIDTH / 3;
        }
        else if (index == 2)
        {
            leftEdge = RIGHT_EDGE - ROAD_WIDTH / 3;
            rightEdge = RIGHT_EDGE;
        }
        int closest = VIEW_HEIGHT + 1;
        for (int j = 0; j < listSize(); j++)
        {
            Actor* cur = getListNum(j);
            if (cur -> collisionWorthy() && cur -> getX() >= leftEdge && cur -> getX() < rightEdge)
            {
                if (cur -> getY() < closest)
                    closest = cur -> getY();
            }
        }
        if (closest > VIEW_HEIGHT / 3)
        {
            speed = getGhosty() -> getSpeed() + randInt(2, 4);
            y_axis = SPRITE_HEIGHT / 2;
            break;
        }
        int farthest = -1;
        for (int j = 0; j < listSize(); j++)
        {
            Actor* cur = getListNum(j);
            if (cur -> collisionWorthy() && cur -> getX() >= leftEdge && cur -> getX() < rightEdge)
            {
                if (cur -> getY() > farthest)
                    farthest = cur -> getY();
            }
        }
        if (farthest < (VIEW_HEIGHT * 2 / 3))
        {
            speed = getGhosty() -> getSpeed() - randInt(2, 4);
            y_axis =  VIEW_HEIGHT - SPRITE_HEIGHT / 2;
            break;
        }
        index++;
    }
    if (index == 3) return;
    if (index == 0)
    {
        addZombieCab(ROAD_CENTER -
                      ROAD_WIDTH/3 , y_axis, leftEdge, rightEdge, speed);
        return;
    }
    if (index == 1)
    {
        addZombieCab(ROAD_CENTER, y_axis, leftEdge, rightEdge, speed);
        return;
    }
    if (index == 2)
    {
        addZombieCab(ROAD_CENTER +
                     ROAD_WIDTH/3 , y_axis, leftEdge, rightEdge, speed);
        return;
    }
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    N = VIEW_HEIGHT / SPRITE_HEIGHT;
    M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    m_lastY = VIEW_HEIGHT - SPRITE_HEIGHT;
    m_bonus = 5000;
    souls2save = 2 * getLevel() + 5;
    soulsLeft = souls2save;
    soulsSaved = 0;
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
    m_lastY = (M - 1) * (4 * SPRITE_HEIGHT);
    m_bonus = 5000;
    souls2save = 2 * getLevel() + 5;
    soulsLeft = souls2save;
    soulsSaved = 0;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    for (vector<Actor*>::iterator it = actorList.begin();it != actorList.end(); it++)
        if ((*it) -> isAlive())
        {
            (*it) -> doSomething();
            if (!m_ghosty -> isAlive())
            {
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (m_ghosty -> isKiller())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (souls2save == soulsSaved)
            {
                increaseScore(m_bonus);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    takeOutTrash();
    checkBorderLines();
    tryOilSlicks();
    trySouls();
    tryWaterGoodies();
    tryHumanPeds();
    tryZombiePeds();
    tryZombieCabs();
    if (m_bonus <= 0)
        m_bonus = 0;
    else m_bonus--;
    
    ostringstream text;
    text << "Score: " << getScore() << " Lvl: " << getLevel() << " Souls2Save: " << soulsLeft << " Lives: " << getLives() << " Health: " << m_ghosty -> getHealth() << " Sprays: " << m_ghosty -> getWater() << " Bonus: " << m_bonus;
    string outPut = text.str();
    setGameStatText(outPut);
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

bool StudentWorld::overLaps(Actor *a1, Actor *a2)
{
    double delta_x = abs(a1 -> getX() - a2 -> getX());
    double delta_y = abs(a1 -> getY() - a2 -> getY());
    double radiusSum = a1 -> getRadius() + a2 -> getRadius();
    if (delta_x < radiusSum * .25 && delta_y < radiusSum * .6)
        return true;
    return false;
}

void StudentWorld::addHolyWaterProj(int x, int y)
{
    actorList.push_back(new HolyWaterProj(this, x, y, m_ghosty -> getDirection()));
}

void StudentWorld::addSoulGoodie(int x, int y)
{
    actorList.push_back(new SoulGoodie(this, x, y));
}

void StudentWorld::addWaterGoodies(int x, int y)
{
    actorList.push_back(new WaterGoodie(this, x, y));
}

void StudentWorld::addHealGoodie(int x, int y)
{
    actorList.push_back(new HealGoodie(this, x, y));
}

void StudentWorld::addHumanPed(int x, int y)
{
    actorList.push_back(new HumanPed(this, x, y));
}

void StudentWorld::addZombiePed(int x, int y)
{
    actorList.push_back(new ZombiePed(this, x, y));
}

void StudentWorld::addZombieCab(int x, int y, int leftEdge, int rightEdge, double speed)
{
    actorList.push_back(new ZombieCab(this, x, y, leftEdge, rightEdge, speed));
}

