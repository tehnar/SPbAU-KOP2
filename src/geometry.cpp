#include "geometry.h"
#include <cmath>
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
    out << "Point(" << p.x << " " << p.y << ")";
    return out;
}

istream& operator >> (istream &in, Point &p)
{
    in >> p.x >> p.y;
    return in;
}

ostream& operator << (ostream &out, const PointDouble &p)
{
    out << "Point(" << p.x << " " << p.y << ")";
    return out;
}

istream& operator >> (istream &in, PointDouble &p)
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

ostream& operator << (ostream &out, const SegmentDouble &segment)
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

bool isInsideBS1(const Point &a, const Polygon &poly)
{
    int l = 0, r = poly.size(), m;
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

int leftTangent(const Point &point, const Polygon &poly)
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
    if ((poly[r] - point) % (poly[r - 1] - point) <= 0)
        return r;
    return l;                                   
}

int rightTangent(const Point &point, const Polygon &poly)
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
    
    if ((point - segment.first) * (segment.second - segment.first) >= 0 && (point - segment.second) * (segment.first - segment.second) >= 0) 
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

inline int findNearestPoint(int left, int right, const Point &point, const Polygon &poly)
{
    if (left >= right)
        right += poly.size();
    #ifdef DEBUG
    int cnt = 0;
    #endif
    while (right - left > 3)
    {
        #ifdef DEBUG
        cnt++;
        assert(1<<(cnt - 1) <= poly.size());
        #endif
        int left1 = (left + right) / 2, right1 = left1 + 1; 
        if (left1 == right1)
            break;
        if ((point - poly[left1]).len2() > (point - poly[right1]).len2())
            left = left1;
        else
            right = right1;
    }
    int result = left;
    for (int i = left + 1; i <= right; i++)
        if ((point - poly[result]).len2() > (point - poly[i]).len2())
            result = i;
    return result;
}

double distance(const Point &point, const Polygon &poly)
{
    int left = leftTangent(point, poly);
    int right = rightTangent(point, poly); 
    int pos = findNearestPoint(left, right, point, poly);
    double result;
    result = min(distance(point, Segment(poly[pos], poly[pos + 1])), distance(point, Segment(poly[pos - 1], poly[pos])));
    pos = findNearestPoint(right, left, point, poly);
    result = min(result, min(distance(point, Segment(poly[pos], poly[pos + 1])), distance(point, Segment(poly[pos - 1], poly[pos]))));
    return result;
}

PointDouble intersection(const Line &first, const Line &second, bool *ok)
{
    if (Point(first.a, first.b) % Point(second.a, second.b) == 0ll)
    {     
        setOk(ok, 0);
        return PointDouble(0, 0);
    }
    setOk(ok, 1);
    
    return PointDouble((double(second.c) * first.b - double(first.c) * second.b) / (double(second.b) * first.a - double(second.a) * first.b), 
                       (double(second.c) * first.a - double(first.c) * second.a) / (double(second.a) * first.b - double(second.b) * first.a));
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

int maximalPoint(const Point &dir, const Polygon &poly)
{
    int l = 0, r = poly.size(), m;
    #ifdef DEBUG
    int cnt = 0;
    #endif
    while (r - l > 1)
    {
        #ifdef DEBUG
        cnt++;
        if (cnt >= 2)
        {
            assert(1<<(cnt - 2) <= poly.size());
        }
        #endif
        int lDir = sign((poly[l] - poly[l - 1]) * dir);
        if (lDir == 0)
        {
            if ((poly[l + 1] - poly[l]) * dir < 0)
                return l;
            else
            {
                l++;
                continue;
            }
        } 
        m = (l + r) / 2;
        if (lDir > 0)
        {
            if ((poly[m] - poly[m - 1]) * dir < 0)
                r = m;
            else if ((poly[m] - poly[l]) * dir >= 0)
                l = m;
            else
                r = m;       
        }
        else
        {
            if ((poly[m] - poly[m - 1]) * dir >= 0)
                l = m;
            else if ((poly[m] - poly[l]) * dir <= 0)
                l = m;
            else
                r = m;
        }
    }    
    return l;
}

int minimalPoint(const Point &dir, const Polygon &poly)
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

SegmentDouble intersection(const Line &line, const Polygon &poly, bool *ok)
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
        int signR = sign(line.normal() * poly[r] + line.c);
        while (r - l > 1)
        {
            m = (l + r) / 2;
            if (signR == sign(line.normal() * poly[m] + line.c))
                r = m;
            else
                l = m;    
        }
        if (isIntersection(line, Segment(poly[l], poly[l + 1])))
            points[cnt++] = intersection(line, Line(poly[l], poly[l + 1]));
        swap(left, right);
    }   

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