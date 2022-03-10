#include "barrier.h"

IBarrier::~IBarrier(){}

Barrier::Barrier():Point(){}
Barrier::Barrier(float x, float y):Point(x,y){}
Barrier::Barrier(const Barrier&o):Point(o){}
int Barrier::init(MyGraph& graph, float step, GameMap&g){}
void Barrier::print(GameMap&g){}
Barrier::~Barrier(){}

bool Barrier::hasPoint(Point p, GameMap&g){}
bool Barrier::hasVertex(int v, GameMap&g){}
bool Barrier::isIntersection(Point a, Point b){}
void Barrier::printToFile(ofstream &out){}

ofstream& operator<<(ofstream &out, const Barrier &barrier)
{
    out<<barrier.x<<endl<<barrier.y<<endl;
    return out;
}

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
    /*
    //искуственно увеличиваем препятствие, чтобы соблюсти габариты
    this->left_top.x=this->left_top.x-g.width_auto/2;
    this->left_top.y=this->left_top.y+g.height_auto/2;
    this->right_bottom.x=this->right_bottom.x+g.width_auto/2;
    this->right_bottom.y=this->right_bottom.y-g.height_auto/2;

    if (abs(this->left_top.x) > abs(int(this->left_top.x / step) * step))
        this->left_top.x -= abs(this->left_top.x - int(this->left_top.x / step) * step);

    if (abs(this->right_bottom.x) > abs(int(this->right_bottom.x / step) * step))
        this->right_bottom.x +=abs(this->right_bottom.x - int(this->right_bottom.x / step) * step);

    if (abs(this->left_top.y) > abs(int(this->left_top.y / step) * step))
        this->left_top.y +=abs(this->left_top.y - int(this->left_top.y / step) * step);

    if (abs(this->right_bottom.y) > abs(int(this->right_bottom.y / step) * step))
        this->right_bottom.y -= abs(this->right_bottom.y - int(this->right_bottom.y / step) * step);
    */
    //если препятствие не влезет на карту, то ошибка
    /*if(g.distance->matrix[0].x > this->left_top.x || g.distance->matrix[g.distance->matrix.size()-1].x < this->right_bottom.x ||
       g.distance->matrix[0].y < this->left_top.y || g.distance->matrix[g.distance->matrix.size()-1].y > this->right_bottom.y)
        return -2;*/

    //если препятсвие никак не влезет на карту, то вернуть 0 иначе попытаться впихнуть хоть как-нибудь
    if(g.distance->matrix[0].x > this->right_bottom.x || g.distance->matrix[g.distance->matrix.size()-1].x < this->left_top.x ||
       g.distance->matrix[0].y < this->right_bottom.y || g.distance->matrix[g.distance->matrix.size()-1].y > this->left_top.y)
    {
        //cout<<(g.distance->matrix[0].x > this->right_bottom.x && g.distance->matrix[g.distance->matrix.size()-1].x < this->left_top.x)<<endl;
        //cout<<(g.distance->matrix[0].y < this->right_bottom.y && g.distance->matrix[g.distance->matrix.size()-1].y > this->left_top.y)<<endl;
            //return -2;
        //delete this;
        return -2;
    }
    else
    {
        if (g.distance->matrix[0].x > this->left_top.x)
            this->left_top.x=g.distance->matrix[0].x;

        if (g.distance->matrix[0].y < this->left_top.y)
            this->left_top.y=g.distance->matrix[0].y;

        if (g.distance->matrix[g.distance->matrix.size()-1].x < this->right_bottom.x)
            this->right_bottom.x=g.distance->matrix[g.distance->matrix.size()-1].x;

        if (g.distance->matrix[g.distance->matrix.size()-1].y > this->right_bottom.y)
            this->right_bottom.y=g.distance->matrix[g.distance->matrix.size()-1].y;

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
}

BQuadrAngle::~BQuadrAngle()
{
    //cout<<"Препятствие удалено!";
}

