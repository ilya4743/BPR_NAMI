#include "point.h"
std::ofstream& operator<<(std::ofstream &out, const Point &p)
{
    out<<p.x<<std::endl<<p.y<<std::endl;
    return out;
}
