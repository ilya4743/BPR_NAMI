#include "barrier.h"

IBarrier::~IBarrier(){}

Barrier::Barrier():affine3(),position(), scale(), rotation(){}
Barrier::Barrier(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23):affine3(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23)
{
    //affine3.decomposition(position, scale, rotation);
}

Barrier::Barrier(const Barrier&o):affine3(o.affine3), position(o.position),scale(o.scale), rotation(o.rotation){}

int Barrier::init(MyGraph& graph, DMQuadrangle& distance){    affine3.decomposition(position, scale, rotation);}
Barrier::~Barrier(){}
void Barrier::print(IDistanceMatrixAdapter &adapter){}
bool Barrier::hasPoint(Ogre::Vector3 p, GameMap&g){}
bool Barrier::hasVertex(int v, GameMap&g){}
bool Barrier::isIntersection(Ogre::Vector3 a, Ogre::Vector3 b){}
void Barrier::printToFile(ofstream &out){}

ofstream& operator<<(ofstream &out, const Barrier &barrier)
{
    out<<barrier.position<<endl;
    return out;
}

BQuadrAngle::BQuadrAngle():Barrier(),left_top(),right_bottom()
{

}

BQuadrAngle::BQuadrAngle(float m00, float m01, float m02, float m03,
                         float m10, float m11, float m12, float m13,
                         float m20, float m21, float m22, float m23):Barrier(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23),
    left_top(position.x-scale.x/2.0,position.y+scale.y/2.0, position.z), right_bottom(position.x+scale.x/2.0,position.y-scale.y/2.0, position.z)
{

}

BQuadrAngle::BQuadrAngle(const BQuadrAngle& o):Barrier(o),left_top(o.left_top),right_bottom(o.right_bottom)
{

}

void BQuadrAngle::print(IDistanceMatrixAdapter &adapter)
{
    cout<<"\033[s";
    for (int i = adapter.GetI(left_top.y); i <= adapter.GetI(right_bottom.y); i++)
        for (int j = adapter.GetJ(left_top.x); j <= adapter.GetJ(right_bottom.x); j++)
            cout<<"\033["<<i+1<<';'<<j+1<<"H\033[0;37;47m \033[0;0m";
    cout<<"\033[u";
}

int BQuadrAngle::init(MyGraph& graph, DMQuadrangle& distance)
{
    IDistanceMatrixAdapter* g=new DistanceMatrixAdapter(dynamic_cast<DMQuadrangle*>(&distance));
    //если препятсвие никак не влезет на карту, то вернуть 0 иначе попытаться впихнуть хоть как-нибудь
    if(distance.matrix[0].x > right_bottom.x || distance.matrix[distance.matrix.size()-1].x < left_top.x ||
       distance.matrix[0].y < right_bottom.y || distance.matrix[distance.matrix.size()-1].y > left_top.y)
        return -2;
    else
    {
        if (distance.matrix[0].x > left_top.x)
            left_top.x=distance.matrix[0].x;

        if (distance.matrix[0].y < left_top.y)
            left_top.y=distance.matrix[0].y;

        if (distance.matrix[distance.matrix.size()-1].x < right_bottom.x)
            right_bottom.x=distance.matrix[distance.matrix.size()-1].x;

        if (distance.matrix[distance.matrix.size()-1].y > right_bottom.y)
            right_bottom.y=distance.matrix[distance.matrix.size()-1].y;

        //соседние по горизонтали
        for (int i = g->GetI(left_top.y); i <= g->GetI(right_bottom.y); i++)
        {
            for (int j = g->GetJ(left_top.x); j <= g->GetJ(right_bottom.x) + 1; j++)
            {
                if (j == 0 || j == distance.width)continue;

                int u = i * distance.width + j - 1;
                int v = i * distance.width + j;
                remove_edge(u, v, *graph.adj_list);
            }
        }

        //соседние по вертикале
        for (int i = g->GetI(left_top.y); i <= g->GetI(right_bottom.y) + 1; i++)
        {
            if (i == 0 || i == distance.height)continue;

            for (int j = g->GetJ(left_top.x); j <= g->GetJ(right_bottom.x); j++)
            {
                int u2 = (i - 1) * distance.width + j;
                int v2 = (i)*distance.width + j;
                remove_edge(u2, v2, *graph.adj_list);
            }
        }

        //главная диагональ
        for (int i = g->GetI(left_top.y); i <= g->GetI(right_bottom.y) + 1; i++)
        {
            if (i == 0 || i == distance.height)continue;
            for (int j = g->GetJ(left_top.x); j <= g->GetJ(right_bottom.x) + 1; j++)
            {
                if (j == 0 || j == distance.width)continue;
                int u1 = (i - 1) * distance.width + j - 1;
                int v1 = (i)*distance.width + j;
                remove_edge(u1, v1, *graph.adj_list);
            }
        }

        //побочная диагональ
        for (int i = g->GetI(right_bottom.y) + 1; i >= g->GetI(left_top.y); i--)
        {
            if (i == distance.height || i==0)continue;
            for (int j = g->GetJ(right_bottom.x) + 1; j >= g->GetJ(left_top.x); j--)
            {
                if (j == distance.width || j == 0)continue;
                int u1 = (i)*distance.width + j - 1;
                int v1 = (i - 1) * distance.width + j;
                remove_edge(u1, v1, *graph.adj_list);
            }
        }
        return 0;
    }
}

