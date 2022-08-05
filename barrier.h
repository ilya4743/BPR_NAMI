#ifndef BARRIER_H
#define BARRIER_H
#include"mygraph.h"
#include "gamemap.h"
#include <fstream>
#include <QDataStream>
#include <OgreMatrix4.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

namespace bg = boost::geometry;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::polygon<point> polygon;
typedef bg::model::box<point> box;

class GameMap;
/// @brief Интерфейс препятствия
class IBarrier
{
public:

    /// Виртуальный деструктор
    virtual ~IBarrier()=0;

    /// @brief Инициализация препятсвия на графе
    /// @param grah граф, на который необходимо наложить препятствие
    /// @param step шаг сетки графа
    /// @param g поле
    virtual int  init(DMQuadrangle& distance)=0;

    /// @brief Принадлежит ли точка пространства препятствию
    /// @param p точка, которую проверям
    /// @param g поле
    virtual bool hasPoint(Ogre::Vector3 p, GameMap&g)=0;

    /// @brief Принадлежит ли вершина графа препятствию
    /// @param v вершина графа, которую проверям
    /// @param g поле
    virtual bool hasVertex(int v, GameMap&g)=0;

    /// @brief Проверка на пересечение отрезка и препятствия
    /// @param a координаты начала отрезка
    /// @param b координаты конеца отрезка
    virtual bool isIntersection(Ogre::Vector3 a, Ogre::Vector3 b)=0;
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
    ~Barrier();

    /// Матрица трансформации
    Ogre::Matrix4 matrix4;
    /// Позиция
    Ogre::Vector3 position;
    /// Масштаб
    Ogre::Vector3 scale;
    /// Поворот
    Ogre::Quaternion rotation;

    int init(DMQuadrangle& distance)override;
    bool hasPoint(Ogre::Vector3 p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    bool isIntersection(Ogre::Vector3 a, Ogre::Vector3 b)override;

    friend ofstream& operator<<(ofstream &out, const Barrier &barrier);
    friend QDataStream& operator <<(QDataStream &out, const Barrier &b);
    friend QDataStream& operator >>(QDataStream &in, Barrier &b);
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
    ~BQuadrAngle();

    int init(DMQuadrangle& distance)override;
    bool hasPoint(Ogre::Vector3 p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    bool isIntersection(Ogre::Vector3 a, Ogre::Vector3 b)override;

    friend ofstream& operator<<(ofstream &out, const BQuadrAngle &barrier);
    friend ostream& operator<<(ostream &out, const BQuadrAngle &barrier);
    friend istream& operator>>(istream &in, BQuadrAngle &barrier);
    friend QDataStream& operator <<(QDataStream &out, const BQuadrAngle &b);
    friend QDataStream& operator >>(QDataStream &in, BQuadrAngle &b);

    /// Вершины куба
    Ogre::Vector3 p1,p2,p3,p4,p5,p6,p7,p8;
};

class PrinterBQuadrAngle
{
private:
    void drawLine(int x1, int y1, int x2,  int y2,DMQuadrangle& distance,MyGraph& g);

public:
    void drawCube(const BQuadrAngle& barrier, IDistanceMatrixAdapter & adapter, DMQuadrangle& distance, MyGraph& g);
};

#endif // BARRIER_H
