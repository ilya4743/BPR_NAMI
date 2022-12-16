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
    inline void bresenham(int x1, int y1, int x2, int y2, OccupancyGrid& grid)
    {
        const int deltaX = abs(x2 - x1);
        const int deltaY = abs(y2 - y1);
        const int signX = x1 < x2 ? 1 : -1;
        const int signY = y1 < y2 ? 1 : -1;
        int error = deltaX - deltaY;
        while(x1 != x2 || y1 != y2)
        {
            int error2 = error * 2;
            if(error2 > -deltaY)
            {
                error -= deltaY;
                x1 += signX;
            }
            if(error2 < deltaX)
            {
                error += deltaX;
                y1 += signY;
            }
            grid.data[grid.width*y1+x1]=100;
        }
    }
    public:
    void placeObstacleOnGrid(OccupancyGrid& OccupancyGrid, const BQuadrAngle & obstacle)
    {
        float width=(OccupancyGrid.width-1)*OccupancyGrid.resolution;
        float height=(OccupancyGrid.height-1)*OccupancyGrid.resolution;

        polygon poly{{{obstacle.p1.x, obstacle.p1.z},{obstacle.p2.x, obstacle.p2.z},
                       {obstacle.p3.x, obstacle.p3.z},{obstacle.p4.x, obstacle.p4.z},
                       {obstacle.p1.x, obstacle.p1.z}}};
        box box{{0, 0}, {width, height}};

        if(boost::geometry::intersects(box,poly))
        {
            vector <polygon>  output ;
            boost::geometry::intersection(box, poly, output);
            vector<Ogre::Vector3> out;
            
            if(output.size()>0)
            {
                out.reserve(output[0].outer().size());
                for(int i=0; i<output[0].outer().size();i++)
                    out.push_back(Ogre::Vector3(bg::get<0>(output[0].outer()[i]),0,bg::get<1>(output[0].outer()[i])));
                for(int i=1; i<out.size();i++)
                {
                    int x1=OccupancyGrid.getI(out[i-1]);
                    int y1=OccupancyGrid.getJ(out[i-1]);
                    
                    int x2=OccupancyGrid.getI(out[i]);
                    int y2=OccupancyGrid.getJ(out[i]);
                    bresenham(x1, y1, x2, y2,OccupancyGrid);    
                }
            }
            else
            {
                out.reserve(4);
                out.push_back(obstacle.p1);
                out.push_back(obstacle.p2);
                out.push_back(obstacle.p3);
                out.push_back(obstacle.p4);
                for(int i=1; i<out.size();i++)
                {
                    int x1=OccupancyGrid.getI(out[i-1]);
                    int y1=OccupancyGrid.getJ(out[i-1]);
                    
                    int x2=OccupancyGrid.getI(out[i]);
                    int y2=OccupancyGrid.getJ(out[i]);
                    bresenham(x1, y1, x2, y2,OccupancyGrid);    
                }
            }
        }
    };
};
#endif // OCCUPANCYGRID_H