bool BQuadrAngle::hasPoint(Point p, GameMap&g)
{
    //искуственно увеличиваем препятствие, чтобы соблюсти габариты
    this->left_top.x=this->left_top.x-g.width_auto/2;
    this->left_top.y=this->left_top.y+g.height_auto/2;
    this->right_bottom.x=this->right_bottom.x+g.width_auto/2;
    this->right_bottom.y=this->right_bottom.y-g.height_auto/2;

    if (abs(this->left_top.x) > abs(int(this->left_top.x / g.step) * g.step))
        this->left_top.x -= abs(this->left_top.x - int(this->left_top.x / g.step) * g.step);

    if (abs(this->right_bottom.x) > abs(int(this->right_bottom.x / g.step) * g.step))
        this->right_bottom.x +=abs(this->right_bottom.x - int(this->right_bottom.x / g.step) * g.step);

    if (abs(this->left_top.y) > abs(int(this->left_top.y / g.step) * g.step))
        this->left_top.y +=abs(this->left_top.y - int(this->left_top.y / g.step) * g.step);

    if (abs(this->right_bottom.y) > abs(int(this->right_bottom.y / g.step) * g.step))
        this->right_bottom.y -= abs(this->right_bottom.y - int(this->right_bottom.y / g.step) * g.step);

    if (this->left_top.x<=p.x && this->right_bottom.x>=p.x)
        if(this->left_top.y>=p.y && this->right_bottom.y<=p.y)
            return true;
    return false;
}

bool BQuadrAngle::hasVertex(int v, GameMap&g)
{
    int i1=g.GetI(this->left_top.y);
    int i2=g.GetI(this->right_bottom.y);
    int j1=g.GetJ(this->left_top.x);
    int j2=g.GetJ(this->right_bottom.x);
    if(i1<=v/g.num_vertices_width && i2>=v/g.num_vertices_width)
        if(j1<=v%g.num_vertices_width && j2>=v%g.num_vertices_width)
            return true;
    return false;
}

void BQuadrAngle::printToFile(ofstream &out)
{
    out<<x<<endl<<y<<endl;
    out<<left_top<<right_bottom;
    out<<width<<endl<<height<<endl;
}

ofstream& operator<<(ofstream &out, const BQuadrAngle &barrier)
{
    out<<barrier.x<<endl<<barrier.y<<endl;
    out<<barrier.left_top<<barrier.right_bottom;
    out<<barrier.width<<endl<<barrier.height<<endl;
    return out;
}

inline int area (Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  swap (a, b);
    if (c > d)  swap (c, d);
    return max(a,c) <= min(b,d);
}

//bool is_point_in_path(float x, float y, vector<Point>poly)
//     {

//}

class Smoother
{
private:
    static bool inside(Point p, vector<float> plane)
    {
        float d = p.x * plane[0] + p.y * plane[1];
        return d > plane[2];
    }

    static Point clip(Point segStart, Point segEnd, vector<float> plane)
    {
        float d1 = segStart.x * plane[0] + segStart.y * plane[1] - plane[2];
        float d2 = segEnd.x * plane[0] + segEnd.y * plane[1] - plane[2];
        float t = (0 - d1) / (d2 - d1);
        return Point(segStart.x + t * (segEnd.x - segStart.x), segStart.y + t * (segEnd.y - segStart.y));
    }
public:
    static list<Point> Polyclip(list<Point> pin, Point segStart, Point segEnd)
    {
        //if (pin == NULL || pin.Count < 3)
        //    throw new ArgumentException("A polygon was not supplied.");
        vector<float> plane;
        plane.push_back(segStart.y-segStart.y );
        plane.push_back(segEnd.x - segStart.x);
        plane.push_back(0);
        plane[2] = segStart.x * plane[0] + segStart.y * plane[1];
        list<Point> pout;
        Point s = *--pin.end();
        for (auto ci = pin.begin(); ci != pin.end(); ci++)
        {
            Point p = *(ci);
            if (inside(p, plane))
            {
                if (inside(s, plane))
                {
                    pout.push_back(p);
                }
                else
                {
                    Point t = clip(s, p, plane);
                    pout.push_back(t);
                    pout.push_back(p);
                }
            }
            else
            {
                if (inside(s, plane))
                {
                    Point t = clip(s, p, plane);
                    pout.push_back(t);
                }
            }

            s = p;
        }
        return pout;
    }

