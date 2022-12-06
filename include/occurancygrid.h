#ifndef OCCURANCYGRID_H
#define OCCURANCYGRID_H
#include "stddef.h"
#include <vector>
#include "barrier.h"

class OccurancyGrid
{
    private:

    public:
        size_t width;
        size_t height;
        float resolution;
        std::vector<unsigned char> data;

        OccurancyGrid():width(0), height(0), resolution(0), data(){};

        OccurancyGrid(size_t width,size_t height, float resolution):width(width), height(height), resolution(resolution),data(width*height){};

        OccurancyGrid(float width_coord,float height_coord, float resolution):
        width(width_coord/resolution),height(height_coord/resolution),resolution(resolution),data(this->width*this->height){};

        OccurancyGrid(size_t width,size_t height, float resolution,  std::vector<unsigned char> data):
        width(width),height(height),resolution(resolution),data(data){};

        OccurancyGrid(const OccurancyGrid& o):width(o.width), height(o.height), resolution(o.resolution), data(o.data){};

        bool isInside(Ogre::Vector3 p){return p.x<=width*resolution&&p.x>=0&&p.z>=0&&p.z<height*resolution;};
        int getI(Ogre::Vector3 p){return p.x/resolution;};
        int getJ(Ogre::Vector3 p){return p.z/resolution;};
};

class Placer
{
    private:
    inline void bresenham(int x1, int y1, int x2, int y2, OccurancyGrid& grid)
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
            grid.data[grid.width*y1+x1]=255;
        }
    }
    public:
    void placeObstacleOnGrid(OccurancyGrid& occurancyGrid, const BQuadrAngle & obstacle)
    {
        float width=(occurancyGrid.width-1)*occurancyGrid.resolution;
        float height=(occurancyGrid.height-1)*occurancyGrid.resolution;

        polygon poly{{{obstacle.p1.x, obstacle.p1.z},{obstacle.p2.x, obstacle.p2.z},
                       {obstacle.p3.x, obstacle.p3.z},{obstacle.p4.x, obstacle.p4.z},
                       {obstacle.p1.x, obstacle.p1.z}}};
        box box{{0, 0}, {width, height}};

        if(boost::geometry::intersects(box,poly))
        {
            vector < polygon >  output ;
            boost::geometry::intersection(box, poly, output);
            vector<Ogre::Vector3> out;
            
            if(output.size()>0)
            {
                out.reserve(output[0].outer().size());
                for(int i=0; i<output[0].outer().size();i++)
                    out.push_back(Ogre::Vector3(bg::get<0>(output[0].outer()[i]),0,bg::get<1>(output[0].outer()[i])));
                for(int i=1; i<out.size();i++)
                {
                    int x1=occurancyGrid.getI(out[i-1]);
                    int y1=occurancyGrid.getJ(out[i-1]);
                    
                    int x2=occurancyGrid.getI(out[i]);
                    int y2=occurancyGrid.getJ(out[i]);
                    bresenham(x1, y1, x2, y2,occurancyGrid);    
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
                    int x1=occurancyGrid.getI(out[i-1]);
                    int y1=occurancyGrid.getJ(out[i-1]);
                    
                    int x2=occurancyGrid.getI(out[i]);
                    int y2=occurancyGrid.getJ(out[i]);
                    bresenham(x1, y1, x2, y2,occurancyGrid);    
                }
            }
        }
    };
};
#endif // OCCURANCYGRID_H