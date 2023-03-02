#pragma once

#include "stddef.h"
#include "obstacle.h"
#include <vector>

#include <boost/geometry/geometries/box.hpp> 
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/algorithms/intersection.hpp> 

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
        
        OccupancyGrid(OccupancyGrid&& tmp)=default;
        
        OccupancyGrid& operator=(OccupancyGrid&& tmp)=default;

        bool isInside(Vector4 p){return X(p)<=width*resolution&&X(p)>=0&&Z(p)>=0&&Z(p)<height*resolution;};
        int getI(Vector4 p)const{return X(p)/resolution;};
        int getJ(Vector4 p)const{return Z(p)/resolution;};
        void resize(float width, float height, float resolution)
        {
            int w=static_cast<int>(width/resolution);
            int h=static_cast<int>(height/resolution);
            if (this->width!=w || this->height!=h || this->resolution!=resolution)
            {
                this->width=w;
                this->height=h;
                this->resolution=resolution;
                data.resize(this->width*this->height);
            }
        }
        float GetWidthCoord() const
        {
            return width*resolution;
        }
        float GetHeightCoord() const
        {
            return height*resolution;
        }
};