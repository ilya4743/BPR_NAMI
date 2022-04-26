#ifndef POINT_H
#define POINT_H

#include<iostream>
#include <fstream>

using namespace std;

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
    friend ofstream& operator<<(ofstream &out, const Point &point);
    friend ostream& operator <<(ostream &out, const Point &point);
    friend istream& operator >>(istream &in, Point &point);
};
#endif // POINT_H