    static bool SegmentIntersection(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4)
    {
        double Ua, Ub, numerator_a, numerator_b, denominator;

        denominator=(y4-y3)*(x1-x2)-(x4-x3)*(y1-y2);

        if (denominator == 0)
        {
            if ( (x1*y2-x2*y1)*(x4-x3) - (x3*y4-x4*y3)*(x2-x1) == 0 && (x1*y2-x2*y1)*(y4-y3) - (x3*y4-x4*y3)*(y2-y1) == 0)
                return true;
            else
                return false;
        }
        else
        {
            numerator_a=(x4-x2)*(y4-y3)-(x4-x3)*(y4-y2);
            numerator_b=(x1-x2)*(y4-y2)-(x4-x2)*(y1-y2);
            Ua=numerator_a/denominator;
            Ub=numerator_b/denominator;

            if(Ua >=0 && Ua <=1 && Ub >=0 && Ub <=1)
                return true;
            else
                return false;
        }
    }
};



bool BQuadrAngle::isIntersection(Point a, Point b)
{
    /*//нижняя сторона
    Point c, d;
    c.x=left_top.x;
    c.y=right_bottom.y;
    d.x=right_bottom.x;
    d.y=right_bottom.y;

    //левая сторона
    Point c1,d1;
    c1.x=left_top.x;
    c1.y=left_top.y;
    d1.x=left_top.x;
    d1.y=right_bottom.y;

    //верхняя сторона
    Point c2,d2;
    c2.x=left_top.x;
    c2.y=left_top.y;
    d2.x=right_bottom.x;
    d2.y=left_top.y;

    //правая
    Point c3,d3;
    c3.x=right_bottom.x;
    c3.y=left_top.y;
    d3.x=right_bottom.x;
    d3.y=right_bottom.y;*/

    vector<Point>poly;


    poly.push_back(Point(left_top.x,left_top.y));
    poly.push_back(Point(right_bottom.x,left_top.y));
    poly.push_back(Point(right_bottom.x,right_bottom.y));
    poly.push_back(Point(left_top.x,right_bottom.y));

    list<Point> poly1;
    //x-width/2,y+height/2
    poly1.push_back(poly[0]);
    poly1.push_back(poly[1]);
    poly1.push_back(poly[2]);
    poly1.push_back(poly[3]);
    poly1.push_back(poly[0]);
    auto kkk=Smoother::Polyclip(poly1,b,a);
    int seg=0;
    /*bool b1=Smoother::SegmentIntersection(x-width/2,y+height/2, x+width/2,y+height/2,   a.x,a.y,b.x,b.y);
    bool b2=Smoother::SegmentIntersection(x+width/2,y+height/2, x+width/2,y-height/2,   a.x,a.y,b.x,b.y);
    bool b3=Smoother::SegmentIntersection(x-width/2,y-height/2, x+width/2,y-height/2,   a.x,a.y,b.x,b.y);
    bool b4=Smoother::SegmentIntersection(x-width/2,y+height/2, x-width/2,y-height/2,   a.x,a.y,b.x,b.y);*/

    bool b1=Smoother::SegmentIntersection(left_top.x,left_top.y,        right_bottom.x,left_top.y,      a.x,a.y,b.x,b.y);
    bool b2=Smoother::SegmentIntersection(right_bottom.x,left_top.y,    right_bottom.x,right_bottom.y,  a.x,a.y,b.x,b.y);
    bool b3=Smoother::SegmentIntersection(left_top.x,right_bottom.y,    right_bottom.x,right_bottom.y,  a.x,a.y,b.x,b.y);
    bool b4=Smoother::SegmentIntersection(left_top.x,left_top.y,        left_top.x,right_bottom.y,      a.x,a.y,b.x,b.y);

   return b1||b2||b3||b4;

    if(kkk.size()>0)
    return true;
    else
        return false;
}
