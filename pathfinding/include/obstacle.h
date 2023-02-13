#pragma once

#include "object.h"

/// @brief Класс прямоугольного препятствия
class BQuadrAngle: public Object
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
    std::vector<Ogre::Vector3> GetVertexes() const {return this->vertexes;};

private:
    /// Вершины куба
    std::vector<Ogre::Vector3> vertexes={Ogre::Vector3(1,-1,1), Ogre::Vector3(1,-1,-1), Ogre::Vector3(-1,-1,-1), Ogre::Vector3(-1,-1,1)};
};