#include "geometry.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <tuple>

using namespace std;
                    
vector <Polygon> handPolygons = { 
Polygon({Point(0, -5), Point(3, -4), Point(4, -3), Point(5, 0), Point(4, 3), Point(3, 4), Point(0, 5), Point(-3, 4), Point(-4, 3), Point(-5, 0), Point(-4, -3), Point(-3, -4)}),
Polygon({Point(-1, 1), Point(1, 1), Point(0, 5)}),
Polygon({Point(-5, 5), Point(-5, 0), Point(0, 0), Point(0, 5)}),
Polygon({Point(0, 0), Point(0, 5), Point(-2, 3)}),
Polygon({Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)}),
Polygon({Point(5, 0), Point(4, 1), Point(1, 1), Point(0, 0)})
};

const int MAX = (1<<30) - 1;
const Polygon maxSquare({Point(MAX, MAX), Point(-MAX, MAX), Point(-MAX, -MAX), Point(MAX, -MAX)}),
              octagon({Point(-1, -2), Point(1, -2), Point(2, -1), Point(2, 1), Point(1, 2), Point(-1, 2), Point(-2, 1), Point(-2, -1)});
              
vector <tuple <Point, Polygon, bool> > isInsideTests = {
make_tuple(Point(0, 0), maxSquare, 1),
make_tuple(Point(MAX, 0), maxSquare, 1),
make_tuple(Point(-MAX, 0), maxSquare, 1),
make_tuple(Point(0, MAX), maxSquare, 1),
make_tuple(Point(0, -MAX), maxSquare, 1),
make_tuple(Point(0, -2), octagon, 1),
make_tuple(Point(2, 0), octagon, 1),
make_tuple(Point(0, 2), octagon, 1),
make_tuple(Point(-2, 0), octagon, 1),
make_tuple(Point(0, 0), octagon, 1),
make_tuple(Point(-3, 0), octagon, 0),
make_tuple(Point(0, 3), octagon, 0),
make_tuple(Point(2, 3), octagon, 0),
make_tuple(Point(-2, 3), octagon, 0),
make_tuple(Point(2, -3), octagon, 0),
make_tuple(Point(2, -3), octagon, 0),
make_tuple(Point(4, -2), octagon, 0),
make_tuple(Point(-5, 2), octagon, 0),
make_tuple(Point(MAX, MAX), octagon, 0),
make_tuple(Point(-MAX, MAX), octagon, 0),
make_tuple(Point(MAX, -MAX), octagon, 0),
make_tuple(Point(-MAX, -MAX), octagon, 0),
};

vector <tuple <Point, Polygon, double> > distanceTests = {
make_tuple(Point(-4, 0), octagon, 2),
make_tuple(Point(1, 3), octagon, 1),
make_tuple(Point(2, 2), octagon, sqrt(2) / 2),
make_tuple(Point(-2, -3), octagon, sqrt(2)),
make_tuple(Point(MAX, MAX), octagon, sqrt(2 * sqr(MAX - 1.5))),
};

vector <tuple <Point, Polygon, int> > maximalPointTests = {
make_tuple(Point(MAX, MAX), maxSquare, 0),
make_tuple(Point(MAX, -MAX), maxSquare, 3),
make_tuple(Point(-MAX, -MAX), maxSquare, 2),
make_tuple(Point(-MAX, MAX), maxSquare, 1),
make_tuple(Point(MAX, 1), maxSquare, 0),
make_tuple(Point(1, -MAX), maxSquare, 3),
make_tuple(Point(-MAX, -1), maxSquare, 2),
make_tuple(Point(-1, MAX), maxSquare, 1),
make_tuple(Point(2, 1), octagon, 3),
make_tuple(Point(-MAX / 2, -MAX), octagon, 0),
};

vector <tuple <Point, Polygon, int> > leftTangentTests = {
make_tuple(Point(MAX, MAX), octagon, 2),
make_tuple(Point(4, 1), octagon, 2),
make_tuple(Point(2, 2), octagon, 3),
make_tuple(Point(-MAX, -MAX), octagon, 6),
make_tuple(Point(0, -3), octagon, 0)
};

vector <tuple <Point, Polygon, int> > rightTangentTests = {
make_tuple(Point(MAX, MAX), octagon, 5),
make_tuple(Point(4, 1), octagon, 4),
make_tuple(Point(2, 2), octagon, 4),
make_tuple(Point(-MAX, -MAX), octagon, 1),
make_tuple(Point(0, -3), octagon, 1)
};

