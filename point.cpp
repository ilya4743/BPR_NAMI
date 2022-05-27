#include "point.h"

ofstream& operator<<(ofstream &out, const Point &point)
{
    out<<point.x<<'\t'<<point.y<<endl;
    return out;
}

ostream& operator <<(ostream &out, const Point &point)
{
    out<<point.x<<endl<<point.y;
    return out;
}

istream& operator >>(istream &in, Point &point)
{
    in>>point.x>>point.y;
    return in;
}

QDataStream& operator <<(QDataStream &out, const Point &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out<<b.x;
    out<<b.y;
    return out;
}

QDataStream& operator >>(QDataStream &in, Point &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in>>b.x;
    in>>b.y;
    return in;
}

ofstream& operator<<(ofstream &out, const Position &position)
{
    out<<position.x<<'\t'<<position.y<<'\t'<<position.z<<'\t'<<position.w<<endl;
    return out;
}

ostream& operator <<(ostream &out, const Position &position)
{
    out<<position.x<<'\t'<<position.y<<'\t'<<position.z<<'\t'<<position.w<<endl;
    return out;
}

istream& operator >>(istream &in, Position &position)
{
    in>>position.x>>position.y>>position.z;
    return in;
}

QDataStream& operator <<(QDataStream &out, const Position &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out<<b.x<<b.y<<b.z<<b.w;
    return out;
}

QDataStream& operator >>(QDataStream &in, Position &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in>>b.x>>b.y>>b.z>>b.w;
    return in;
}

ofstream& operator<<(ofstream &out, const Scale &scale)
{
    out<<scale.x<<'\t'<<scale.y<<'\t'<<scale.z<<endl;
    return out;
}

ostream& operator <<(ostream &out, const Scale &scale)
{
    out<<scale.x<<'\t'<<scale.y<<'\t'<<scale.z<<endl;
    return out;
}

istream& operator >>(istream &in, Scale &scale)
{
    in>>scale.x>>scale.y>>scale.z;
    return in;
}

QDataStream& operator <<(QDataStream &out, const Scale &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out<<b.x<<b.y<<b.z;
    return out;
}

QDataStream& operator >>(QDataStream &in, Scale &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in>>b.x>>b.y>>b.z;
    return in;
}
