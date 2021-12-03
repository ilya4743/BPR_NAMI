#include "barrier.h"

Barrier::Barrier():width(0),height(0)
{

}

Barrier::Barrier(float x, float y, float w, float h):width(w),height(h),Point(x,y),
    left_top(x-w/2.0,y+h/2.0), right_bottom(x+w/2.0,y-h/2.0)
{

}

Barrier::Barrier(const Barrier& o):
    width(o.width),height(o.height),Point(o.center),left_top(o.left_top),right_bottom(o.right_bottom)
{

}

void Barrier::print(GameMap&g)
{
    cout<<"\033[s";
    for (int i = g.GetI(this->left_top.y); i <= g.GetI(this->right_bottom.y); i++)
        for (int j = g.GetJ(this->left_top.x); j <= g.GetJ(this->right_bottom.x); j++)
            cout<<"\033["<<i+1<<';'<<j+1<<"H\033[0;37;47m \033[0;0m";
    cout<<"\033[u";
}


void Barrier::init(MyGraph& graph, float step, GameMap&g)
{
    if (abs(this->left_top.x) > abs(int(this->left_top.x / step) * step))
    {
        this->left_top.x = int(this->left_top.x / step) * step;
        if (this->left_top.x > 0)
            this->left_top.x += step;
        else
            this->left_top.x -= step;
    }

    if (abs(this->right_bottom.x) > abs(int(this->right_bottom.x / step) * step))
    {
        this->right_bottom.x = int(this->right_bottom.x / step) * step;
        if (this->right_bottom.x > 0)
            this->right_bottom.x += step;
        else
            this->right_bottom.x -= step;
    }

    if (abs(this->left_top.y) > abs(int(this->left_top.y / step) * step))
    {
        this->left_top.y = int(this->left_top.y / step) * step;
        if (this->left_top.y > 0)
            this->left_top.y += step;
        else
            this->left_top.y -= step;
    }

    if (abs(this->right_bottom.y) > abs(int(this->right_bottom.y / step) * step))
    {
        this->right_bottom.y = int(this->right_bottom.y / step) * step;
        if (this->right_bottom.y > 0)
            this->right_bottom.y += step;
        else
            this->right_bottom.y -= step;
    }

    //соседние по горизонтали
    for (int i = g.GetI(this->left_top.y); i <= g.GetI(this->right_bottom.y); i++)
    {
        for (int j = g.GetJ(this->left_top.x); j <= g.GetJ(this->right_bottom.x) + 1; j++)
        {
            if (j == 0 || j == g.num_vertices_width)continue;

            int u = i * width + j - 1;
            int v = i * width + j;
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
        if (i == 0 || i == height)continue;
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
}
