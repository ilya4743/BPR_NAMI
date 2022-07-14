#ifndef LOCATIONMAP_H
#define LOCATIONMAP_H
#include<iostream>
#include"point.h"
#include<vector>
#include"OgreMatrix4.h"

using namespace std;

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
public:
    /// Конструктор по умолчанию
    DMQuadrangle();

    /// Конструктор с параметрами
    DMQuadrangle(int width, int height, int center, float step);

    /// Конструктор копирования
    DMQuadrangle(const DMQuadrangle&o);

    /// Принадлежит ли точка матрице расстояний
    bool isPtInDM(const Ogre::Vector3& pt)
    {
        return(pt.x>=matrix[0].x && pt.x<=matrix[matrix.size()-1].x)&&(pt.z<=matrix[0].z && pt.z>=matrix[matrix.size()-1].z);
    }

    int     GetIG(const Ogre::Vector3& pt)
    {
        int i = 0;
        if(pt.z < 0) i=static_cast<int>(floor(pt.z / step));
        else if(pt.z > 0) i=static_cast<int>(ceil(pt.z / step));
        int j = 0;
        if(pt.x < 0) j = static_cast<int>(floor(pt.x / step));
        else if(pt.x > 0) j = static_cast<int>(ceil(pt.x / step));
        int u=(((center-center%width) - i * width));
                int o=(center + j) % width;
        return u+o;
    }

    int GetI(float j)
    {
        if(j>98)
            j=98;
        if(j<-98)
            j=-98;
        return (center - int(j / step) * width) / width;
    }

    int GetJ(float i)
    {
        /*
        i=-i;
        if(i>200)
            return 200;
        if(i<-200)
            return 0;*/
        return (center + int(i/step)) % width;
    }

    /// вернуть
    void toNearPt(Ogre::Vector3& pt)
    {
        /*if(!isPtInDM(pt))
        {
            if (pt.x<matrix[0].x)
                pt.x=matrix[0].x;
            else if(pt.x>matrix[matrix.size()-1].x)
                pt.x=matrix[matrix.size()-1].x;
            else
                pt.x=matrix[GetIG(pt)%width].x;

            if (pt.z>matrix[0].z)
                pt.z=matrix[0].z;
            else if(pt.z<matrix[matrix.size()-1].z)
                pt.z=matrix[matrix.size()-1].z;
            else
                pt.z=matrix[GetIG(pt)/width].z;

            return;
        }*/
        //int b=GetIG(pt);
        //pt=matrix[b];
    }

    void init() override;
    ~DMQuadrangle()override;
};

class DistanceMatrixPrinter
{
public:
    template<class DistanceMatrix>
    void print(const DistanceMatrix& distance);
};

class IDistanceMatrixAdapter
{
public:
    int virtual GetI(float j)=0;
    int virtual GetJ(float i)=0;
    int virtual GetIG(const Ogre::Vector3& pt)=0;
};

class DistanceMatrixAdapter:public IDistanceMatrixAdapter
{
public:

    DMQuadrangle* distance;

    DistanceMatrixAdapter(DMQuadrangle *distance)
    {
        this->distance=distance;
    }

    int GetI(float j)override
    {
        /*if(j>98)
            j=98;
        if(j<-98)
            j=-98;*/
        return (distance->center - int(j / distance->step) * distance->width) / distance->width;
    }

    int GetJ(float i)override
    {

        /*i=-i;
        if(i>200)
            return 200;
        if(i<-200)
            return 0;*/
        return (distance->center + int(i/distance->step)) % distance->width;
    }

    int GetIG(const Ogre::Vector3& pt) override
    {
        (distance->center - int(pt.z / distance->step) * distance->width) / distance->width;

        (distance->center + int(pt.x/distance->step)) % distance->width;
    }
};

#endif // LOCATIONMAP_H
