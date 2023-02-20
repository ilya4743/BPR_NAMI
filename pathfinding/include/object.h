#pragma once

#include <boost/qvm.hpp>
#include <cmath>

using namespace boost::qvm;
using Quaternion=quat<float>;
using TransformMatrix=mat<float,4,4>;
using Vector4=vec<float, 4>;
using Vector3=vec<float, 3>;
using Vector2=vec<float, 2>;

inline Quaternion ExtractQuaternion(const TransformMatrix &m) noexcept{
    float w,x,y,z;
    float fTrace = m.a[0][0]+m.a[1][1]+m.a[2][2];
    float fRoot;

    if ( fTrace > 0.0 ){
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = std::sqrt(fTrace + 1.0f);  // 2w
        w = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;  // 1/(4w)
        x = (m.a[2][1]-m.a[1][2])*fRoot;
        y = (m.a[0][2]-m.a[2][0])*fRoot;
        z = (m.a[1][0]-m.a[0][1])*fRoot;
        return Quaternion{w,x,y,z};
    }
    else{
            // |w| <= 1/2
        static size_t s_iNext[3] = { 1, 2, 0 };
        size_t i = 0;
        if ( m.a[1][1] > m.a[0][0] )
            i = 1;
        if ( m.a[2][2] > m.a[i][i] )
            i = 2;
        size_t j = s_iNext[i];
        size_t k = s_iNext[j];

        fRoot = std::sqrt(m.a[i][i]-m.a[j][j]-m.a[k][k] + 1.0f);
        float* apkQuat[3] = { &x, &y, &z };
        *apkQuat[i] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        w = (m.a[k][j]-m.a[j][k])*fRoot;
        *apkQuat[j] = (m.a[j][i]+m.a[i][j])*fRoot;
        *apkQuat[k] = (m.a[k][i]+m.a[i][k])*fRoot;
        return Quaternion{w,*apkQuat[0],*apkQuat[1],*apkQuat[2]};
    }
}

inline Vector3 ExtractPosition(const TransformMatrix &m) noexcept{
    return Vector3{m.a[0][3],m.a[1][3],m.a[2][3]};
}

inline Vector4 ExtractScale(const TransformMatrix &m) noexcept{
    return Vector4{m.a[0][0],m.a[1][1],m.a[2][2],m.a[3][3]};
}

inline void SetPosition(const Vector4 vec4, TransformMatrix &m) noexcept{
    m.a[0][3]=vec4.a[0];
    m.a[1][3]=vec4.a[1];
    m.a[2][3]=vec4.a[2];
    m.a[3][3]=vec4.a[3];
}

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
            matrix4{m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33}
    {

    };
    
    Object(const TransformMatrix &matrix4):matrix4(matrix4)
    {
    };
    
    Object(const Object&o):matrix4(o.matrix4){};
    
    protected:
    TransformMatrix matrix4;
};