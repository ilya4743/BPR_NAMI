#ifndef CAR_H
#define CAR_H
#include<iostream>
#include"point.h"
#include <OgreMatrix4.h>

using namespace std;

/// @brief Класс автомобиля
class Car
{
public:
    ///Конструктор по умолчанию
    Car();

    /// @brief Конструктор с параметрами
    /// @param width ширина авто
    /// @param height высота авто
    /// @param speed скорость авто
    Car(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float speed);

    /// @brief Конструктор копирования
    /// @param car экземпляр авто
    /// @return экземпляр авто
    Car(const Car&car);

    /// Масштабирование
    //Scale scale;

    /// Скорость авто
    float speed;

    Ogre::Affine3 affine3;

    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::Quaternion rotation;

    friend ostream& operator <<(ostream &out, const Car &b);
    friend istream& operator >>(istream &in, Car &b);
    friend QDataStream& operator <<(QDataStream &out, const Car &car);
    friend QDataStream& operator >>(QDataStream &in, Car &car);
};

#endif // CAR_H
