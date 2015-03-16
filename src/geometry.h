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
	long long x, y;

	Point();
	Point(long long _x, long long _y);
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


bool isInsideLinear(const Point &point, const Polygon &poly);
bool isInsideBS1(const Point &point, const Polygon &poly);
bool isInsideBS2(const Point &point, const Polygon &poly);
bool isOnSegment(const Point &point, const Segment &segment);
bool isIntersection(const Ray &ray, const Segment &segment);

#endif
