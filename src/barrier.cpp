#include "barrier.h"

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

Barrier::~Barrier()
{
    //cout<<"Препятствие удалено!";
}

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

BQuadrAngle::~BQuadrAngle()
{
    //cout<<"Препятствие удалено!";
}
