#ifndef BARRIER_H
#define BARRIER_H
#include <OGRE/OgreMatrix4.h>

class GameMap;
/// @brief Интерфейс препятствия
class IBarrier
{
public:
    /// Виртуальный деструктор
    virtual ~IBarrier()=0;
};

/// @brief Базовый класс препятствия
class Barrier:public IBarrier
{
public:
    /// Конструктор по умолчанию
    Barrier();

    /// @brief Конструктор с параметрами
    /// @param m00-m33 элементы матрицы трансформации
    Barrier(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

    /// @brief Конструктор с параметрами
    /// @param matrix4 элементы матрица трансформации
    Barrier(const Ogre::Matrix4 &matrix4);

    /// Конструктор копирования
    Barrier(const Barrier &o);

    /// Деструктор
    ~Barrier()override;

    /// Матрица трансформации
    Ogre::Matrix4 matrix4;
    /// Позиция
    Ogre::Vector3 position;
    /// Масштаб
    Ogre::Vector3 scale;
    /// Поворот
    Ogre::Quaternion rotation;
};

/// @brief Класс прямоугольного препятствия
class BQuadrAngle: public Barrier
{
public:
    /// Конструктор по умолчанию
    BQuadrAngle();

    /// @brief Конструктор с параметрами
    /// @param m00-m33 элементы матрицы трансформации
    BQuadrAngle(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33);

    /// @brief Конструктор с параметрами
    /// @param matrix4 матрица трансформации
    BQuadrAngle(const Ogre::Matrix4 &matrix4);

    /// Конструктор копирования
    BQuadrAngle(const BQuadrAngle& o);

    /// Деструктор
    ~BQuadrAngle()override;

    /// Вершины куба
    Ogre::Vector3 p1,p2,p3,p4;
};

#endif // BARRIER_H
