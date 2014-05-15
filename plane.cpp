#include "plane.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;
Plane::Plane(vector<Vector3f> coords)
{
    anchor = coords[1];
    points = coords;
    normal = Vector3f::cross(coords[0]-coords[1], coords[0]-coords[2]);
    color = Vector3f(rand()%256, rand()%256,rand()%256);
    buffer = Vector3f(color.x(),color.y(), color.z());
    colorf = Vector3f(float (color.x())/255.f, float (color.y())/255.f, float (color.z())/255.f);
}

void Plane::move_plane(Vector3f diff)
{
    for (int i = 0; i < points.size(); i++){
        points[i] = points[i]+diff;
    }
}

bool Plane::match_id(Vector3f c)
{
    bool t = (c.x() == buffer.x()) && (c.y() == buffer.y()) && (c.z() == buffer.z());
    return t;
}