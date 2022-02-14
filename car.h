#ifndef CAR_H
#define CAR_H


class Car
{
public:
    Car();
    Car(float width, float height, float speed):width(width),height(height), speed(speed)
    {

    }
    Car(const Car&o):width(o.width), height(o.height), speed(o.speed)
    {

    }
    float width;
    float height;
    float speed;
};

#endif // CAR_H