vector <tuple <Line, Polygon, SegmentDouble> > intersectionTests = {
make_tuple(Line(Point(-MAX, -MAX), Point(MAX, MAX)), octagon, SegmentDouble(PointDouble(-1.5, -1.5), PointDouble(1.5, 1.5))),
make_tuple(Line(Point(MAX, -MAX), Point(-MAX, MAX)), octagon, SegmentDouble(PointDouble(1.5, -1.5), PointDouble(-1.5, 1.5))),
make_tuple(Line(Point(2, 1), Point(2, -1)), octagon, SegmentDouble(PointDouble(2, 1), PointDouble(2, -1))),
make_tuple(Line(Point(-1, -2), Point(1, -2)), octagon, SegmentDouble(PointDouble(-1, -2), PointDouble(1, -2))),
make_tuple(Line(Point(-1, -3), Point(3, -1)), octagon, SegmentDouble(PointDouble(1, -2), PointDouble(1, -2))),
make_tuple(Line(Point(MAX, MAX), Point(-MAX, -MAX)), maxSquare, SegmentDouble(PointDouble(MAX, MAX), PointDouble(-MAX, -MAX)))
};

Polygon genLargePolygon(int maxSideCoord)
{
    vector <Point> points;
    for (int i = 1; i <= maxSideCoord; i++)
        for (int j = 1; j < maxSideCoord; j++)
            if (__gcd(i, j) == 1)
            {
                points.push_back(Point(i, j));
                points.push_back(Point(i, -j));
                points.push_back(Point(-i, j));
                points.push_back(Point(-i, -j));
            }
    points.push_back(Point(0, maxSideCoord));
    points.push_back(Point(0, -maxSideCoord));
    points.push_back(Point(maxSideCoord, 0));
    points.push_back(Point(-maxSideCoord, 0));
    sort(points.begin(), points.end(), compareByAngle);
    points[0] = Point(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2);
    for (int i = 1; i < (int) points.size(); i++)
        points[i] += points[i - 1];
    return Polygon(points);                                
}

int rand31()
{                               
    if (RAND_MAX == (1u<<31u) - 1)
        return rand();
    return (rand() << 16) + rand();
}

bool eq(double a, double b)
{
    return abs(a - b) < 1e-8;   
}

void testIsInside()
{
    Polygon a({Point(2, 0), Point(2, 2), Point(0, 2), Point(0, 0)});

    assert(isInsideLinear(Point(0, 0), a));
    assert(isInsideLinear(Point(1, 1), a));
    assert(isInsideLinear(Point(1, 2), a));
    assert(!isInsideLinear(Point(-1, 2), a));
    assert(!isInsideLinear(Point(10, 1), a));
    assert(!isInsideLinear(Point(3, -1), a));

    assert(isInsideBS1(Point(0, 0), a));
    assert(isInsideBS1(Point(1, 1), a));
    assert(isInsideBS1(Point(1, 2), a));
    assert(!isInsideBS1(Point(-1, 2), a));
    assert(!isInsideBS1(Point(10, 1), a));
    assert(!isInsideBS1(Point(3, -1), a));

    assert(isInsideBS2(Point(0, 0), a));
    assert(isInsideBS2(Point(1, 1), a));
    assert(isInsideBS2(Point(1, 2), a));
    assert(!isInsideBS2(Point(-1, 2), a));
    assert(!isInsideBS2(Point(10, 1), a));
    assert(!isInsideBS2(Point(3, -1), a));
        
    for (auto &test : isInsideTests)
    {
        assert(isInsideLinear(get<0>(test), get<1>(test)) == get<2>(test));
        assert(isInsideBS1(get<0>(test), get<1>(test)) == get<2>(test));
        assert(isInsideBS2(get<0>(test), get<1>(test)) == get<2>(test)); 
    }
    for (auto &poly : handPolygons)
    {
        for (int i = 0; i < poly.size(); i++)
        {
            assert(isInsideLinear(poly[i], poly));
            assert(isInsideBS1(poly[i], poly));
            assert(isInsideBS2(poly[i], poly)); 
        }
    }
               
}

void testDistance()
{
    for (auto &test : distanceTests)
    {
        assert(eq(distanceLinear(get<0>(test), get<1>(test)), get<2>(test)));
        assert(eq(distance(get<0>(test), get<1>(test)), get<2>(test)));
    }

    for (auto &poly : handPolygons)
    {
        for (int i = 0; i < poly.size(); i++)
        {
            assert(eq(distanceLinear(poly[i], poly), 0));
            assert(eq(distance(poly[i], poly), 0));
        }
    }
}

void testMaximalPoint()
{
    for (auto &test : maximalPointTests)
    {
        assert(get<1>(test)[maximalPointLinear(get<0>(test), get<1>(test))] * get<0>(test) == get<1>(test)[get<2>(test)] * get<0>(test));
        assert(get<1>(test)[maximalPoint(get<0>(test), get<1>(test))] * get<0>(test) == get<1>(test)[get<2>(test)] * get<0>(test));
    }      
}

void testLeftTangent()
{
    for (auto &test : leftTangentTests)
    {
        Point from = get<0>(test);
        assert((from - get<1>(test)[leftTangent(from, get<1>(test))]) % (from - get<1>(test)[get<2>(test)]) == 0);
        assert((from - get<1>(test)[leftTangentLinear(from, get<1>(test))]) % (from - get<1>(test)[get<2>(test)]) == 0);
    }
}

