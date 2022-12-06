#include "gamemap.h"

GameMap::GameMap()
{
}

GameMap::GameMap(float width_coord, float height_coord, float step,
                 const Car &car,
                 const Ogre::Vector3 &goal_p):
    width_coord(width_coord),height_coord(height_coord),step(step),
    goal_point(goal_p)
{
}

GameMap::~GameMap()
{
    //for(auto it= barriers.begin(); it!=barriers.end(); ++it)
        //delete *it;
    this->barriers.clear();
}