#include "pathfinder.h"

#include <Eigen/Geometry>

#include "constants.h"
#include "constants_app.h"
void PathFinder::UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
                            float theta, float speed, size_t n, std::map<uint64_t, Eigen::Matrix4f>&& objects) {
    // пересоздаем сетку занятости, если её размер изменился
    grid.resize(width, height, resolution);

    // находим автомобиль в хеш-таблице объектов и удаляем его оттуда
    auto itCar = objects.find(0);
    objects.erase(itCar);

    // создаем промежуточные переменные
    Eigen::Vector3f goal_point{x, theta, 1.5708f};
    Eigen::Matrix4f mat4_of_car = (*itCar).second;
    Eigen::Affine3f affine_of_car(mat4_of_car);
    rotation_start = affine_of_car.rotation();

    // смещаем влево и вниз автомобиль, чтобы точкой отсчета была левый нижний угол сетки занятости
    Eigen::Vector3f position_car(affine_of_car.translation().transpose());
    affine_of_car.translate(Eigen::Vector3f{-width / 2, 0, -height / 2});

    Eigen::Vector3f pos_local = Eigen::Affine3f(affine_of_car.matrix().inverse() * mat4_of_car).translation().transpose();
    car = std::make_unique<Car>(mat4_of_car, speed);
    mat4_of_car = affine_of_car.matrix();
    start = pos_local;
    goal = goal_point + Eigen::Vector3f{width / 2, height / 2, 0};

    // создаем препятствия из единичного куба
    obstacles.reserve(n);
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        auto obstacle = std::make_unique<BQuadrAngle>(mat4_of_car.inverse() * (*it).second);
        placer.PlaceObstacleOnGrid(grid, *(obstacle.get()));
        obstacles.push_back(std::move(obstacle));
    }

    // печатаем сетку занятости
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

std::vector<Eigen::Vector3f> PathFinder::FindPath() {
    auto path = hybrid_astar.SearchHybridAStar(start(0), start(2), 1.57, goal(0), goal(1), 1.57, grid);

    // смещаем найденный путь, где машина будет точкой отсчета
    std::vector<Eigen::Vector3f> out;
    out.reserve(path.size());
    Eigen::Vector3f translate_vec{-grid.GetWidthCoord() / 2, 0, -grid.GetHeightCoord() / 2};

    for (int i = 0; i < path.size(); i++) {
        Eigen::Vector3f pos{path[i].getX() * grid.resolution, path[i].getT(), path[i].getY() * grid.resolution};
        out.push_back(rotation_start * (pos + translate_vec));
    }
    return out;
}

void PathFinder::Clear() noexcept {
    obstacles.clear();
    obstacles.shrink_to_fit();
    placer.clearGrid(grid);
    car.reset();
}
