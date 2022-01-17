#ifndef POINT_H
#define POINT_H

#include<fstream>

class Point
{

public:
    float x;
    float y;
    Point():x(0),y(0)
    {

    }
    Point(float x, float y):x(x),y(y)
    {

    }
    Point (const Point& o):x(o.x),y(o.y)
    {

    }
    friend std::ofstream& operator<<(std::ofstream &out, const Point &p);
};
#endif // POINT_H
