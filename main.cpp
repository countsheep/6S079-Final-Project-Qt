#include "mainwindow.h"
#include <QApplication>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/property_map.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string.h>
#include <locale>

#include <map>
#include <vector>

#include "Extra.h"
#include "camera.h"

#include <stdio.h>

#include <QtDebug>

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
typedef K::Plane_3                  Plane;
typedef K::Point_3                  Point;
typedef K::Triangle_3               Triangle;

Polyhedron mesh;

void cgalstuffdoit() {
    Polyhedron mesh;
    std::ifstream input("/Users/jhaip/Desktop/segment2/data/cactus.off");
    if ( !input || !(input >> mesh) || mesh.empty() ) {
        qDebug() << "Not a valid off file.";
        //return EXIT_FAILURE;
    }
    // create a property-map for segment-ids
    typedef std::map<Polyhedron::Facet_const_handle, std::size_t> Facet_int_map;
    Facet_int_map internal_segment_map;
    boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);
    // calculate SDF values and segment the mesh using default parameters.
    size_t number_of_segments = CGAL::segmentation_via_sdf_values(mesh, segment_property_map);
    qDebug() << "Number of segments: " << number_of_segments;

    for(Facet_iterator facet_it = mesh.facets_begin(); facet_it != mesh.facets_end(); ++facet_it) {
        qDebug() << segment_property_map[facet_it] << " ";
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //cgalstuffdoit();

    return a.exec();
}
