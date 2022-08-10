#include "barrier.h"
#include <QDebug>

IBarrier::~IBarrier(){}

Barrier::Barrier():matrix4(),position(), scale(), rotation(){}
Barrier::Barrier(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33):
                matrix4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
{
    Ogre::Affine3 af(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23);
    af.decomposition(position,scale,rotation);
}

Barrier::Barrier(const Ogre::Matrix4 &matrix4):matrix4(matrix4)
{
    Ogre::Affine3 af(matrix4);
    af.decomposition(position,scale,rotation);
}

Barrier::Barrier(const Barrier&o):matrix4(o.matrix4), position(o.position),scale(o.scale), rotation(o.rotation)
{
}

int Barrier::init(DMQuadrangle& distance,vector<Ogre::Vector3>& out){}
Barrier::~Barrier(){}

BQuadrAngle::BQuadrAngle():Barrier()
{
}

BQuadrAngle::BQuadrAngle(float m00, float m01, float m02, float m03,
                         float m10, float m11, float m12, float m13,
                         float m20, float m21, float m22, float m23,
                         float m30, float m31, float m32, float m33):
                        Barrier(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
{
}

BQuadrAngle::BQuadrAngle(const Ogre::Matrix4 &matrix4):Barrier(matrix4),p1(matrix4*Ogre::Vector3(1,-1,1)),p2(matrix4*Ogre::Vector3(1,-1,-1)),p3(matrix4*Ogre::Vector3(-1,-1,-1)),p4(matrix4*Ogre::Vector3(-1,-1,1))
//Barrier(matrix4),p1(0.5,-0.5,-0.5),p2(0.5,-0.5,0.5),p3(0.5,0.5,-0.5),p4(0.5,0.5,0.5),p5(-0.5,0.5,-0.5),p6(-0.5,0.5,0.5),p7(-0.5,-0.5,-0.5),p8(-0.5,-0.5,0.5)
//Barrier(matrix4),p1(1,0,0),p2(1,0,1),p3(1,1,0),p4(1,1,1),p5(0,1,0),p6(0,1,1),p7(0,0,0),p8(0,0,1)
{
}

BQuadrAngle::BQuadrAngle(const BQuadrAngle& o):Barrier(o)
{

}

int BQuadrAngle::init(DMQuadrangle& distance, vector<Ogre::Vector3>& out)
{
    polygon poly1{{{p1.x,p1.z},{p2.x,p2.z},{p3.x,p3.z},{p4.x,p4.z},{p1.x,p1.z}}};
    box box1{{distance.matrix[0].x, distance.matrix[distance.matrix.size()-1].z},
             {distance.matrix[distance.matrix.size()-1].x, distance.matrix[0].z}};

    vector < polygon >  output ;
    boost::geometry::intersection(box1, poly1, output);
    if(output.size()>0)
    {
        out.reserve(output[0].outer().size());
        for(int i=0; i<output[0].outer().size();i++)
            out.push_back(Ogre::Vector3(bg::get<0>(output[0].outer()[i]),0,bg::get<1>(output[0].outer()[i])));
        return 0;
    }
    else
        return -1;
}

BQuadrAngle::~BQuadrAngle()
{
    //cout<<"Препятствие удалено!";
}

bool correct(int u1, int u2)
{
    if(abs(u1/200-u2/200)==1)
        if(u2>-1&&u2<20000)
            return 1;
    return 0;
}

void PrinterBQuadrAngle::drawLine(int x1, int y1, int x2,  int y2,DMQuadrangle& distance, MyGraph& g)
{
    cout<<"\033[s";
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    if(y2>=0 && x2>=0)
    {
    cout<<"\033["<<y2+1<<';'<<x2+1<<"H\033[0;37;47m \033[0;0m";
    int u=distance.width*y2+x2;
    clear_vertex(u,*g.adj_list);

    if(correct(u-1,u-distance.width))
        remove_edge(u-1,u-distance.width,*g.adj_list);
    if(correct(u-1,u+distance.width))
        remove_edge(u-1,u+distance.width,*g.adj_list);
    if(correct(u+1,u+distance.width))
        remove_edge(u+1,u+distance.width,*g.adj_list);
    if(correct(u+1,u-distance.width))
        remove_edge(u+1,u-distance.width,*g.adj_list);

    }
    while(x1 != x2 || y1 != y2)
   {
        if(y1>=0 && x1>=0)

        cout<<"\033["<<y1+1<<';'<<x1+1<<"H\033[0;37;47m \033[0;0m";
        int u=distance.width*y1+x1;
        clear_vertex(u,*g.adj_list);

        if(correct(u-1,u-distance.width))
            remove_edge(u-1,u-distance.width,*g.adj_list);
        if(correct(u-1,u+distance.width))
            remove_edge(u-1,u+distance.width,*g.adj_list);
        if(correct(u+1,u+distance.width))
            remove_edge(u+1,u+distance.width,*g.adj_list);
        if(correct(u+1,u-distance.width))
            remove_edge(u+1,u-distance.width,*g.adj_list);
        int error2 = error * 2;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
    cout<<"\033[u";
}

void PrinterBQuadrAngle::drawCube(const vector<Ogre::Vector3>& clipping,const BQuadrAngle& barrier, IDistanceMatrixAdapter & adapter,DMQuadrangle& distance, MyGraph& g)
{
    for(int i=1; i<clipping.size(); i++)
    {
        int y0=adapter.GetI(clipping[i-1].z);
        int x0=adapter.GetJ(clipping[i-1].x);

        int y1=adapter.GetI(clipping[i].z);
        int x1=adapter.GetJ(clipping[i].x);

        drawLine(x0, y0, x1, y1,distance,g);
    }
}
