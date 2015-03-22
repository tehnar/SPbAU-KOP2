#include "geometry.h"
#include <cassert>
#include <iostream>
#include <algorithm>

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
    for (int i = 1; i < points.size(); i++)
        points[i] += points[i - 1];
    return Polygon(points);                                
}

int main()
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

    assert(abs(distanceLinear(Point(-1, -1), a) - sqrt(2) < 0.0001));
    assert(abs(distanceLinear(Point(-1, 1), a) - 1 < 0.0001));

    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++)
        {
            for (auto &poly : handPolygons)
            {
                bool resultLinear = isInsideLinear(Point(i, j), poly);
                bool resultBS1 = isInsideBS1(Point(i, j), poly);
                bool resultBS2 = isInsideBS2(Point(i, j), poly);
                //cerr << i << " " << j << " " << resultLinear << " " << resultBS1 << " " << resultBS2 << endl;
                assert(resultLinear == resultBS1 && resultBS1 == resultBS2);
                if (!resultLinear)
                {
                    assert(poly[findLeftTangentLinear(Point(i, j), poly)] == poly[findLeftTangentBS(Point(i, j), poly)]);
                    assert(poly[findRightTangentLinear(Point(i, j), poly)] == poly[findRightTangentBS(Point(i, j), poly)]);
                    assert(abs(distanceLinear(Point(i, j), poly) - distance(Point(i, j), poly)) < 1e-5);
                }
            }
        
        } 

}
