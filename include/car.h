#ifndef CAR_H
#define CAR_H
#include <OgreMatrix4.h>

/// @brief Класс автомобиля
class Car
{
public:
    /// Конструктор по умолчанию
    Car();

    /// @brief Конструктор с параметрами
    /// @param m00-m33 элементы матрицы трансформации
    /// @param speed скорость авто
    Car(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33, float speed);

    /// @brief Конструктор с параметрами
    /// @param matrix4 матрица трансформации
    /// @param speed скорость авто
    Car(const Ogre::Matrix4 &matrix4, float speed);

    /// Конструктор копирования
    Car(const Car&car);

    /// Матрица трансформации
    Ogre::Matrix4 matrix4;
    /// Позиция
    Ogre::Vector3 position;
    /// Масштаб
    Ogre::Vector3 scale;
    /// Поворот
    Ogre::Quaternion rotation;
    /// Скорость авто
    float speed;
};

#endif // CAR_H
