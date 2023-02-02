#pragma once

#include "obstacle.h"
#include "car.h"
#include "occupancy_grid.h"
using namespace std;

class PathFinder
{
public:
    PathFinder() =delete;
    PathFinder(const PathFinder&) = delete;
    PathFinder& operator=(const PathFinder&) = delete;

    /// Точка конечного маршрута
    Ogre::Vector3 goal_point;

    /// Автомобиль
    Car car;

    /// Список препятствий
    vector<BQuadrAngle> barriers;
};