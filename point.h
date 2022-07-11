#ifndef POINT_H
#define POINT_H

#include<iostream>
#include <fstream>
#include <QDataStream>

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
    friend QDataStream& operator <<(QDataStream &out, const Point &b);
    friend QDataStream& operator >>(QDataStream &in, Point &b);
};

class Position
{
public:
    float x;
    float y;
    float z;
    float w;
    Position():x(0),y(0),z(0),w(0){}
    Position(float x, float y, float z, float w):x(x),y(y),z(z),w(w){}
    Position(const Position& position):x(position.x),y(position.y),z(position.z),w(position.w){}
    friend ofstream& operator<<(ofstream &out, const Position &position);
    friend ostream& operator <<(ostream &out, const Position &position);
    friend istream& operator >>(istream &in, Position &position);
    friend QDataStream& operator <<(QDataStream &out, const Position &b);
    friend QDataStream& operator >>(QDataStream &in, Position &b);
};

class Scale
{
public:
    float x;
    float y;
    float z;
    Scale():x(0),y(0),z(0){}
    Scale(float x, float y, float z):x(x),y(y),z(z){}
    Scale(const Scale& scale):x(scale.x),y(scale.y),z(scale.z){}
    friend ofstream& operator<<(ofstream &out, const Scale &scale);
    friend ostream& operator <<(ostream &out, const Scale &scale);
    friend istream& operator >>(istream &in, Scale &scale);
    friend QDataStream& operator <<(QDataStream &out, const Scale &b);
    friend QDataStream& operator >>(QDataStream &in, Scale &b);
};


#endif // POINT_H
