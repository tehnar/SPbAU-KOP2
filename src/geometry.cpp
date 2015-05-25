#include "geometry.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;


int sign(long long a)
{
    if (!a)
        return 0;
    return a > 0 ? 1 : -1;
}

inline void setOk(bool *ok, int res)
{
    if (ok)
        *ok = res;
}


Point::Point() : x(0), y(0) {}
Point::Point(int _x, int _y) : x(_x), y(_y) {}

Point Point::operator + (const Point &a) const
{
    return Point(x + a.x, y + a.y);
}

Point Point::operator - (const Point &a) const
{
    return Point(x - a.x, y - a.y);
}

long long Point::operator * (const Point &a) const
{
    return 1ll * x * a.x + 1ll * y * a.y;
}

long long Point::operator % (const Point &a) const
{
    return 1ll * x * a.y - 1ll * y * a.x;
}

Point Point::operator * (int a) const
{
    return Point(x * a, y * a);
}

Point& Point::operator += (const Point &a)
{
    x += a.x;
    y += a.y;
    return *this;
}

Point& Point::operator -= (const Point &a)
{
    x -= a.x;
    y -= a.y;
    return *this;
}

bool Point::operator == (const Point &a) const
{
    return x == a.x && y == a.y;
}

bool Point::operator != (const Point &a) const
{
    return x != a.x || y != a.y;
}

Point::operator PointDouble() const
{
    return PointDouble(x, y);
}

double Point::len() const
{
    return sqrt(sqr(1ll * x) + sqr(1ll * y));
}

long long Point::len2() const
{
    return sqr(1ll * x) + sqr(1ll * y);
}

PointDouble::PointDouble() : x(0), y(0) {}
PointDouble::PointDouble(double _x, double _y) : x(_x), y(_y) {}

bool PointDouble::operator == (const PointDouble &a) const
{
    return abs(x - a.x) < eps && abs(y - a.y) < eps;
}

bool PointDouble::operator != (const PointDouble &a) const
{
    return abs(x - a.x) >= eps || abs(y - a.y) >= eps;
}

Segment::Segment(){}

Segment::Segment(const Point &a, const Point &b): first(a), second(b){}

bool Segment::operator == (const Segment &other) const
{
    return (first == other.first && second == other.second) ||
           (first == other.second && second == other.first);
}

SegmentDouble::SegmentDouble(){}

SegmentDouble::SegmentDouble(const PointDouble &a, const PointDouble &b): first(a), second(b){}

bool SegmentDouble::operator == (const SegmentDouble &other) const
{
    return (first == other.first && second == other.second) ||
           (first == other.second && second == other.first);
}

Ray::Ray(const Point &a, const Point &b): start(a), dir(b){}

Line::Line(const Point &first, const Point &second)
{
    a = second.y - first.y;
    b = first.x - second.x;
    c = 1ll * first.y * second.x - 1ll * first.x * second.y;
}

Point Line::normal() const
{
    return Point(a, b);
}

Point Line::direction() const
{
    return Point(-b, a);
}

Polygon::Polygon(const vector <Point> &p) : points(p) 
{
    if ((points[2] - points[0]) % (points[1] - points[0]) > 0)
        reverse(points.begin(), points.end());
    upmost = downmost = 0;
    for (int i = 0; i < (int) points.size(); i++)
    {
        if (points[i].y > points[upmost].y || (points[i].y == points[upmost].y && points[i].x > points[upmost].x))
            upmost = i;
        if (points[i].y < points[downmost].y || (points[i].y == points[downmost].y && points[i].x > points[downmost].x))
            downmost = i;
    }
}

const Point& Polygon::operator [] (int i) const
{
    return points[(i % (int) points.size() + points.size()) % points.size()];
}

int Polygon::size() const
{
    return (int) points.size();
}

ostream& operator << (ostream &out, const Point &p)
{
    out << "point " << p.x << " " << p.y << "\n";
    return out;
}

istream& operator >> (istream &in, Point &p)
{
    in >> p.x >> p.y;
    return in;
}

ostream& operator << (ostream &out, const PointDouble &p)
{
    out << "point " << p.x << " " << p.y << "\n";
    return out;
}

istream& operator >> (istream &in, PointDouble &p)
{
    in >> p.x >> p.y;
    return in;
}

ostream& operator << (ostream &out, const Polygon &p)
{
    for (int i = 0; i < p.size(); i++)
        out << Segment(p[i], p[i + 1]);
    return out;
}

