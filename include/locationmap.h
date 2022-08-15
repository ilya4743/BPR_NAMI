#ifndef LOCATIONMAP_H
#define LOCATIONMAP_H
#include<iostream>
#include<vector>
#include"OgreMatrix4.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

using namespace std;
namespace bg = boost::geometry;

/// @brief Интерфейс матрицы расстояний
class IDistanceMatrix
{
public:
    /// Инициализация матрицы расстояний
    void virtual init()=0;

    /// Виртуальный деструктор
    virtual ~IDistanceMatrix()=0;
};

/// @brief Базовый класс матрицы расстояний
class DistanceMatrix:public IDistanceMatrix
{
public:
    /// Вектор точек будущей матрицы
    vector<Ogre::Vector3> matrix;
    void init()override;
     ~DistanceMatrix()override;
};

/// @brief Класс прямоугольной матрицы расстояний
/** @details
 *  <p>Матрица создается на базе вектора точек (x, y).
 *  <br>Значение <b>width</b> задает количество элементов матрицы по ширине.
 *  <br>Значение <b>height</b> задает количетво элементов матрицы по высоте.
 *  <br>Значение <b>step</b>(шаг) отвечает за расстояние от одной точки до другой.
 *  <br>Значение <b>center</b> задает номер элемента матрицы, который будет считаться точкой отсчета.
 *  <br>На рисунке представлена схема прямоугольной матрицы расстояний, с точкой отсчета в элементе под номером 12.
 *  <p>
 *  <img src="pic/DMQuadrAngle1.svg" alt="Схема прямоугольной матрицы расстояний" height="425px" width="450px" />
*/
class DMQuadrangle: public DistanceMatrix
{
public:
    /// Ширина матрицы
    int width;

    /// Высота матрицы
    int height;

    /// Точка отсчета в прямоугольной системе координат
    int center;

    /// Шаг элементов матрицы расстояний
    float step;

    /// Конструктор по умолчанию
    DMQuadrangle();

    /// Конструктор с параметрами
    DMQuadrangle(int width, int height, int center, float step);

    /// Конструктор копирования
    DMQuadrangle(const DMQuadrangle&o);

    void init() override;

    int GetI(float j) const
    {
        return (center - int(j / step) * width) / width;
    }

    int GetJ(float i) const
    {
        return (center + int(i/step)) % width;
    }

    ///Получить левую нижнюю точку (min_corner)
    bg::model::d2::point_xy<float> getLDP() const
    {return bg::model::d2::point_xy<float>(matrix[0].x,matrix[matrix.size()-1].z);}

    ///Получить правую нижнюю точку (max_corner)
    bg::model::d2::point_xy<float> getRUP() const
    {return bg::model::d2::point_xy<float>(matrix[matrix.size()-1].x,matrix[0].z);}

    ~DMQuadrangle()override;
};

class DistanceMatrixPrinter
{
public:
    template<class DistanceMatrix>
    void print(const DistanceMatrix& distance);
};

#endif // LOCATIONMAP_H