BQuadrAngle::~BQuadrAngle()
{
    //cout<<"Препятствие удалено!";
}

bool BQuadrAngle::hasPoint(Ogre::Vector3 p, GameMap&g)
{
    //искуственно увеличиваем препятствие, чтобы соблюсти габариты
    left_top.x=left_top.x-g.car.scale.x/2;
    left_top.y=left_top.y+g.car.scale.y/2;
    right_bottom.x=right_bottom.x+g.car.scale.x/2;
    right_bottom.y=right_bottom.y-g.car.scale.y/2;

    if (abs(left_top.x) > abs(int(left_top.x / g.step) * g.step))
        left_top.x -= abs(left_top.x - int(left_top.x / g.step) * g.step);

    if (abs(right_bottom.x) > abs(int(right_bottom.x / g.step) * g.step))
        right_bottom.x +=abs(right_bottom.x - int(right_bottom.x / g.step) * g.step);

    if (abs(left_top.y) > abs(int(left_top.y / g.step) * g.step))
        left_top.y +=abs(left_top.y - int(left_top.y / g.step) * g.step);

    if (abs(right_bottom.y) > abs(int(right_bottom.y / g.step) * g.step))
        right_bottom.y -= abs(right_bottom.y - int(right_bottom.y / g.step) * g.step);

    if (left_top.x<=p.x && right_bottom.x>=p.x)
        if(left_top.y>=p.y && right_bottom.y<=p.y)
            return true;
    return false;
}

bool BQuadrAngle::hasVertex(int v, GameMap&g)
{
    int i1=g.adapter->GetI(left_top.y);
    int i2=g.adapter->GetI(right_bottom.y);
    int j1=g.adapter->GetJ(left_top.x);
    int j2=g.adapter->GetJ(right_bottom.x);
    auto d=dynamic_cast<DMQuadrangle*>(g.distance);
    if(i1<=v/d->width && i2>=v/d->width)
        if(j1<=v%d->width && j2>=v%d->width)
            return true;
    return false;
}

void BQuadrAngle::printToFile(ofstream &out)
{
    out<<position;
    out<<scale;
    out<<left_top<<right_bottom;
}

ofstream& operator<<(ofstream &out, const BQuadrAngle &barrier)
{
    out<<barrier.position;
    out<<barrier.scale;
    out<<barrier.left_top<<barrier.right_bottom;
    return out;
}

inline int area (Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  swap (a, b);
    if (c > d)  swap (c, d);
    return max(a,c) <= min(b,d);
}

/// Класс сглаживания пути
class Smoother
{
private:
    static bool inside(Ogre::Vector3 p, vector<float> plane)
    {
        float d = p.x * plane[0] + p.y * plane[1];
        return d > plane[2];
    }

    static Ogre::Vector3 clip(Ogre::Vector3 segStart, Ogre::Vector3 segEnd, vector<float> plane)
    {
        float d1 = segStart.x * plane[0] + segStart.y * plane[1] - plane[2];
        float d2 = segEnd.x * plane[0] + segEnd.y * plane[1] - plane[2];
        float t = (0 - d1) / (d2 - d1);
        return Ogre::Vector3(segStart.x + t * (segEnd.x - segStart.x), segStart.y + t * (segEnd.y - segStart.y),0);
    }
public:
    static list<Ogre::Vector3> Polyclip(list<Ogre::Vector3> pin, Ogre::Vector3 segStart, Ogre::Vector3 segEnd)
    {
        //if (pin == NULL || pin.Count < 3)
        //    throw new ArgumentException("A polygon was not supplied.");
        vector<float> plane;
        plane.push_back(segStart.y-segStart.y );
        plane.push_back(segEnd.x - segStart.x);
        plane.push_back(0);
        plane[2] = segStart.x * plane[0] + segStart.y * plane[1];
        list<Ogre::Vector3> pout;
        Ogre::Vector3 s = *--pin.end();
        for (auto ci = pin.begin(); ci != pin.end(); ci++)
        {
            Ogre::Vector3 p = *(ci);
            if (inside(p, plane))
            {
                if (inside(s, plane))
                {
                    pout.push_back(p);
                }
                else
                {
                    Ogre::Vector3 t = clip(s, p, plane);
                    pout.push_back(t);
                    pout.push_back(p);
                }
            }
            else
            {
                if (inside(s, plane))
                {
                    Ogre::Vector3 t = clip(s, p, plane);
                    pout.push_back(t);
                }
            }

            s = p;
        }
        return pout;
    }