ostream& operator << (ostream &out, const Ray &ray)
{
    out << "ray " << ray.start << " " << ray.dir << "\n";
    return out;
}

ostream& operator << (ostream &out, const Segment &segment)
{
    out << "segment " << segment.first << " " << segment.second << "\n";
    return out;
}

ostream& operator << (ostream &out, const SegmentDouble &segment)
{
    out << "segment " << segment.first << " " << segment.second << "\n";
    return out;
}

void drawPoint(FILE *out, const Point& point, const char *name)
{
    fprintf(out, "point %d %d", point.x, point.y);
    if (strlen(name))
        fprintf(out, " name=%s", name); 
    fprintf(out, "\n");
}   

void drawPoint(FILE *out, const PointDouble& point, const char *name)
{
    fprintf(out, "point %f %f", point.x, point.y);
    if (strlen(name))
        fprintf(out, " name=%s", name); 
    fprintf(out, "\n");
}   

void drawSegment(FILE *out, const Point &from, const Point &to, const char* color, const char *name)
{
    fprintf(out, "segment %d %d %d %d color=%s", from.x, from.y, to.x, to.y, color); 
    if (strlen(name))
        fprintf(out, " name=%s", name); 
    fprintf(out, "\n");
}

void drawVector(FILE *out, const Point &from, const Point &to, const char* color, const char *name)
{
    fprintf(out, "vector %d %d %d %d color=%s", from.x, from.y, to.x, to.y, color); 
    if (strlen(name))
        fprintf(out, " name=%s", name); 
    fprintf(out, "\n");
}

void drawRay(FILE *out, const Point &from, const Point &dir, const char* color, const char *name)
{
    fprintf(out, "ray %d %d %d %d color=%s", from.x, from.y, dir.x, dir.y, color);
    if (strlen(name))
        fprintf(out, "name=%s", name); 
    fprintf(out, "\n");
}

void drawLine(FILE *out, const Line &line, const char* color)
{
    fprintf(out, "line %d %d %I64d color=%s\n", line.a, line.b, line.c, color);
}

void drawAngle(FILE *out, const Point &vertex, const Point &first, const Point &second, bool withArc)
{
    fprintf(out, "angle %d %d %d %d %d %d arc=%d\n", vertex.x, vertex.y, first.x, first.y, second.x, second.y, withArc);        
}

void drawText(FILE *out, const char *text)
{
    fprintf(out, "print %s\n", text);
}

void endOfSlide(FILE *out)
{
    fprintf(out, "end\n");
}

void endOfStep(FILE *out)
{
    fprintf(out, "wait\n");
}

bool isConvex(const Polygon &poly)
{
    int sgn = sign((poly[2] - poly[1]) % (poly[1] - poly[0]));
    if (sgn == 0)
        return 0;
    for (int i = 0; i < poly.size(); i++)
        if (sign((poly[i + 2] - poly[i + 1]) % (poly[i + 1] - poly[i])) != sgn)
            return 0;
    return 1;
}

Polygon genLargePolygon(int vcnt, double radius)                                               
{
    vector <Point> result;
    while (result.size() == 0 || !isConvex(result))
    {
        result.clear();
        double dx = max(0.5, (double) rand() / RAND_MAX * 2);
        double dy = max(0.5, (double) rand() / RAND_MAX * 2);
        for (int i = 0; i < vcnt; i++)
        {
            double angle = (double) rand() / RAND_MAX * 2 * 3.1415;
            result.push_back(Point(int(cos(angle) * radius * dx), int(sin(angle) * radius * dy)));    
        }
        sort(result.begin(), result.end(), compareByAngle);
    }
    return result;
}

bool isOnSegment(const Point &a, const Segment &segment)
{
    return (a - segment.first) % (segment.second - segment.first) == 0 &&
            (a - segment.first) * (segment.second - segment.first) >= 0 &&
            (a - segment.second) * (segment.first - segment.second) >= 0;
}

bool isIntersection(const Ray &ray, const Segment &segment)
{
    if (sign(ray.dir % (segment.first - ray.start)) * sign(ray.dir % (segment.second - ray.start)) <= 0)
        return sign((ray.start - segment.first) % (segment.second - segment.first)) * sign(ray.dir % (segment.second - segment.first)) <= 0;
    else
        return 0;
}

bool isIntersection(const Line &line, const Segment &segment)
{
    return sign(segment.first * line.normal() + line.c) * sign(segment.second * line.normal() + line.c) <= 0;
}

