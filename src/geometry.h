#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <ostream>
#include <istream>
#include <cstdio>
#include <vector>
#include <cassert>

int sign(long long a);
template <class T> T sqr(const T &a);

const double eps = 1e-8;
struct PointDouble;

// Structure that represents point or vector on plane.
// Coordinates of point should not exceed 2^31 by an
// absolute value
struct Point
{   
    int x, y;

    Point();
    Point(int x, int y);

    Point operator + (const Point &rhs) const;
    Point operator - (const Point &rhs) const;
    long long operator * (const Point &rhs) const;
    long long operator % (const Point &rhs) const;
    Point operator * (int a) const;

    Point& operator += (const Point &rhs);
    Point& operator -= (const Point &rhs);
    Point& operator *= (int a);
    
    operator PointDouble() const;

    bool operator == (const Point &rhs) const;
    bool operator != (const Point &rhs) const;

    double len() const;
    long long len2() const;

};

struct PointDouble
{
    double x, y;
    PointDouble();
    PointDouble(double x, double y);

    // Checks if two points are equal with absolute error 1e-8
    bool operator == (const PointDouble &other) const;
    bool operator != (const PointDouble &other) const;
};

// Structure that stores polygon in counter-clockwise order.
// This polygon should be convex otherwise result of operations 
// with it is undefined
struct Polygon
{
    Polygon(const std::vector <Point> &points);
    Point& operator [] (int i);
    const Point& operator [] (int i) const;
    void print(FILE *f = stdout) const;
    int size() const;
    int downmost, upmost;
    std::vector <Point> points;
};

struct Line
{
    Line(const Point &first, const Point &second);
    Point normal() const;
    Point direction() const;
    int a, b;
    long long c;
};

struct Segment
{
    Segment();
    Segment(const Point &first, const Point &second);
    bool operator == (const Segment &other) const;

    Point first, second;
};

struct SegmentDouble
{
    SegmentDouble();
    SegmentDouble(const PointDouble &first, const PointDouble &second);
    bool operator == (const SegmentDouble &other) const;

    PointDouble first, second;
};

struct Ray
{
    Ray(const Point &start, const Point &dir);
    Point start, dir;
};

std::ostream& operator << (std::ostream &out, const Point &p);
std::istream& operator >> (std::istream &in, const Point &p);
std::ostream& operator << (std::ostream &out, const PointDouble &p);
std::istream& operator >> (std::istream &in, const PointDouble &p);
std::ostream& operator << (std::ostream &out, const Polygon &p);
std::ostream& operator << (std::ostream &out, const Ray &ray);
std::ostream& operator << (std::ostream &out, const Segment &segment);
std::ostream& operator << (std::ostream &out, const SegmentDouble &segment);


// Compares points by angle with the horizontal axis. 
// The angle lies in [-PI, PI)
bool compareByAngle(const Point &first, const Point &second);

bool isInsideLinear(const Point &point, const Polygon &poly);
bool isInsideBS1(const Point &point, const Polygon &poly);
bool isInsideBS2(const Point &point, const Polygon &poly);

// Returns index of point in polygon such as line formed by this point
// and point "from" is left tangent to given polygon. If there exists
// several such points then this function returns the most distant point.
// If point "from" is inside polygon then returning value is undefined.
int findLeftTangentLinear(const Point &from, const Polygon &poly);
int findLeftTangentBS(const Point &from, const Polygon &poly);

// Returns index of point in polygon such as line formed by this point
// and point "from" is right tangent to given polygon. If there exists
// several such points then this function returns the most distant point 
// If point "from" is inside polygon then returning value is undefined.
int findRightTangentLinear(const Point &from, const Polygon &poly);
int findRightTangentBS(const Point &from, const Polygon &poly);

bool isOnSegment(const Point &point, const Segment &segment);

bool isIntersection(const Ray &ray, const Segment &segment);
bool isIntersection(const Line &line, const Segment &segment);

double distance(const Point &point, const Segment &segment);

// Returns distance from point inside polygon to polygon.
// If point is inside polygon then returning value is undefined.

double distanceLinear(const Point &point, const Polygon &poly);
double distance(const Point &point, const Polygon &poly);

// Returns index of point in polygon that has maximal projection on
// vector dir 
int maximalPointLinear(const Point &dir, const Polygon &poly);
int maximalPoint(const Point &dir, const Polygon &poly);

// Returns index of point in polygon that has minimal projection on
// vector dir 
int minimalPointLinear(const Point &dir, const Polygon &poly);
int minimalPoint(const Point &dir, const Polygon &poly);


PointDouble intersection(const Line &first, const Line &second, bool *ok = 0);

// Intersects line and polygon and returns segment of line that lies inside
// polygon. 
// If there is no intersection, then returning value is undefined.
// If "ok" is not null than *ok will be set to true if intersection exists
// or false otherwise
SegmentDouble intersectionLinear(const Line &line, const Polygon &poly, bool *ok = NULL);
SegmentDouble intersection(const Line &line, const Polygon &poly, bool *ok = NULL);
#endif
