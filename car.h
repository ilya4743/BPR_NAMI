#ifndef CAR_H
#define CAR_H

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
    Car(float width, float height, float speed):width(width),height(height), speed(speed)
    {

    }

    /// @brief Конструктор копирования
    /// @param car экземпляр авто
    /// @return экземпляр авто
    Car(const Car&car):width(car.width), height(car.height), speed(car.speed)
    {

    }

    /// Ширина авто
    float width;

    /// Высота авто
    float height;

    /// Скорость авто
    float speed;
};

#endif // CAR_H