bool isInsideLinear(const Point &a, const Polygon &poly)
{
    int cnt = 0;
    for (int i = 0; i < poly.size(); i++)
    {
        if (a.y == poly[i].y && a.y == poly[i + 1].y)
            return isOnSegment(a, Segment(poly[i], poly[i + 1]));
        if (isOnSegment(a, Segment(poly[i], poly[i + 1])))
            return 1;
        if (a == poly[i])
            return 1;
        cnt += isIntersection(Ray(a, Point(1<<30, (1<<30) - 1)), Segment(poly[i], poly[i + 1]));
    }
    return cnt % 2 == 1;
}

inline void drawBinSearchState(FILE *out, const Polygon &poly, int l, int m, int r)
{
    if (l != -1)
        drawPoint(out, poly[l], "l");
    
    if (r != -1)            
        drawPoint(out, poly[r], "r");

    if (m != -1)
        drawPoint(out, poly[m], "m");

    for (int i = 0; i < poly.size(); i++)
    {
        if (i >= l && i < r)
            drawSegment(out, poly[i], poly[i + 1], "red");
        else
            drawSegment(out, poly[i], poly[i + 1], "black");
        drawPoint(out, poly[i]);
    }
}

inline void drawIsInsideState(FILE *out, const Point &point, const Polygon &poly, int l, int m, int r)
{
    drawBinSearchState(out, poly, l, m, r);
    drawPoint(out, point, "point");
    if (l != -1)
        drawSegment(out, poly[0], poly[l], "red");
    if (r != -1)
        drawSegment(out, poly[0], poly[r], "red");
}

bool isInside(const Point &point, const Polygon &poly, FILE *out)
{
    int l = 0, r = poly.size(), m;
    #ifdef DEBUG
    int cnt = 0;
    #endif        
    if (out)
    {
        drawIsInsideState(out, point, poly, -1, -1, -1);
        drawText(out, "Проверка на принадлежность точки многоугольнику\nwait");
        drawText(out, "Хотим найти две вершины такие, что точка лежит между векторами,\\n\
проведенными из первой вершины в эти две"); 
        endOfStep(out);
        drawText(out, "Будем искать бинпоиском ближайшую к данной точке по углу вершину.");
        endOfSlide(out);                                                                                         
        drawIsInsideState(out, point, poly, l, -1, r);
        drawText(out, "Изначально границы бинпоиска покрывают весь многоугольник.");
        endOfSlide(out);
    }
    while (r - l > 1)
    {
        #ifdef DEBUG
        cnt++;
        assert((1<<(cnt - 1)) <= poly.size());
        #endif
        m = (l + r) / 2;
        if (out)
        {
            drawIsInsideState(out, point, poly, l, m, r);
            endOfStep(out);            
            drawText(out, "Смотрим на векторное произведение вектора из нулевой точки многоугольника до\\nвершины m и вектора до данной точки");
            drawAngle(out, poly[0], point, poly[m], 1);
            endOfStep(out);                        
        }
        if ((point - poly[0]) % (poly[m] - poly[0]) <= 0)
        {
            if (out)
                drawText(out, "Так как точка лежит правее вершины (по обходу против часовой стрелки),\\n\
то двигаем левый указатель");
            l = m;
        }
        else
        {
            if (out)
                drawText(out, "Так как точка лежит левее вершины (по обходу против часовой стрелки),\\n\
то двигаем правый указатель");
            r = m;
        }
        if (out)
            endOfSlide(out);
    }
    if (out)
    {
        drawIsInsideState(out, point, poly, l, -1, r);        
        endOfStep(out);
    }
    bool result;
    if (l == poly.size() - 1)
        result = isOnSegment(point, Segment(poly[l], poly[0]));
    else
        result = (point - poly[0]) % (poly[l] - poly[0]) <= 0 && 
        (point - poly[l]) % (poly[l + 1] - poly[l]) <= 0 &&
        (point - poly[l + 1]) % (poly[0] - poly[l + 1]) <= 0;
    if (out)
    {
        if (result)
            drawText(out, "Так как точка лежит внутри треугольника, то она лежит и внутри многоугольника");
        else
            drawText(out, "Так как точка лежит вне треугольника, то она лежит и снаружи многоугольника");
    }
}

