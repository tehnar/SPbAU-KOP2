#include "geometry.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;
                    
vector <Polygon> handPolygons = { 
Polygon({Point(0, -5), Point(3, -4), Point(4, -3), Point(5, 0), Point(4, 3), Point(3, 4), Point(0, 5), Point(-3, 4), Point(-4, 3), Point(-5, 0), Point(-4, -3), Point(-3, -4)}),
Polygon({Point(-1, 1), Point(1, 1), Point(0, 5)}),
Polygon({Point(-5, 5), Point(-5, 0), Point(0, 0), Point(0, 5)}),
Polygon({Point(0, 0), Point(0, 5), Point(-2, 3)}),
Polygon({Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)}),
Polygon({Point(5, 0), Point(4, 1), Point(1, 1), Point(0, 0)})
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
int main()
{
    srand(time(NULL));    
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

    assert(abs(distanceLinear(Point(-1, -1), a) - sqrt(2) < 0.0001));
    assert(abs(distanceLinear(Point(-1, 1), a) - 1 < 0.0001));

    assert(a[minimalPointLinear(Point(1, 1), a)] == Point(0, 0));
    assert(a[maximalPointLinear(Point(1, 1), a)] == Point(2, 2));
    assert(a[minimalPoint(Point(1, 1), a)] == Point(0, 0));
    assert(a[maximalPoint(Point(1, 1), a)] == Point(2, 2));

    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++)
        {
            for (auto &poly : handPolygons)
            {                                        
                Point pnt = Point(i, j);
                
                assert(poly[minimalPoint(pnt, poly)] * pnt == poly[minimalPointLinear(pnt, poly)] * pnt);
                assert(poly[maximalPoint(pnt, poly)] * pnt == poly[maximalPointLinear(pnt, poly)] * pnt);
                bool resultLinear = isInsideLinear(pnt, poly);
                bool resultBS1 = isInsideBS1(pnt, poly);
                bool resultBS2 = isInsideBS2(pnt, poly);
                //cerr << i << " " << j << " " << resultLinear << " " << resultBS1 << " " << resultBS2 << endl;
                assert(resultLinear == resultBS1 && resultBS1 == resultBS2);
                if (!resultLinear)
                {
                    assert(poly[findLeftTangentLinear(pnt, poly)] == poly[findLeftTangentBS(pnt, poly)]);
                    assert(poly[findRightTangentLinear(pnt, poly)] == poly[findRightTangentBS(pnt, poly)]);
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
        assert(poly[minimalPoint(pnt, poly)] * pnt == poly[minimalPointLinear(pnt, poly)] * pnt);
        assert(poly[maximalPoint(pnt, poly)] * pnt == poly[maximalPointLinear(pnt, poly)] * pnt);
        assert(in1 == in2 && in2 == in3);
        if (!in1)
        {
            assert(poly[findLeftTangentLinear(pnt, poly)] == poly[findLeftTangentBS(pnt, poly)]);
            assert(poly[findRightTangentLinear(pnt, poly)] == poly[findRightTangentBS(pnt, poly)]);
            assert(abs(distanceLinear(pnt, poly) - distance(pnt, poly)) < 1e-5);
            
        }
    }
}
