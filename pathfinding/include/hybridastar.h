#pragma once

#include "node3d.h"
#include "algorithm.h"
#include "helper.h"
#include <smoother.h>
#include <vector>
#include "dynamicvoronoi.h"
#include "occupancy_grid.h"
#include "object.h"

namespace HybridAStar {

class HybridAstarAlgo
{
    private:
        DynamicVoronoi* voronoiDiagram;
        CollisionDetection cd;
        float* dubinsLookup;
        Smoother smoother;
    public:
    HybridAstarAlgo();
    ~HybridAstarAlgo();
    std::vector<Vector3> searchHybridAStar(float x1, float y1, Quaternion t1, float x2, float y2, float t2, const OccupancyGrid& occurancy);
};
}