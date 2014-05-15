#ifndef PLANE_H
#define PLANE_H

#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <vecmath/include/vecmath.h>

using namespace std;
class Plane
{
public:
    Plane(vector<Vector3f> coords);
    Vector3f getAnchor();
    vector<Vector3f> getPoints(){return points;}
    Vector3f getNormal(){return normal;}
    void move_plane(Vector3f diff);
    Vector3f getColor(){return color;}
    Vector3f getColorf(){return colorf;}
    bool match_id(Vector3f c);
    bool light = false;
    float getD();


protected:

private:
    Vector3f anchor;
    vector<Vector3f> points;
    Vector3f normal;
    Vector3f color;
    Vector3f colorf;
    Vector3f buffer;
    float d;
};

#endif // PLANE_H