inline int findIntersection(const Point &a, const Polygon &poly, int from, int to, int signOfCompare)
{
    int l, r, m;
    l = from, r = to;
    if (l >= r)
        r += poly.size();
    #ifdef DEBUG
    int cnt = 0;
    #endif
    while (r - l > 1)
    {
        #ifdef DEBUG
        cnt++;
        assert((1<<(cnt - 1)) <= poly.size());
        #endif
        m = (l + r) / 2;
        if (poly[m].y * signOfCompare >= a.y * signOfCompare)
            l = m;
        else
            r = m;
    }
    if (poly[l] == a)
        return 3;
    if (poly[l].y == a.y && poly[l + 1].y == a.y)
        return isOnSegment(a, Segment(poly[l], poly[l + 1])) ? 3 : -3;
    if (isOnSegment(a, Segment(poly[l], poly[l + 1])))
        return 3;
    return isIntersection(Ray(a, Point(1, 0)), Segment(poly[l], poly[l + 1]));        
} 

bool isInsideBS2(const Point &a, const Polygon &poly)
{
    int cnt = findIntersection(a, poly, poly.upmost, poly.downmost, 1) +
              findIntersection(a, poly, poly.downmost, poly.upmost, -1);
    if (cnt >= 3)
        return 1;
    return cnt == 1;
}

bool compareByAngle(const Point &first, const Point &second) 
{
    if (sign(first.y) * sign(second.y) < 0)
        return first.y == second.y ? first.x < second.x : first.y < second.y;
    return first.y == 0 && second.y == 0 ? first.x < second.x : first % second > 0;
}

int leftTangentLinear(const Point &point, const Polygon &poly)
{
    for (int i = 0; i < poly.size(); i++)
    {
        if ((poly[i] - point) % (poly[i - 1] - point) <= 0 && (poly[i + 1] - point) % (poly[i] - point) > 0)               
            return i;
    }
    return 0;
}

int rightTangentLinear(const Point &point, const Polygon &poly)
{
    for (int i = 0; i < poly.size(); i++)
    {
        if ((poly[i] - point) % (poly[i - 1] - point) >= 0 && (poly[i + 1] - point) % (poly[i] - point) < 0)               
            return i;
    }
    return 0;
}

inline void drawTangentState(FILE *out, const Point &point, const Polygon &poly, int l, int m, int r, int ang1 = -1, int ang2 = -1)
{
    drawBinSearchState(out, poly, l, m, r);
    drawPoint(out, point, "point");
    if (ang1 != -1 && ang2 != -1)
        drawAngle(out, point, poly[ang1], poly[ang2], 1);                                    
}

