#ifndef POINT_H
#define POINT_H

#include<fstream>
/// Класс точки
class Point
{

public:
    float x;
    float y;
    /// Конструктор по умолчанию
    Point():x(0),y(0)
    {

    }
    /// Конструктор с параметрами
    Point(float x, float y):x(x),y(y)
    {

    }
    /// Конструктор копирования
    Point (const Point& o):x(o.x),y(o.y)
    {

    }
    friend std::ofstream& operator<<(std::ofstream &out, const Point &p);
};
#endif // POINT_H