    static bool SegmentIntersection(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4)
    {
        double Ua, Ub, numerator_a, numerator_b, denominator;

        denominator=(y4-y3)*(x1-x2)-(x4-x3)*(y1-y2);

        if (denominator == 0)
        {
            if ( (x1*y2-x2*y1)*(x4-x3) - (x3*y4-x4*y3)*(x2-x1) == 0 && (x1*y2-x2*y1)*(y4-y3) - (x3*y4-x4*y3)*(y2-y1) == 0)
                return true;
            else
                return false;
        }
        else
        {
            numerator_a=(x4-x2)*(y4-y3)-(x4-x3)*(y4-y2);
            numerator_b=(x1-x2)*(y4-y2)-(x4-x2)*(y1-y2);
            Ua=numerator_a/denominator;
            Ub=numerator_b/denominator;

            if(Ua >=0 && Ua <=1 && Ub >=0 && Ub <=1)
                return true;
            else
                return false;
        }
    }
};

bool BQuadrAngle::isIntersection(Ogre::Vector3 a, Ogre::Vector3 b)
{
    vector<Ogre::Vector3>poly;
    poly.push_back(Ogre::Vector3(left_top.x,left_top.y,0));
    poly.push_back(Ogre::Vector3(right_bottom.x,left_top.y,0));
    poly.push_back(Ogre::Vector3(right_bottom.x,right_bottom.y,0));
    poly.push_back(Ogre::Vector3(left_top.x,right_bottom.y,0));

    list<Ogre::Vector3> poly1;
    poly1.push_back(poly[0]);
    poly1.push_back(poly[1]);
    poly1.push_back(poly[2]);
    poly1.push_back(poly[3]);
    poly1.push_back(poly[0]);
    auto kkk=Smoother::Polyclip(poly1,b,a);
    int seg=0;

    bool b1=Smoother::SegmentIntersection(left_top.x,left_top.y,        right_bottom.x,left_top.y,      a.x,a.y,b.x,b.y);
    bool b2=Smoother::SegmentIntersection(right_bottom.x,left_top.y,    right_bottom.x,right_bottom.y,  a.x,a.y,b.x,b.y);
    bool b3=Smoother::SegmentIntersection(left_top.x,right_bottom.y,    right_bottom.x,right_bottom.y,  a.x,a.y,b.x,b.y);
    bool b4=Smoother::SegmentIntersection(left_top.x,left_top.y,        left_top.x,right_bottom.y,      a.x,a.y,b.x,b.y);

   return b1||b2||b3||b4;

    if(kkk.size()>0)
    return true;
    else
        return false;
}

ostream& operator<<(ostream &out, const BQuadrAngle &barrier)
{
    out<<barrier.position<<endl;
    out<<barrier.scale<<endl;
    out<<barrier.scale.x<<endl<<barrier.scale.y<<endl;
    out<<barrier.left_top<<endl<<barrier.right_bottom<<endl;
    return out;
}

istream& operator>>(istream &in, BQuadrAngle &barrier)
{
    //in>>barrier.position;
    //in>>barrier.scale;
    in>>barrier.left_top.x>>barrier.left_top.y>>barrier.left_top.z;
    in>>barrier.right_bottom.x>>barrier.right_bottom.y>>barrier.right_bottom.z;
    //barrier.left_top=Ogre::Vector3(barrier.position.x-barrier.scale.x/2.0,barrier.position.y+barrier.scale.y/2.0);
    //barrier.right_bottom=Ogre::Vector3(barrier.position.x+barrier.scale.x/2.0,barrier.position.y-barrier.scale.y/2.0);
    return in;
}

QDataStream& operator <<(QDataStream &out, const Barrier &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    //out << b.position;
    //out << b.scale;
    return out;
}

QDataStream& operator >>(QDataStream &in, Barrier &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    //in >> b.position;
    //in >> b.scale;
    return in;
}

QDataStream& operator <<(QDataStream &out, const BQuadrAngle &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    //out << b.position;
    //out << b.scale;
    out << b.left_top.x<< b.left_top.y<< b.left_top.z;
    out << b.right_bottom.x<< b.right_bottom.y<< b.right_bottom.z;
    return out;
}

QDataStream& operator >>(QDataStream &in, BQuadrAngle &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    //in >> b.position;
    //in >> b.scale;
    in>>b.left_top.x>>b.left_top.y>>b.left_top.z;
    in>>b.right_bottom.x>>b.right_bottom.y>>b.right_bottom.z;
    return in;
}
