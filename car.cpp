#include "car.h"

Car::Car():affine3(), position(), scale(), rotation(), speed(0)
{

}


Car::Car(float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23, float speed):affine3(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23),speed(speed)
{
    affine3.decomposition(position,scale,rotation);
}

Car::Car(const Ogre::Matrix4 &matrix4, float speed):matrix4(matrix4),affine3(matrix4),speed(speed)
{
    affine3.decomposition(position,scale,rotation);

}

Car::Car(const Car&car):matrix4(car.matrix4),affine3(car.affine3),position(car.position), scale(car.scale),rotation(car.rotation), speed(car.speed)
{
    affine3.decomposition(position,scale,rotation);

}

ostream& operator <<(ostream &out, const Car &car)
{
    out<<car.scale<<endl<<car.speed;
    return out;
}

istream& operator >>(istream &in, Car &car)
{
    //in>>car.scale>>car.speed;
    return in;
}

QDataStream& operator <<(QDataStream &out, const Car &car)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;

    Ogre::Matrix4 mat;
    //out<<car.scale<<car.speed;
    return out;
}

QDataStream& operator >>(QDataStream &in, Car &car)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    in>>m00>>m01>>m02>>m03>>m10>>m11>>m12>>m13>>m20>>m21>>m22>>m23;
    //car.affine3.s(m00, m01, m02, m03,m10, m11, m12, m13,m20, m21, m22, m23);
    //in>>car.scale>>car.speed;
    return in;
}
