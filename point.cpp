#include "point.h"
std::ofstream& operator<<(std::ofstream &out, const Point &p)
{
    out<<p.x<<'\t'<<p.y<<std::endl;
    return out;
}