int leftTangent(const Point &point, const Polygon &poly, FILE *out)
{
    int l, m, r;
    l = 0, r = poly.size();
    #ifdef DEBUG
    int cnt = 0;
    #endif
    
    if (out)
    {
        drawTangentState(out, point, poly, -1, -1, -1);
        drawText(out, "Поиск левой касательной из точки к многоугольнику");
        endOfStep(out);
        drawText(out, "Точка касания это такая вершина многоугольника\\n\
вектор из данной точки до которой имеет наибольший угол");
        endOfStep(out);
        drawText(out, "Поскольку функция угла является выпуклой, ее максимум\\n\
можно найти бинпоиском по производной");
        endOfSlide(out);
        drawTangentState(out, point, poly, l, -1, r);
        drawText(out, "Изначально границы бинпоиска покрывают весь многоугольник.");
        endOfSlide(out);
    }
    
    while (r - l > 1)
    {
        #ifdef DEBUG
        cnt++;
        if (cnt >= 3)
            assert(1<<(cnt - 3) <= poly.size());
        #endif
        m = (l + r) / 2;
        drawTangentState(out, point, poly, l, m, r);
        if ((poly[l + 1] - point) % (poly[l] - point) == 0)
        {
            l++;
            if (out)
            {
                drawText(out, "Так как угол равен нулю, то, чтобы избежать крайних случаев,\\n\
двигаем левый указатель на 1");
                endOfSlide(out);
            }
            continue;
        }
        if ((poly[l + 1] - point) % (poly[l] - point) > 0 && (poly[l] - point) % (poly[l - 1] - point) <= 0)
        {
            if (out)
            {
                drawText(out, "Поскольку вершина, на которую указывает l, является точкой касания,\\n\
то выходим из цикла бинпоиска");
                endOfSlide(out);
                break;
            }
        }
        if (out)
        {
            endOfStep(out);
            drawTangentState(out, point, poly, l, m, r, l, l + 1);
            drawText(out, "Смотрим на знак производной");
            endOfStep(out);
        }
        if ((poly[l + 1] - point) % (poly[l] - point) < 0)
        {
            if (out)
            {
                drawText(out, "Она больше нуля. Теперь посмотрим на знак производной в вершине m");
                endOfSlide(out);
                drawTangentState(out, point, poly, l, m, r, m, m + 1);
            }
            if ((poly[m + 1] - point) % (poly[m] - point) > 0)
            {
                if (out)
                {
                    drawText(out, "Так как в вершине m она меньше нуля, то двигаем правую границу");
                    endOfSlide(out);
                }
                r = m;
            }
            else
            {
                if (out)
                {
                    drawText(out, "Она больше нуля, поэтому надо сравнить углы до вершин l и m");
                    endOfSlide(out);
                    drawTangentState(out, point, poly, l, m, r, l, m);
                }
                if ((poly[l] - point) % (poly[m] - point) < 0)
                {
                    if (out)
                        drawText(out, "Так как вершина l \"выше\" вершина m, то двигаем правый указатель");
                    r = m;
                }
                else
                {
                    if (out)
                        drawText(out, "Так как вершина m \"выше\" вершина l, то двигаем левый указатель");
                    l = m;
                }
                endOfSlide(out);
            }
        }
        else
        {
            if (out)
            {
                drawText(out, "Она меньше нуля. Теперь посмотрим на знак производной в вершине m");
                endOfSlide(out);
                drawTangentState(out, point, poly, l, m, r, m, m + 1);
            }
            if ((poly[m + 1] - point) % (poly[m] - point) <= 0)
            {
                if (out)
                {
                    drawText(out, "Так как в вершине m она больше нуля, то двигаем левую границу");
                    endOfSlide(out);
                }
                l = m;
            }
            else
            {
                if (out)
                {
                    drawText(out, "Она меньше нуля, поэтому надо сравнить углы до вершин l и m");
                    endOfSlide(out);
                    drawTangentState(out, point, poly, l, m, r, l, m);
                }
                if ((poly[l] - point) % (poly[m] - point) >= 0)
                {
                    if (out)
                        drawText(out, "Так как вершина l \"ниже\" вершина m, то двигаем правый указатель");
                    r = m;
                }
                else
                {
                    if (out)
                        drawText(out, "Так как вершина l \"выше\" вершина m, то двигаем левый указатель");
                    l = m;
                }
                endOfSlide(out);
            }
        }
    }
    if (out)
    {                                              
        drawTangentState(out, point, poly, -1, -1, -1);
        drawText(out, "Результат");
    }
    if ((poly[r] - point) % (poly[r - 1] - point) <= 0)
    {
        if (out)
            drawRay(out, point, poly[r] - point, "blue");
        return r;
    }
    else
    {
        if (out)
            drawRay(out, point, poly[l] - point, "blue");
        return l;                                   
    }
}

int rightTangent(const Point &point, const Polygon &poly, FILE *out)
{
    int l, m, r;
    l = 0, r = poly.size();
    #ifdef DEBUG
    int cnt = 0;
    #endif
    while (r - l > 1)
    {
        #ifdef DEBUG
        cnt++;
        if (cnt >= 3)
            assert(1<<(cnt - 3) <= poly.size());
        #endif
        m = (l + r) / 2;
        if ((poly[l + 1] - point) % (poly[l] - point) == 0)
        {
            l++;
            continue;
        }
        if ((poly[l + 1] - point) % (poly[l] - point) < 0 && (poly[l] - point) % (poly[l - 1] - point) >= 0)
            return l; 
        if ((poly[l + 1] - point) % (poly[l] - point) < 0)
        {
            if ((poly[m + 1] - point) % (poly[m] - point) >= 0)
                l = m;
            else
            {
                if ((poly[l] - point) % (poly[m] - point) <= 0)
                    r = m;
                else
                    l = m;
            }
        }
        else
        {
            if ((poly[m + 1] - point) % (poly[m] - point) < 0)
                r = m;
            else
            {
                if ((poly[l] - point) % (poly[m] - point) >= 0)
                    r = m;
                else
                    l = m;
            }
        }
    }                 
    if ((poly[r] - point) % (poly[r - 1] - point) >= 0)
        return r;
    return l;                                   
}

