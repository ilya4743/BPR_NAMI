#ifndef BARRIER_H
#define BARRIER_H
#include"point.h"
#include"mygraph.h"
#include "gamemap.h"
#include <fstream>
#include <QDataStream>
#include <OgreMatrix4.h>

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
    virtual int  init(MyGraph& graph, DMQuadrangle& distance)=0;

    /*
    /// Печать препятствия в консоль
    virtual void  print(IDistanceMatrixAdapter &adapter)=0;

    /// Печать препятствия в файл
    virtual void  printToFile(ofstream &out)=0;
    */

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
    /// Конструктор с параметрами
    Barrier(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23);
    Barrier(const Ogre::Matrix4 &matrix4);
    /// Конструктор копирования
    Barrier(const Barrier &o);
    /// Деструктор
    ~Barrier();
    /// Позиция
    //Position position;
    /// Масштабирование
    //Scale scale;

    Ogre::Affine3 affine3;
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::Quaternion rotation;

    int init(MyGraph& graph, DMQuadrangle& distance)override;
    //void print(IDistanceMatrixAdapter &adapter)override;
    bool hasPoint(Ogre::Vector3 p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    //void printToFile(ofstream &out)override;
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
    /// Конструктор с параметрами
    BQuadrAngle(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23);

    BQuadrAngle(const Ogre::Matrix4 &matrix4);
    /// Конструктор копирования
    BQuadrAngle(const BQuadrAngle& o);
    /// Деструктор
    ~BQuadrAngle();

    int init(MyGraph& graph, DMQuadrangle& distance)override;
    //void print(IDistanceMatrixAdapter &adapter)override;
    bool hasPoint(Ogre::Vector3 p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    //void printToFile(ofstream &out)override;
    bool isIntersection(Ogre::Vector3 a, Ogre::Vector3 b)override;

    friend ofstream& operator<<(ofstream &out, const BQuadrAngle &barrier);
    friend ostream& operator<<(ostream &out, const BQuadrAngle &barrier);
    friend istream& operator>>(istream &in, BQuadrAngle &barrier);
    friend QDataStream& operator <<(QDataStream &out, const BQuadrAngle &b);
    friend QDataStream& operator >>(QDataStream &in, BQuadrAngle &b);

    /// Левая верхняя точка препятствия
    Ogre::Vector3 left_top;
    /// Правая нижняя препятствия
    Ogre::Vector3 right_bottom;
    Ogre::Vector3 p1;
};

class PrinterBQuadrAngle
{
private:
    void line(int x1, int y1, int x2,  int y2)
    {
        cout<<"\033[s";
        const int deltaX = abs(x2 - x1);
        const int deltaY = abs(y2 - y1);
        const int signX = x1 < x2 ? 1 : -1;
        const int signY = y1 < y2 ? 1 : -1;
        int error = deltaX - deltaY;
        cout<<"\033["<<y2<<';'<<x2<<"H\033[0;37;47m \033[0;0m";
        while(x1 != x2 || y1 != y2)
       {
            cout<<"\033["<<y1<<';'<<x1<<"H\033[0;37;47m \033[0;0m";
            int error2 = error * 2;
            if(error2 > -deltaY)
            {
                error -= deltaY;
                x1 += signX;
            }
            if(error2 < deltaX)
            {
                error += deltaX;
                y1 += signY;
            }
        }
        cout<<"\033[u";
    }

public:
    void drawCube(const BQuadrAngle& barrier, IDistanceMatrixAdapter & adapter)
    {
        float gg=abs(barrier.right_bottom.y-barrier.left_top.y);
        //правая верхняя точка препятствия
        Ogre::Vector3 p1(barrier.right_bottom.x,barrier.right_bottom.y+gg, barrier.left_top.z);
        //левая нижняя точка

        Ogre::Vector3 p2(barrier.left_top.x,barrier.left_top.y-gg, barrier.left_top.z);

        int y0=adapter.GetI(barrier.left_top.y);
        int x0=adapter.GetJ(barrier.left_top.x);

        int y1=adapter.GetI(p1.y);
        int x1=adapter.GetJ(p1.x);

        int y2=adapter.GetI(barrier.right_bottom.y);
        int x2=adapter.GetJ(barrier.right_bottom.x);

        int y3=adapter.GetI(p2.y);
        int x3=adapter.GetJ(p2.x);

        //верхняя горизонталь
        line(x0, y0, x1, y1);
        //правая вертикаль
        line(x1, y1, x2, y2);
        //левая вертикаль
        line(x0, y0, x3, y3);
        //нижняя горизонталь
        line(x3, y3, x2, y2);
    }
};

#endif // BARRIER_H
