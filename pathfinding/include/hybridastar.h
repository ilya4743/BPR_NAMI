#pragma once

#include <smoother.h>

#include <vector>

#include "algorithm.h"
#include "dynamicvoronoi.h"
#include "helper.h"
#include "node3d.h"
#include "object.h"
#include "occupancy_grid.h"

namespace HybridAStar {

class HybridAstarAlgo {
   private:
    DynamicVoronoi* voronoiDiagram;
    CollisionDetection cd;
    float* dubinsLookup;
    Smoother smoother;

   public:
    HybridAstarAlgo();
    ~HybridAstarAlgo();
    std::vector<Eigen::Vector3f> searchHybridAStar(float x1, float y1, Eigen::Quaternionf t1, float x2, float y2, float t2, const OccupancyGrid& occurancy);
};
}  // namespace HybridAStar