double distance(const Point &point, const Segment &segment)
{
    double result = min((point - segment.first).len(), (point - segment.second).len());     
    
    if ((point - segment.first) * (segment.second - segment.first) >= 0 && 
            (point - segment.second) * (segment.first - segment.second) >= 0) 
        result = min(result, abs(double((segment.first - point) % (segment.second - point))) / (segment.first - segment.second).len());
    return result;
}

double distanceLinear(const Point &point, const Polygon &poly)
{
    double result = 1e18;
    for (int i = 0; i < poly.size(); i++)
        result = min(result, distance(point, Segment(poly[i], poly[i + 1])));      
    return result;
}

inline void drawDistanceState(FILE *out, const Point &point, const Polygon &poly, int l, int m, int r, 
                              int ltangent, int rtangent)
{
    drawBinSearchState(out, poly, l, m, r);
    drawPoint(out, point, "point");
    if (ltangent != -1)
        drawRay(out, point, poly[ltangent] - point, "red");
    if (rtangent != -1)
        drawRay(out, point, poly[rtangent] - point, "red");
}

double distance(const Point &point, const Polygon &poly, FILE *out)
{
    if (out)
    {
        drawDistanceState(out, point, poly, -1, -1, -1, -1, -1);
        drawText(out, "Хотим посчитать расстояние от точки до многоугольника");
        endOfStep(out);
        drawText(out, "Для этого сначала найдем точки касательных к этому многоугольнику");
        endOfSlide(out);
    }
    int ltangent = leftTangent(point, poly);
    int rtangent = rightTangent(point, poly); 

    if (out)
    {
        drawDistanceState(out, point, poly, -1, -1, -1, ltangent, rtangent);
        drawText(out, "Теперь хотим найти вершину многоугольника, которая ближе всего к данной точке.\\n\
Это можно сделать бинпоиском по производной функции расстояния"); 
        endOfSlide(out);
    }

    int left = ltangent, right = rtangent;
    
    if (left >= right)
        right += poly.size();
    #ifdef DEBUG
    int cnt = 0;
    #endif
    if (out)
    {
        drawDistanceState(out, point, poly, left, -1, right, ltangent, rtangent);
        drawText(out, "Исходные границы - часть многоугольника от точки левой касательной до\\n\
точки правой касательной в порядке обхода против часовой стрелки");    
        endOfStep(out);
        drawText(out, "Чтобы избежать каких-то крайних случаев, будем делать бинпоиск пока разность между\\n\
двумя указателями больше трех, остаток разберем руками.");
        endOfSlide(out);
    }

    while (right - left > 3)
    {
        #ifdef DEBUG
        cnt++;
        assert(1<<(cnt - 1) <= poly.size());
        #endif
        int left1 = (left + right) / 2, right1 = left1 + 1; 
        if (left1 == right1)
            break;
        if (out)
        {
            drawDistanceState(out, point, poly, left, left1, right, ltangent, rtangent); 
            drawSegment(out, point, poly[left1], "blue");
            drawSegment(out, point, poly[right1], "blue");
            drawText(out, "Посмотрим на расстояние до вершины m и m+1");
            endOfStep(out);
        }
        if ((point - poly[left1]).len2() > (point - poly[right1]).len2())
        {
            if (out)
                drawText(out, "Так как расстояние до вершины m больше, то двигаем левый указатель");
            left = left1;
        }
        else
        {
            if (out)
                drawText(out, "Так как расстояние до вершины m меньше, то двигаем правый указатель");
            right = right1;
        }
        if (out)
            endOfSlide(out);
    }


    int pos = left;
    for (int i = left + 1; i <= right; i++)
        if ((point - poly[pos]).len2() > (point - poly[i]).len2())
            pos = i;
    
    if (out)
    {   
        drawDistanceState(out, point, poly, -1, pos, -1, -1, -1);
        drawText(out, "Самая близкая вершина к данной точке. Теперь надо посмотреть на две стороны многоугольника,\\n\
соседние с ней, посчитать расстояния до двух отрезков и взять минимум");
    }        
    return min(distance(point, Segment(poly[pos], poly[pos + 1])), distance(point, Segment(poly[pos - 1], poly[pos])));
}

PointDouble intersection(const Line &first, const Line &second, bool *ok, FILE *out)
{
    if (Point(first.a, first.b) % Point(second.a, second.b) == 0ll)
    {     
        setOk(ok, 0);
        return PointDouble(0, 0);
    }
    setOk(ok, 1);
    
    return PointDouble((double(second.c) * first.b - double(first.c) * second.b) / 
                       (double(second.b) * first.a - double(second.a) * first.b), 
                       (double(second.c) * first.a - double(first.c) * second.a) / 
                       (double(second.a) * first.b - double(second.b) * first.a));
}

