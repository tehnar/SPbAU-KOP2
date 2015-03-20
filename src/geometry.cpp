#include "geometry.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;


template <class T> T sqr(const T &a)
{
    return a * a;
}

int sign(long long a)
{
    if (!a)
        return 0;
    return a > 0 ? 1 : -1;
}


Point::Point() : x(0), y(0) {}
Point::Point(int _x, int _y) : x(_x), y(_y) {}

void Point::print(FILE *f) const
{
    fprintf(f, "Point(%d, %d)", x, y);
}   

void Point::read(FILE *f)
{
    fscanf(f, "%d%d", &x, &y);
}

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

double Point::len() const
{
    return sqrt(sqr(1ll * x) + sqr(1ll * y));
}

long long Point::len2() const
{
    return sqr(1ll * x) + sqr(1ll * y);
}

Segment::Segment(){}

Segment::Segment(const Point &a, const Point &b): first(a), second(b){}

void Segment::print(FILE *f) const
{
    fprintf(f, "Segment {Point(%d, %d), Point(%d, %d)}", first.x, first.y, second.x, second.y);
}

Ray::Ray(){}

Ray::Ray(const Point &a, const Point &b): start(a), dir(b){}

void Ray::print(FILE *f) const
{
    fprintf(f, "Ray {Point(%d, %d), (%d, %d)}", start.x, start.y, dir.x, dir.y);
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

Point& Polygon::operator [] (int i) 
{
    return points[(i % (int) points.size() + points.size()) % points.size()];
}

const Point& Polygon::operator [] (int i) const
{
    return points[(i % (int) points.size() + points.size()) % points.size()];
}

int Polygon::size() const
{
    return (int) points.size();
}

void Polygon::print(FILE *f) const
{
    fprintf(f, "Polygon {");
    for (int i = 0; i < size(); i++)
    {
        points[i].print(f);
        if (i != size() - 1)
            fprintf(f, ", ");
    }
    fprintf(f, "}");
}

ostream& operator << (ostream &out, const Point &p)
{
    out << "Point(" << p.x << " " << p.y << ")";
    return out;
}

istream& operator >> (istream &in, Point &p)
{
    in >> p.x >> p.y;
    return in;
}

ostream& operator << (ostream &out, const Polygon &p)
{
    out << "Polygon {";
    for (int i = 0; i < p.size(); i++)
        out << p[i] << ((i != p.size() - 1) ? ", " : "");
    out << "}";
    return out;
}

ostream& operator << (ostream &out, const Ray &ray)
{
    out << "Ray {" << ray.start << ", dir: (" << ray.dir.x << ", " << ray.dir.y << ")}";
    return out;
}

ostream& operator << (ostream &out, const Segment &segment)
{
    out << "Segment {" << segment.first << " " << segment.second << "}";
    return out;
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
        cnt += isIntersection(Ray(a, Point(1e9 + 7, 1e9 + 9)), Segment(poly[i], poly[i + 1]));
    }
    return cnt % 2 == 1;
}

bool isInsideBS1(const Point &a, const Polygon &poly)
{
    int l = 0, r = poly.size(), m;
    while (r - l > 1)
    {
        m = (l + r) / 2;
        if ((a - poly[0]) % (poly[m] - poly[0]) <= 0)
            l = m;
        else
            r = m;
    }
    if (l == poly.size() - 1)
        return isOnSegment(a, Segment(poly[l], poly[0]));
    return (a - poly[0]) % (poly[l] - poly[0]) <= 0 &&
            (a - poly[l]) % (poly[l + 1] - poly[l]) <= 0 &&
            (a - poly[l + 1]) % (poly[0] - poly[l + 1]) <= 0;
}

inline int findIntersection(const Point &a, const Polygon &poly, int from, int to, int signOfCompare)
{
    int l, r, m;
    l = from, r = to;
    if (l >= r)
        r += poly.size();
    while (r - l > 1)
    {
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

int findLeftTangentLinear(const Point &point, const Polygon &poly)
{
    for (int i = 0; i < poly.size(); i++)
    {
        if ((poly[i] - point) % (poly[i - 1] - point) <= 0 && (poly[i + 1] - point) % (poly[i] - point) > 0)               
            return i;
    }
    return 0;
}

int findRightTangentLinear(const Point &point, const Polygon &poly)
{
    for (int i = 0; i < poly.size(); i++)
    {
        if ((poly[i] - point) % (poly[i - 1] - point) >= 0 && (poly[i + 1] - point) % (poly[i] - point) < 0)               
            return i;
    }
    return 0;
}

int findLeftTangentBS(const Point &point, const Polygon &poly)
{
    int l, m, r;
    l = 0, r = poly.size();
    while (r - l > 1)
    {
        m = (l + r) / 2;
        if ((poly[l + 1] - point) % (poly[l] - point) == 0)
        {
            l++;
            continue;
        }
        if ((poly[l + 1] - point) % (poly[l] - point) > 0 && (poly[l] - point) % (poly[l - 1] - point) <= 0)
            return l; 
        if ((poly[l + 1] - point) % (poly[l] - point) < 0)
        {
            if ((poly[m + 1] - point) % (poly[m] - point) > 0)
                r = m;
            else
            {
                if ((poly[l] - point) % (poly[m] - point) < 0)
                    r = m;
                else
                    l = m;
            }
        }
        else
        {
            if ((poly[m + 1] - point) % (poly[m] - point) <= 0)
                l = m;
            else
            {
                if ((poly[l] - point) % (poly[m] - point) >= 0)
                    r = m;
                else
                    l = m;
            }
        }
    }
    cerr << (poly[r] - point) % (poly[r - 1] - point) << endl;        
    if ((poly[r] - point) % (poly[r - 1] - point) <= 0)
        return r;
    return l;                                   
}

int findRightTangentBS(const Point &point, const Polygon &poly)
{
    int l, m, r;
    l = 0, r = poly.size();
    while (r - l > 1)
    {
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

double getDistanceTo(const Point &point, const Polygon &poly)
{

}