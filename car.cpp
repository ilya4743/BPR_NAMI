#include "car.h"

Car::Car():width(0),height(0),speed(0)
{

}

Car::Car(float width, float height, float speed):width(width),height(height), speed(speed)
{

}

Car::Car(const Car&car):width(car.width), height(car.height), speed(car.speed)
{

}

ostream& operator <<(ostream &out, const Car &car)
{
    out<<car.width<<endl<<car.height<<endl<<car.speed;
    return out;
}

istream& operator >>(istream &in, Car &car)
{
    in>>car.width>>car.height>>car.speed;
    return in;
}
