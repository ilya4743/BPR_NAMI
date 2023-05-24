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
    std::vector<Eigen::Vector3f> FindPath();
    void Clear() noexcept;

   private:
    /// Конечная точка
    Eigen::Vector3f goal;
    /// Начальная точка
    Eigen::Vector3f start;
    /// Ориентация в начальной точке
    Eigen::Quaternionf rotation_start;
    /// Сетка занятости
    OccupancyGrid grid;
    /// Автомобиль
    std::unique_ptr<Object> car;
    /// Вектор препятствий
    std::vector<std::unique_ptr<Object>> obstacles;

    Placer placer;
    HybridAStar::HybridAstarAlgorithm hybrid_astar;
};