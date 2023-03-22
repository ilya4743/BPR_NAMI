#pragma once

#include <memory>

#include "car.h"
#include "hybridastar.h"
#include "obstacle.h"
#include "occupancy_grid.h"
#include "placer.h"

class PathFinder {
   public:
    PathFinder() = default;
    PathFinder(const PathFinder&) = delete;
    PathFinder& operator=(const PathFinder&) = delete;
    void UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
                    float theta, float speed, size_t n, std::map<uint64_t, Eigen::Matrix4f>&& objects);
    std::vector<Eigen::Vector3f> Find();
    void Clear() noexcept;

   private:
    /// Точка конечного маршрута
    Eigen::Vector3f goal;

    Eigen::Vector3f start;

    Eigen::Quaternionf rotation;
    /// Матрица занятости
    OccupancyGrid grid;
    /// Автомобиль
    std::unique_ptr<Object> car;
    /// Список препятствий
    std::vector<std::unique_ptr<Object>> obstacles;

    Placer placer;
    HybridAStar::HybridAstarAlgo hybrid_astar;
};