#include "hybridastar.h"

namespace HybridAStar {

HybridAstarAlgo::HybridAstarAlgo() {
    dubinsLookup = new float[Constants::GetInstance().HEADINGS() * Constants::GetInstance().HEADINGS() * Constants::GetInstance().DUBINS_WIDTH() * Constants::GetInstance().DUBINS_WIDTH()];
    voronoiDiagram = new DynamicVoronoi;
}

HybridAstarAlgo::~HybridAstarAlgo() {
    delete voronoiDiagram;
    delete[] dubinsLookup;
}

std::vector<Eigen::Vector3f> HybridAstarAlgo::searchHybridAStar(float x1, float y1, Eigen::Quaternionf t1, float x2, float y2, float t2, const OccupancyGrid& grid) {
    int width = grid.width;
    int height = grid.height;
    int depth = Constants::GetInstance().HEADINGS();
    int length = width * height * depth;
    Node3D* nodes3D = new Node3D[length]();
    Node2D* nodes2D = new Node2D[width * height]();
    // ________________________
    // retrieving goal position
    x1 = x1 / grid.resolution;
    y1 = y1 / grid.resolution;
    // t1 = Helper::normalizeHeadingRad(1.57);
    Node3D nStart(x1, y1, 1.57, 0, 0, nullptr);

    x2 = x2 / grid.resolution;
    y2 = y2 / grid.resolution;
    t2 = Helper::normalizeHeadingRad(t2);
    const Node3D nGoal(x2, y2, t2, 0, 0, nullptr);

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
    std::vector<Eigen::Vector3f> out;
    auto p = smoother.getPath();
    out.reserve(p.size());
    for (int i = 0; i < smoother.getPath().size(); i++) {
        Eigen::Vector3f ve{p[i].getX() * grid.resolution - grid.GetWidthCoord() / 2, p[i].getT(), p[i].getY() * grid.resolution - grid.GetHeightCoord() / 2};
        out.push_back(t1 * ve);
    }
    smoother.ClearPath();
    return out;
}

}  // namespace HybridAStar