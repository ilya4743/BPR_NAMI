#include "hybridastar.h"

namespace HybridAStar {

HybridAstarAlgorithm::HybridAstarAlgorithm() {
    dubinsLookup = new float[Constants::GetInstance().HEADINGS() * Constants::GetInstance().HEADINGS() * Constants::GetInstance().DUBINS_WIDTH() * Constants::GetInstance().DUBINS_WIDTH()];
    voronoiDiagram = new DynamicVoronoi;
}

HybridAstarAlgorithm::~HybridAstarAlgorithm() {
    delete voronoiDiagram;
    delete[] dubinsLookup;
}

std::vector<HybridAStar::Node3D> HybridAstarAlgorithm::SearchHybridAStar(float x0, float y0, float t0, float x1, float y1, float t1, const OccupancyGrid& grid) {
    smoother.ClearPath();

    int width = grid.width;
    int height = grid.height;
    int depth = Constants::GetInstance().HEADINGS();
    int length = width * height * depth;
    Node3D* nodes3D = new Node3D[length]();
    Node2D* nodes2D = new Node2D[width * height]();
    // ________________________
    // retrieving goal position
    x0 = x0 / grid.resolution;
    y0 = y0 / grid.resolution;
    t1 = Helper::normalizeHeadingRad(t0);
    Node3D nStart(x0, y0, t0, 0, 0, nullptr);

    x1 = x1 / grid.resolution;
    y1 = y1 / grid.resolution - grid.resolution;
    t1 = Helper::normalizeHeadingRad(t1);
    const Node3D nGoal(x1, y1, t1, 0, 0, nullptr);

    cd.updateGrid(grid);

    Node3D* nSolution = Algorithm::hybridAStar(nStart, nGoal, nodes3D, nodes2D, grid.width, grid.height, cd, dubinsLookup);

    // voronoiDiagram->initializeMap(grid.width, grid.height, grid.data);
    // voronoiDiagram->update();
    // voronoiDiagram->visualize();

    // TRACE THE PATH
    smoother.tracePath(nSolution);
    // smoother.smoothPath(voronoiDiagram);
    // delete voronoiDiagram;
    delete[] nodes3D;
    delete[] nodes2D;
    return smoother.getPath();
}

}  // namespace HybridAStar