#include "car.h"

Car::Car():matrix4(), position(), scale(), rotation(), speed(0)
{

}


Car::Car(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33,float speed):
    matrix4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33),speed(speed)
{
    Ogre::Affine3 af(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23);
    af.decomposition(position,scale,rotation);
}

Car::Car(const Ogre::Matrix4 &matrix4, float speed):matrix4(matrix4),speed(speed)
{
    Ogre::Affine3 af(matrix4);
    af.decomposition(position,scale,rotation);
}

Car::Car(const Car&car):matrix4(car.matrix4),position(car.position), scale(car.scale),rotation(car.rotation), speed(car.speed)
{
    Ogre::Affine3 af(car.matrix4);
    af.decomposition(position,scale,rotation);
}
