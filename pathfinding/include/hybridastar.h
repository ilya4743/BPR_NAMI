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

class HybridAstarAlgorithm {
   private:
    DynamicVoronoi* voronoiDiagram;
    CollisionDetection cd;
    float* dubinsLookup;
    Smoother smoother;

   public:
    HybridAstarAlgorithm();
    ~HybridAstarAlgorithm();
    std::vector<HybridAStar::Node3D> SearchHybridAStar(float x0, float y0, float t0, float x1, float y1, float t1, const OccupancyGrid& grid);
};
}  // namespace HybridAStar