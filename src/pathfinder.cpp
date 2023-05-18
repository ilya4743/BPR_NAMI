#include "pathfinder.h"

#include <Eigen/Geometry>

#include "constants.h"
#include "constants_app.h"
void PathFinder::UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
                            float theta, float speed, size_t n, std::map<uint64_t, Eigen::Matrix4f>&& objects) {
    grid.resize(width, height, resolution);
    auto itCar = objects.find(0);
    objects.erase(itCar);
    Eigen::Vector3f goal_point{x, theta, 1.5708f};
    Eigen::Matrix4f mat4_of_car = (*itCar).second;
    Eigen::Affine3f affine_of_car(mat4_of_car);
    rotation = affine_of_car.rotation();

    Eigen::Vector3f position_car(affine_of_car.translation().transpose());
    affine_of_car.translate(Eigen::Vector3f{-width / 2, 0, -height / 2});

    Eigen::Vector3f pos_local = Eigen::Affine3f(affine_of_car.matrix().inverse() * mat4_of_car).translation().transpose();  // выводим позицию вектора
    car = std::make_unique<Car>(mat4_of_car, speed);
    mat4_of_car = affine_of_car.matrix();
    start = pos_local;
    goal = goal_point + Eigen::Vector3f{
                            width / 2,
                            height / 2, 0};
    obstacles.reserve(n);
    placer.clearGrid(grid);
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        auto obstacle = std::make_unique<BQuadrAngle>(mat4_of_car.inverse() * (*it).second);
        placer.placeObstacleOnGrid(grid, *(obstacle.get()));
        obstacles.push_back(std::move(obstacle));
    }
    if (BPR_NAMI::Constants::GetInstance().IS_PRINT_GRID()) {
        system("clear");
        for (int i = grid.height - 1; i >= 0; i--) {
            for (int j = grid.width - 1; j >= 0; j--) {
                if (grid.data[i * grid.width + j] == 100)
                    std::cout << 1;
                else
                    std::cout << 0;
            }
            std::cout << std::endl;
        }
    }
}

std::vector<Eigen::Vector3f> PathFinder::Find() {
    return hybrid_astar.searchHybridAStar(start(0), start(2), rotation, goal(0), goal(1), goal(2), grid);
}

void PathFinder::Clear() noexcept {
    obstacles.clear();
    car.reset();
}
