#include "barrier.h"

IBarrier::~IBarrier(){}

Barrier::Barrier():Point(){}
Barrier::Barrier(float x, float y):Point(x,y){}
Barrier::Barrier(const Barrier&o):Point(o){}
int Barrier::init(MyGraph& graph, float step, GameMap&g){}
void Barrier::print(GameMap&g){}
Barrier::~Barrier(){}

BQuadrAngle::BQuadrAngle():Barrier(),width(0),height(0)
{

}

BQuadrAngle::BQuadrAngle(float x, float y, float w, float h):Barrier(x,y),width(w),height(h),
    left_top(x-w/2.0,y+h/2.0), right_bottom(x+w/2.0,y-h/2.0)
{

}

BQuadrAngle::BQuadrAngle(const BQuadrAngle& o):Barrier(o.x,o.y),width(o.width),height(o.height),
    left_top(o.left_top),right_bottom(o.right_bottom)
{

}

void BQuadrAngle::print(GameMap&g)
{
    cout<<"\033[s";
    for (int i = g.GetI(this->left_top.y); i <= g.GetI(this->right_bottom.y); i++)
        for (int j = g.GetJ(this->left_top.x); j <= g.GetJ(this->right_bottom.x); j++)
            cout<<"\033["<<i+1<<';'<<j+1<<"H\033[0;37;47m \033[0;0m";
    cout<<"\033[u";
}

int BQuadrAngle::init(MyGraph& graph, float step, GameMap&g)
{
    //искуственно увеличиваем препятствие, чтобы соблюсти габариты

    this->left_top.x=this->left_top.x-g.width_auto/2;
    this->left_top.y=this->left_top.y+g.height_auto/2;
    this->right_bottom.x=this->right_bottom.x+g.width_auto/2;
    this->right_bottom.y=this->right_bottom.y-g.height_auto/2;

    //g.GetI(g.center)
/*
    //если препятсвие не влезет на карту, то ошибка
    float lx=this->left_top.x;
    float lx2=g.distance->matrix[0].x;
    float rx=this->right_bottom.x;
    float rx1=g.distance->matrix[g.num_vertices_width].x;

    float ly=this->left_top.y;
    float ly1=g.distance->matrix[0].y;
    float ry=this->right_bottom.y;
    float ry1= g.distance->matrix[g.num_vertices_width].y;


    if(lx>g.distance->matrix[0].x || rx>g.distance->matrix[g.num_vertices_width].x||
       ly>g.distance->matrix[0].y || ry>g.distance->matrix[g.num_vertices_height*g.num_vertices_width-1].x)
    {
        //cout<<"Errrorororo";
        return -2;
    }*/

    if (abs(this->left_top.x) > abs(int(this->left_top.x / step) * step))
    {
        this->left_top.x -= abs(this->left_top.x - int(this->left_top.x / step) * step);
     /*   if (this->left_top.x > 0)
            this->left_top.x += step;
        else
            this->left_top.x -= step;*/
    }

    if (abs(this->right_bottom.x) > abs(int(this->right_bottom.x / step) * step))
    {
        this->right_bottom.x +=abs(this->right_bottom.x - int(this->right_bottom.x / step) * step);
     /*   if (this->right_bottom.x > 0)
            this->right_bottom.x += step;
        else
            this->right_bottom.x -= step;*/
    }

    if (abs(this->left_top.y) > abs(int(this->left_top.y / step) * step))
    {
        this->left_top.y +=abs(this->left_top.y - int(this->left_top.y / step) * step);
    /*    if (this->left_top.y > 0)
            this->left_top.y += step;
        else
            this->left_top.y -= step;*/
    }

    if (abs(this->right_bottom.y) > abs(int(this->right_bottom.y / step) * step))
    {
        this->right_bottom.y -= abs(this->right_bottom.y - int(this->right_bottom.y / step) * step);
    /*    if (this->right_bottom.y > 0)
            this->right_bottom.y += step;
        else
            this->right_bottom.y -= step;*/
    }

    int a1=g.GetJ(this->left_top.x);
    int a2=g.GetI(this->left_top.y);
    int c1=a1+a2*g.num_vertices_width;
    int b1=g.GetJ(this->right_bottom.x);
    int b2=g.GetI(this->right_bottom.y);
    int c2=b1+b2*g.num_vertices_width;

    if(g.distance->matrix[0].x > this->left_top.x || g.distance->matrix[0].y < this->left_top.y ||
       g.distance->matrix[g.distance->matrix.size()-1].x < this->right_bottom.x ||
       g.distance->matrix[g.distance->matrix.size()-1].y > this->right_bottom.y)
            return -2;
    //if(a1<0||a2<0||b1<0||b2<0||b2>=g.num_vertices_height||
    //   b1>=g.num_vertices_width||a1>=g.num_vertices_width||a2>=g.num_vertices_height)
    //    return -2;

    //соседние по горизонтали
    for (int i = g.GetI(this->left_top.y); i <= g.GetI(this->right_bottom.y); i++)
    {
        for (int j = g.GetJ(this->left_top.x); j <= g.GetJ(this->right_bottom.x) + 1; j++)
        {
            if (j == 0 || j == g.num_vertices_width)continue;

            int u = i * g.num_vertices_width + j - 1;
            int v = i * g.num_vertices_width + j;
            remove_edge(u, v, *graph.adj_list);
        }
    }

    //соседние по вертикале
    for (int i = g.GetI(this->left_top.y); i <= g.GetI(this->right_bottom.y) + 1; i++)
    {
        if (i == 0 || i == g.num_vertices_height)continue;

        for (int j = g.GetJ(this->left_top.x); j <= g.GetJ(this->right_bottom.x); j++)
        {
            int u2 = (i - 1) * g.num_vertices_width + j;
            int v2 = (i)*g.num_vertices_width + j;
            remove_edge(u2, v2, *graph.adj_list);
        }
    }

    //главная диагональ
    for (int i = g.GetI(this->left_top.y); i <= g.GetI(this->right_bottom.y) + 1; i++)
    {
        if (i == 0 || i == g.num_vertices_height)continue;
        for (int j = g.GetJ(this->left_top.x); j <= g.GetJ(this->right_bottom.x) + 1; j++)
        {
            if (j == 0 || j == g.num_vertices_width)continue;
            int u1 = (i - 1) * g.num_vertices_width + j - 1;
            int v1 = (i)*g.num_vertices_width + j;
            remove_edge(u1, v1, *graph.adj_list);
        }
    }
    //побочная диагональ
    for (int i = g.GetI(this->right_bottom.y) + 1; i >= g.GetI(this->left_top.y); i--)
    {
        if (i == g.num_vertices_height || i==0)continue;
        for (int j = g.GetJ(this->right_bottom.x) + 1; j >= g.GetJ(this->left_top.x); j--)
        {
            if (j == g.num_vertices_width || j == 0)continue;
            int u1 = (i)*g.num_vertices_width + j - 1;
            int v1 = (i - 1) * g.num_vertices_width + j;
            remove_edge(u1, v1, *graph.adj_list);
        }
    }
    return 0;
}

BQuadrAngle::~BQuadrAngle(){}
