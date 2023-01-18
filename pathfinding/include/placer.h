#ifndef PLACER_H
#define PLACER_H
#include <list>
#include "occupancygrid.h"
#include "barrier.h"

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
class OccupancyGrid;
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
            occupancyCell.push_back(grid.width*y1+x1);
        }      
    }
    std::list<int> occupancyCell;
public:
    inline void placeObstacleOnGrid(OccupancyGrid& OccupancyGrid, const BQuadrAngle & obstacle)
    {
        float width=(OccupancyGrid.width-1)*OccupancyGrid.resolution;
        float height=(OccupancyGrid.height-1)*OccupancyGrid.resolution;

        polygon poly{{{obstacle.p1.x, obstacle.p1.z},{obstacle.p2.x, obstacle.p2.z},
                    {obstacle.p3.x, obstacle.p3.z},{obstacle.p4.x, obstacle.p4.z},
                    {obstacle.p1.x, obstacle.p1.z}}};
        box box{{0, 0}, {width, height-OccupancyGrid.resolution}};

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
    }
    inline void clearGrid(OccupancyGrid& OccupancyGrid)
    {
        for(auto it=occupancyCell.cbegin(); it!=occupancyCell.end(); ++it)
            OccupancyGrid.data[*it]=0;
    }
};
#endif
