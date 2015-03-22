#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <ostream>
#include <istream>
#include <cstdio>
#include <vector>

int sign(long long a);
template <class T> T sqr(const T &a);

struct Point
{   
    int x, y;

    Point();
    Point(int _x, int _y);
    void print(FILE *f = stdout) const;
    void read(FILE *f = stdin);

    Point operator + (const Point &a) const;
    Point operator - (const Point &a) const;
    long long operator * (const Point &a) const;
    long long operator % (const Point &a) const;
    Point operator * (int a) const;

    Point& operator += (const Point &a);
    Point& operator -= (const Point &a);
    Point& operator *= (int a);

    bool operator == (const Point &a) const;

    double len() const;
    long long len2() const;

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

struct Segment
{
    Segment();
    Segment(const Point &first, const Point &second);
    Point first, second;
    void print(FILE *f = stdout) const;
};

struct Ray
{
    Ray();
    Ray(const Point &start, const Point &dir);
    Point start, dir;
    void print(FILE *f = stdout) const;
};

std::ostream& operator << (std::ostream &out, const Point &p);
std::istream& operator >> (std::istream &in, const Point &p);
std::ostream& operator << (std::ostream &out, const Polygon &p);
std::ostream& operator << (std::ostream &out, const Ray &ray);
std::ostream& operator << (std::ostream &out, const Segment &segment);


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

double distance(const Point &point, const Segment &poly);

// Returns distance from point inside polygon to polygon.
// If point is inside polygon then returning value is undefined.

double distanceLinear(const Point &point, const Polygon &poly);
double distance(const Point &point, const Polygon &poly);
#endif