int maximalPointLinear(const Point &dir, const Polygon &poly)
{
    int result = 0;
    for (int i = 0; i < poly.size(); i++)
        if (poly[i] * dir > poly[result] * dir)
            result = i;
    return result;
}

int minimalPointLinear(const Point &dir, const Polygon &poly)
{
    int result = 0;
    for (int i = 0; i < poly.size(); i++)
        if (poly[i] * dir < poly[result] * dir)
            result = i;
    return result;
}

inline void drawMaximalPointState(FILE *out, const Point &dir, const Polygon &poly, int l, int m, int r)
{
    drawBinSearchState(out, poly, l, m, r);
    drawRay(out, poly[0], dir, "blue", "direction");        
}

int maximalPoint(const Point &dir, const Polygon &poly, FILE *out)
{
    int l = 0, r = poly.size(), m;
    if (out)
    {
        drawMaximalPointState(out, dir, poly, -1, -1, -1);
        drawText(out, "Хотим найти вершину многоугольника, имеющую максимальную проекцию на вектор direction");
        endOfSlide(out);
        drawMaximalPointState(out, dir, poly, l, -1, r);
        drawText(out, "Будем делать бинпоиск, исходные границы покрывают весь многоугольник");
        endOfSlide(out);
    }
    
    #ifdef DEBUG
    int cnt = 0;
    #endif
    
    while (r - l > 1)
    {
        #ifdef DEBUG
        cnt++;
        if (cnt >= 2)
            assert(1<<(cnt - 2) <= poly.size());
        #endif
        m = (l + r) / 2;
        if (out)
        {
            drawMaximalPointState(out, dir, poly, l, m, r);
            endOfStep(out);
            drawText(out, "Посмотрим на знак скалярного произведения вектора из вершины l-1 в вершину l\\n\
и вектора направления");
            drawAngle(out, poly[l - 1], poly[l - 1] + dir, poly[l]);
            endOfStep(out);
        }
        int lDir = sign((poly[l] - poly[l - 1]) * dir);
        if (lDir == 0)
        {
            if ((poly[l + 1] - poly[l]) * dir < 0)
            {
                if (out)
                {
                    drawText(out, "Поскольку оно равно нулю и при этом скалярное произведение вектора направления и следующей стороны\\n\
меньше нуля, то эта вершина имеет максимальную проекцию");
                    endOfSlide(out);
                }
                break;
            }
            else
            {
                if (out)
                {
                    drawText(out, "Поскольку оно равно нулю и при этом скалярное произведение вектора направления и следующей стороны\\n\
больше нуля, то просто двигаем левый указатель на 1 вправо");
                    endOfSlide(out);
                }
                l++;
                continue;
            }
        } 
        if (lDir > 0)
        {
            if (out)
            {
                drawMaximalPointState(out, dir, poly, l, m, r);
                drawText(out, "Оно больше нуля. Теперь посмотрим на скалярное произведение вектора направления и\\n\
вектора из точки m-1 в точку m");
                drawAngle(out, poly[m - 1], poly[m - 1] + dir, poly[m]);
                endOfStep(out);
            }
            if ((poly[m] - poly[m - 1]) * dir < 0)
            {
                if (out)
                    drawText(out, "Так как оно меньше нуля, то двигаем правый указатель");
                r = m;
            }
            else if ((poly[m] - poly[l]) * dir >= 0)
            {
                if (out)
                    drawText(out, "Так как оно больше нуля, и при этом вершина m находится дальше по направлению, то\\n\
двигаем левый указатель");                
                l = m;
            }
            else
            {
                if (out)
                    drawText(out, "Так как оно больше нуля, и при этом вершина l находится дальше по направлению, то\\n\
двигаем правый указатель");            
                r = m;       
            }            
            endOfSlide(out);
        }
        else
        {
            if (out)
            {
                drawMaximalPointState(out, dir, poly, l, m, r);
                drawText(out, "Оно меньше нуля. Теперь посмотрим на скалярное произведение вектора направления и\\n\
вектора из точки m-1 в точку m");
                drawAngle(out, poly[m - 1], poly[m - 1] + dir, poly[m]);
                endOfStep(out);
            }
            if ((poly[m] - poly[m - 1]) * dir >= 0)
            {
                if (out)
                    drawText(out, "Так как оно больше нуля, то двигаем левый указатель");
                l = m;
            }
            else if ((poly[m] - poly[l]) * dir <= 0)
            {
                if (out)
                    drawText(out, "Так как оно меньше нуля, и при этом вершина l находится дальше по направлению, то\\n\
двигаем левый указатель");                
                l = m;
            }
            else
            {
                if (out)
                    drawText(out, "Так как оно больше нуля, и при этом вершина m находится дальше по направлению, то\\n\
двигаем левый указатель");                
                r = m;                                  
            }
            endOfSlide(out);
        }
    }    
    drawMaximalPointState(out, dir, poly, -1, -1,- 1);
    drawPoint(out, poly[l], "result");
    return l;
}

