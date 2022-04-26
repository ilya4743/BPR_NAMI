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
