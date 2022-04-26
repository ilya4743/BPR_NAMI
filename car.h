#ifndef CAR_H
#define CAR_H
#include<iostream>
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
    Car(float width, float height, float speed);

    /// @brief Конструктор копирования
    /// @param car экземпляр авто
    /// @return экземпляр авто
    Car(const Car&car);

    /// Ширина авто
    float width;

    /// Высота авто
    float height;

    /// Скорость авто
    float speed;

    friend ostream& operator <<(ostream &out, const Car &b);
    friend istream& operator >>(istream &in, Car &b);
};

#endif // CAR_H
