#ifndef BARRIER_H
#define BARRIER_H
#include"point.h"
#include"mygraph.h"
#include "gamemap.h"
#include <fstream>

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

    /// Печать препятствия в консоль
    virtual void  print(IDistanceMatrixAdapter &adapter)=0;

    /// Печать препятствия в файл
    virtual void  printToFile(ofstream &out)=0;

    /// @brief Принадлежит ли точка пространства препятствию
    /// @param p точка, которую проверям
    /// @param g поле
    virtual bool hasPoint(Point p, GameMap&g)=0;

    /// @brief Принадлежит ли вершина графа препятствию
    /// @param v вершина графа, которую проверям
    /// @param g поле
    virtual bool hasVertex(int v, GameMap&g)=0;

    /// @brief Проверка на пересечение отрезка и препятствия
    /// @param a координаты начала отрезка
    /// @param b координаты конеца отрезка
    virtual bool isIntersection(Point a, Point b)=0;
};

/// @brief Базовый класс препятствия
class Barrier:public IBarrier, public Point
{
public:
    /// Конструктор по умолчанию
    Barrier();
    /// Конструктор с параметрами
    Barrier(float x, float y);
    /// Конструктор копирования
    Barrier(const Barrier &o);
    /// Деструктор
    ~Barrier();

    int init(MyGraph& graph, DMQuadrangle& distance)override;
    void print(IDistanceMatrixAdapter &adapter)override;
    bool hasPoint(Point p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    void printToFile(ofstream &out)override;
    bool isIntersection(Point a, Point b)override;

    friend ofstream& operator<<(ofstream &out, const Barrier &barrier);
};

/// @brief Класс прямоугольного препятствия
class BQuadrAngle: public Barrier
{
public:
    /// Конструктор по умолчанию
    BQuadrAngle();
    /// Конструктор с параметрами
    BQuadrAngle(float x, float y, float w, float h);
    /// Конструктор копирования
    BQuadrAngle(const BQuadrAngle& o);
    /// Деструктор
    ~BQuadrAngle();

    int init(MyGraph& graph, DMQuadrangle& distance)override;
    void print(IDistanceMatrixAdapter &adapter)override;
    bool hasPoint(Point p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    void printToFile(ofstream &out)override;
    bool isIntersection(Point a, Point b)override;

    friend ofstream& operator<<(ofstream &out, const BQuadrAngle &barrier);
    friend ostream& operator<<(ostream &out, const BQuadrAngle &barrier);
    friend istream& operator>>(istream &in, BQuadrAngle &barrier);

    /// Ширина препятствия
    float width;
    /// Высота препятствия
    float height;
    /// Левая верхняя точка препятствия
    Point left_top;
    /// Правая нижняя препятствия
    Point right_bottom;
};

#endif // BARRIER_H
