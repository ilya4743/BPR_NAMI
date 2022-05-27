#include "car.h"

Car::Car():scale(),speed(0)
{

}

Car::Car(Scale scale, float speed):scale(scale), speed(speed)
{

}

Car::Car(const Car&car):scale(car.scale), speed(car.speed)
{

}

ostream& operator <<(ostream &out, const Car &car)
{
    out<<car.scale<<endl<<car.speed;
    return out;
}

istream& operator >>(istream &in, Car &car)
{
    in>>car.scale>>car.speed;
    return in;
}

QDataStream& operator <<(QDataStream &out, const Car &car)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out<<car.scale<<car.speed;
    return out;
}

QDataStream& operator >>(QDataStream &in, Car &car)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in>>car.scale>>car.speed;
    return in;
}
