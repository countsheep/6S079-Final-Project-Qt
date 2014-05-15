#include "segmenter.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/property_map.h>
#include <CGAL/centroid.h>
#include <stdio.h>
#include <string>
#include <QtDebug>
#include <vecmath/include/vecmath.h>

//using namespace std;

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

Segmenter::Segmenter(string path)
{
    std::ifstream input(path);

    if ( !input || !(input >> mesh) || mesh.empty() ) {
        qDebug() << "Not a valid off file.";
        //return EXIT_FAILURE;
    }

    number_of_segments = 0;
}

std::vector<int> Segmenter::segment_mesh() {
    std::vector<int> segments;

    // create a property-map for segment-ids
    typedef std::map<Polyhedron::Facet_const_handle, std::size_t> Facet_int_map;
    Facet_int_map internal_segment_map;
    boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);

    // calculate SDF values and segment the mesh using default parameters.
    number_of_segments = CGAL::segmentation_via_sdf_values(mesh, segment_property_map);
    segments.push_back(number_of_segments);

    qDebug() << "Number of segments: " << number_of_segments;

    // Populate joint vector
    for (int i=0; i<number_of_segments; i++) {
        std::vector< std::vector<Point_3> > a;
        joints.push_back(a);
        for (int j=0; j<number_of_segments; j++) {
            std::vector<Point_3> b;
            joints.at(i).push_back(b);
        }
    }

    // populate edge lists
    for (Edge_iterator h = mesh.edges_begin(); h != mesh.edges_end(); ++h) {
        int side1Label = segment_property_map[h->facet()];
        int side2Label = segment_property_map[h->opposite()->facet()];
        if (side1Label != side2Label) {
            Point_3 p1 = h->vertex()->point();
            Point_3 p2 = h->next()->vertex()->point();
            //qDebug() << "Found edge between " << side1Label << " and " << side2Label;
            joints.at(side1Label).at(side2Label).push_back(p1);
            joints.at(side1Label).at(side2Label).push_back(p2);
            joints.at(side2Label).at(side1Label).push_back(p1);
            joints.at(side2Label).at(side1Label).push_back(p2);
        }
    }

    for(Facet_iterator facet_it = mesh.facets_begin(); facet_it != mesh.facets_end(); ++facet_it) {
        segments.push_back(segment_property_map[facet_it]);
    }
    return segments;
}

std::vector<std::vector<Vector3f> > Segmenter::get_joint_planes() {

    // jointPlanes[for each joint][list of 4 Vector3f's defining the plane]
    std::vector<std::vector<Vector3f> > jointPlanes; // this is the function's return!

    for (int i=0; i<number_of_segments; i++) {
        //qDebug() << "SEGMENT LEVEL " << i;
        for (int j=0; j<number_of_segments; j++) {
            //qDebug() << "---" << j;
            std::vector<Point> jointPoints;
            for (int k=0; k<joints.at(i).at(j).size(); k++) {
                Point_3 point = joints.at(i).at(j).at(k);
                jointPoints.push_back(Point(point.x(),point.y(),point.z()));
                //qDebug() << "POINT " << point.x() << " " << point.y() << " " << point.z();
            }
            // jointPoints is a list of the vertices that make up the boundary between two segments
            // PCA fit a plane and find the centroid of the points listed in jointPoints
            if (joints.at(i).at(j).size() > 0) {
                Plane_3 plane;
                linear_least_squares_fitting_3(jointPoints.begin(),jointPoints.end(),plane,CGAL::Dimension_tag<0>());
                Point centroidPoint = CGAL::centroid(jointPoints.begin(), jointPoints.end(),CGAL::Dimension_tag<0>());

                float scale = 0.5;
                Vector3f planeVector1(plane.base1().x(),plane.base1().y(),plane.base1().z());
                Vector3f planeVector2(plane.base2().x(),plane.base2().y(),plane.base2().z());
                Vector3f planeCentroid(centroidPoint.x(),centroidPoint.y(),centroidPoint.z());

                std::vector<Vector3f> planePoints;
                planePoints.push_back(planeCentroid+planeVector1*scale);
                planePoints.push_back(planeCentroid+planeVector2*scale);
                planePoints.push_back(planeCentroid-planeVector2*scale);
                planePoints.push_back(planeCentroid-planeVector1*scale);

                qDebug() << "Plane Points: (" << planePoints.at(0).x() << planePoints.at(0).y() << planePoints.at(0).z() <<
                                        ") (" << planePoints.at(1).x() << planePoints.at(1).y() << planePoints.at(1).z() <<
                                        ") (" << planePoints.at(2).x() << planePoints.at(2).y() << planePoints.at(2).z() <<
                                        ") (" << planePoints.at(3).x() << planePoints.at(3).y() << planePoints.at(3).z() << ")";
                // add list of 4 joint plane points to the jointPlanes list to be returned:
                jointPlanes.push_back(planePoints);
            }
        }
    }

    return jointPlanes;

}