void testRightTangent()
{
    for (auto &test : rightTangentTests)
    {
        Point from = get<0>(test);
        assert((from - get<1>(test)[rightTangent(from, get<1>(test))]) % (from - get<1>(test)[get<2>(test)]) == 0);
        assert((from - get<1>(test)[rightTangentLinear(from, get<1>(test))]) % (from - get<1>(test)[get<2>(test)]) == 0);
    }
}

void testIntersection()
{
    for (auto &test : intersectionTests)
    {
        assert(intersectionLinear(get<0>(test), get<1>(test)) == get<2>(test));
        assert(intersection(get<0>(test), get<1>(test)) == get<2>(test));
    }    
}

int main()
{
    srand(time(NULL));    

    testIsInside();
    testDistance();
    testMaximalPoint();
    testLeftTangent();
    testRightTangent();
    testIntersection();
    assert(isIntersection(Line(Point(0, 0), Point(1, 1)), Segment(Point(0, 0), Point(0, 1))));
    assert(isIntersection(Line(Point(0, 0), Point(1, 1)), Segment(Point(0, 0), Point(1, 1))));
    assert(!isIntersection(Line(Point(0, 0), Point(1, -1)), Segment(Point(2, 2), Point(1, 1))));
    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++)
        {
            for (auto &poly : handPolygons)
            {                                        
                Point pnt = Point(i, j);
                if (pnt != Point(0, 0))
                {                
                    assert(poly[minimalPoint(pnt, poly)] * pnt == poly[minimalPointLinear(pnt, poly)] * pnt);
                    assert(poly[maximalPoint(pnt, poly)] * pnt == poly[maximalPointLinear(pnt, poly)] * pnt);
                }
                bool resultLinear = isInsideLinear(pnt, poly);
                bool resultBS1 = isInsideBS1(pnt, poly);
                bool resultBS2 = isInsideBS2(pnt, poly);
                //cerr << i << " " << j << " " << resultLinear << " " << resultBS1 << " " << resultBS2 << endl;
                assert(resultLinear == resultBS1 && resultBS1 == resultBS2);
                if (!resultLinear)
                {
                    assert(poly[leftTangentLinear(pnt, poly)] == poly[leftTangent(pnt, poly)]);
                    assert(poly[rightTangentLinear(pnt, poly)] == poly[rightTangent(pnt, poly)]);
                    assert(abs(distanceLinear(pnt, poly) - distance(pnt, poly)) < 1e-5);
                }
            }
        
        } 
                          
    for (auto &poly : handPolygons)
        for (int a = -25; a <= 25; a++)
            for (int b = -25; b <= 25; b++)
                for (int c = -25; c <= 25; c++)
                    for (int d = -25; d <= 25; d++)
                    {
                        if (a == c && b == d)
                            continue;
                        bool ok1 = 0, ok2 = 0;
                        SegmentDouble x, y;
                        x = intersection(Line(Point(a, b), Point(c, d)), poly, &ok1);
                        y = intersectionLinear(Line(Point(a, b), Point(c, d)), poly, &ok2);
                        if (ok1 != ok2)
                            cerr << Point(a, b) << " " << Point(c, d) << " " << poly << endl << x << " " << y << " " << ok1 << " " << ok2 << endl;
                        assert(ok1 == ok2);
                        if (ok1)
                            if (!(x == y))
                            {
                                cerr << Point(a, b) << " " << Point(c, d) << " " << x << " " << y << endl << poly << endl;
                                return 0;
                            }
                            assert(x == y);
                    } 
    Polygon poly = genLargePolygon(50);
    for (int i = 0; i < 3000; i++)
    {
        int x = rand() % 10 + 5;
        Point pnt(rand() % (1<<(x + 1)) - (1<<x), rand() - (1<<x));
        bool in1 = isInsideLinear(pnt, poly), in2 = isInsideBS1(pnt, poly), in3 = isInsideBS2(pnt, poly);
        if (pnt != Point(0, 0))
        {
            assert(poly[minimalPoint(pnt, poly)] * pnt == poly[minimalPointLinear(pnt, poly)] * pnt);
            assert(poly[maximalPoint(pnt, poly)] * pnt == poly[maximalPointLinear(pnt, poly)] * pnt);
        }
        Point pnt1(rand() % (1<<15) - (1<<14), rand() % (1<<15) - (1<<14));
        if (pnt1 != pnt)
            assert(intersection(Line(pnt, pnt1), poly) == intersectionLinear(Line(pnt, pnt1), poly));
        assert(in1 == in2 && in2 == in3);
        if (!in1)
        {
            assert(poly[leftTangentLinear(pnt, poly)] == poly[leftTangent(pnt, poly)]);
            assert(poly[rightTangentLinear(pnt, poly)] == poly[rightTangent(pnt, poly)]);
            assert(abs(distanceLinear(pnt, poly) - distance(pnt, poly)) < 1e-5);
            
        }
    }                       
}
