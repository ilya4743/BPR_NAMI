#pragma once

#include "obstacle.h"
#include "car.h"
#include "occupancy_grid.h"
#include "placer.h"
#include <memory>
#include "hybridastar.h"

class PathFinder
{
public:
    PathFinder()=default;
    PathFinder(const PathFinder&) = delete;
    PathFinder& operator=(const PathFinder&) = delete;
    void UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
    float theta, float speed, size_t n, std::map<uint64_t,Ogre::Matrix4>&& objects);
    std::vector<Ogre::Vector3> Find();
    void Clear() noexcept;
private:
    /// Точка конечного маршрута
    Ogre::Vector3 goal;
    /// Матрица занятости
    OccupancyGrid grid;
    /// Автомобиль
    std::unique_ptr<Object> car;
    /// Список препятствий
    vector<std::unique_ptr<Object>> obstacles;

    Placer placer;
    HybridAstarAlgo hybrid_astar;
};