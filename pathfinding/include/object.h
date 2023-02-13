#pragma once

#include <OGRE/OgreMatrix4.h>

class IObject
{
    public:
    virtual ~IObject()=0;
};

class Object: public IObject
{
    public:
    Object():matrix4(){};
    
    Object(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33):
            matrix4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
    {
        Ogre::Affine3 af(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23);
        af.decomposition(this->position,this->scale,this->rotation);
    };
    
    Object(const Ogre::Matrix4 &matrix4):matrix4(matrix4)
    {
        Ogre::Affine3 af(matrix4);
        af.decomposition(this->position,this->scale,this->rotation);
    };
    
    Object(const Object&o):matrix4(o.matrix4), position(o.position), scale(o.scale), rotation(o.rotation){};
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::Quaternion rotation;
    
    protected:
    Ogre::Matrix4 matrix4;

};