int minimalPoint(const Point &dir, const Polygon &poly, FILE *out)
{
    return maximalPoint(Point(-dir.x, -dir.y), poly);
}

SegmentDouble intersectionLinear(const Line &line, const Polygon &poly, bool *ok)
{
    bool found = 0;
    SegmentDouble result; 
    for (int i = 0; i < poly.size(); i++)
    {
        if (line.direction() % (poly[i + 1] - poly[i]) == 0 && (line.normal() * poly[i] + line.c) == 0)
        {
            setOk(ok, 1);
            return SegmentDouble(poly[i], poly[i + 1]);
        }
        if (isIntersection(line, Segment(poly[i], poly[i + 1])))
        {
            if (!found)            
            {
                result.first = result.second = intersection(line, Line(poly[i], poly[i + 1]));
                found = 1;
            }
            else
            {
                PointDouble tmp = intersection(line, Line(poly[i], poly[i + 1]));
                if (tmp != result.first)
                    result.second = tmp;
            }
        }
    }
    setOk(ok, found);
    return result;
}

inline void drawIntersectionState(FILE *out, const Line &line, const Polygon &poly, int l, int m, int r, 
        const PointDouble *points, int cnt)
{
    drawBinSearchState(out, poly, l, m, r);
    drawLine(out, line, "red");
    if (cnt >= 1)
        printf("draw point %f %f name=first\n", points[0].x, points[0].y);
    if (cnt >= 2)
        printf("draw point %f %f name=second\n", points[1].x, points[1].y);
    puts("wait");
}

SegmentDouble intersection(const Line &line, const Polygon &poly, bool *ok, FILE *out)
{
    int left = minimalPoint(line.normal(), poly), right = maximalPoint(line.normal(), poly);
    int cnt = 0;
    PointDouble points[2];

    for (int i = 0; i < 2; i++)
    {
        if (poly[left] * line.normal() + line.c == 0)
        {         
            setOk(ok, 1);
            if (poly[left - 1] * line.normal() + line.c == 0)
                return SegmentDouble(poly[left - 1], poly[left]);
            else if (poly[left + 1] * line.normal() + line.c == 0)
                return SegmentDouble(poly[left], poly[left + 1]);
            else
                return SegmentDouble(poly[left], poly[left]);
        }
        swap(left, right);
    }      

    #ifdef DEBUG
    int counter = 0;
    #endif
    for(int i = 0; i < 2; i++)
    {
        #ifdef DEBUG
        counter++;
        assert(1<<((counter - 1) / 2) <= poly.size());
        #endif
        int l = left, r = right, m;
        if (l >= r)
            r += poly.size();
        drawIntersectionState(out, line, poly, l, -1, r, points, cnt); 
        int signR = sign(line.normal() * poly[r] + line.c);
        while (r - l > 1)
        {
            m = (l + r) / 2;
            drawIntersectionState(out, line, poly, l, m, r, points, cnt); 
            if (signR == sign(line.normal() * poly[m] + line.c))
                r = m;
            else
                l = m;    
        }
        if (isIntersection(line, Segment(poly[l], poly[l + 1])))
            points[cnt++] = intersection(line, Line(poly[l], poly[l + 1]));
        swap(left, right);
    }   
    drawIntersectionState(out, line, poly, -1, -1, -1, points, cnt);

    if (cnt == 0)
    {
        setOk(ok, 0);
        return SegmentDouble(PointDouble(0, 0), PointDouble(0, 0));
    }
    else
    {
        setOk(ok, 1);
        return SegmentDouble(points[0], points[1]);                        
    }
}
