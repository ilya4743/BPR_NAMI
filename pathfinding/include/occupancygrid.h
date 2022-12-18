#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H
#include "stddef.h"
#include "barrier.h"
#include <vector>

#include <boost/geometry/geometries/box.hpp> 
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/algorithms/intersection.hpp> 

using namespace std;
namespace bg = boost::geometry;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point> polygon;

class OccupancyGrid
{
    private:

    public:
        size_t width;
        size_t height;
        float resolution;
        std::vector<int8_t> data;

        OccupancyGrid():width(0), height(0), resolution(0), data(){};

        OccupancyGrid(size_t width,size_t height, float resolution):width(width), height(height), resolution(resolution),data(width*height){};

        OccupancyGrid(float width_coord,float height_coord, float resolution):
        width(width_coord/resolution),height(height_coord/resolution),resolution(resolution),data(this->width*this->height){};

        OccupancyGrid(size_t width,size_t height, float resolution,  std::vector<int8_t> data):
        width(width),height(height),resolution(resolution),data(data){};

        OccupancyGrid(const OccupancyGrid& o):width(o.width), height(o.height), resolution(o.resolution), data(o.data){};

        bool isInside(Ogre::Vector3 p){return p.x<=width*resolution&&p.x>=0&&p.z>=0&&p.z<height*resolution;};
        int getI(Ogre::Vector3 p){return p.x/resolution;};
        int getJ(Ogre::Vector3 p){return p.z/resolution;};
};

class Placer
{
private:
    inline void bresenham(int x1, int y1, int x2, int y2, OccupancyGrid& grid);
public:
    void placeObstacleOnGrid(OccupancyGrid& OccupancyGrid, const BQuadrAngle & obstacle);
};
#endif // OCCUPANCYGRID_H