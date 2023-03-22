#pragma once

#include "object.h"

/// @brief Класс автомобиля
class Car : public Object {
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
    Car(const Eigen::Matrix4f &matrix4, float speed);

    /// Конструктор копирования
    Car(const Car &car);

    ~Car() override;
    /// Скорость авто
    float speed;
};