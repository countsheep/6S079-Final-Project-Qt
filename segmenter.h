#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <vecmath/include/vecmath.h>

using namespace std;

// Mesh segmentation typedefs:
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Polyhedron::Edge_iterator Edge_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Polyhedron::Halfedge_around_vertex_circulator Vertex_halfedge_circulator;

// PCA typedefs:
typedef double                      FT;
typedef CGAL::Simple_cartesian<FT>  K;
typedef K::Line_3                   Line;
typedef K::Plane_3                  Plane_3;
typedef K::Point_3                  Point;
typedef K::Triangle_3               Triangle;

class Segmenter
{
public:
    Segmenter(string path);
    std::vector<int> segment_mesh();
    std::vector<std::vector<Vector3f> > get_joint_planes();
private:
    Polyhedron mesh;
    int number_of_segments;
    // joints[segment group 1][segment group 2][list of points at the intersection of group 1 and group 2]
    std::vector<std::vector<std::vector<Point_3> > > joints;
};

#endif // SEGMENTER_H
