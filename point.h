#ifndef POINT_H
#define POINT_H


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
};
#endif // POINT_H
