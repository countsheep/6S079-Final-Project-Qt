#include "separator.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Extended_cartesian.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QtDebug>
#include <vecmath/include/vecmath.h>

using namespace std;


typedef CGAL::Extended_cartesian< CGAL::Lazy_exact_nt<CGAL::Gmpq> > K_extcart;
//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<K_extcart>  Polyhedron_extcart;
typedef CGAL::Nef_polyhedron_3<K_extcart>  Nef_polyhedron;
typedef K_extcart::Plane_3  Plane_3_extcart;

typedef Polyhedron_extcart::Facet_iterator Facet_iterator_extcart;
typedef Polyhedron_extcart::Halfedge_around_facet_circulator Halfedge_facet_circulator_extcart;
typedef K_extcart::Point_3 Point_3_extcart;

Separator::Separator(){}
Separator::Separator(string path)
{
    ifstream inputtt(path);

    Polyhedron_extcart P;

    if ( !inputtt || !(inputtt >> P) || P.empty() ) {
        qDebug() << "Not a valid off file.";
        //return EXIT_FAILURE;
    }

    qDebug() << "SRART: size of vertices" << P.size_of_vertices();

    Nef_polyhedron mesh(P); // convert the loaded Polyhedron_extcart to a Nef_polyhedron

    qDebug() << "Is the mesh valid? " << mesh.is_valid();
    qDebug() << "Is the mesh empty? " << mesh.is_empty();
    qDebug() << "Number of vertices? " << mesh.number_of_facets();
    qDebug() << "-------------";

    meshPieces.push_back(mesh);
}

void Separator::slice(float a ,float b,float c,float d) {
    qDebug() << "Slice happening" << a << b << c << d;
    Nef_polyhedron NPlane(Plane_3_extcart( a, b, c, d));
    Nef_polyhedron NPlane2(Plane_3_extcart( -a, -b, -c, -d));
    vector<Nef_polyhedron> meshPiecesCopy = meshPieces;
    //for (vector<Nef_polyhedron>::iterator i=meshPiecesCopy.begin(); i != meshPiecesCopy.end(); ) {
    for ( int i=0; i < meshPiecesCopy.size(); i++) {
        qDebug() << "Slice iteration";
        Nef_polyhedron mesh = meshPiecesCopy[i];//*i;
        qDebug() << "Is the mesh valid? " << mesh.is_valid();
        qDebug() << "Is the mesh empty? " << mesh.is_empty();
        qDebug() << "Number of vertices? " << mesh.number_of_facets();
        qDebug() << "Now performing union operations with the planes";
        Nef_polyhedron piece1 = mesh * NPlane; //union operation
        Nef_polyhedron piece2 = mesh * NPlane2;
        if(!piece1.is_simple()) {
            qDebug() << "Uh oh, piece 1 was not simple!";
        }
        if(!piece2.is_simple()) {
            qDebug() << "Uh oh, piece 2 was not simple!";
        }
        qDebug() << "Now checking equality of two parts of cut";
        if (piece1 != piece2) {
            qDebug() << "unique peices";
            meshPieces.erase(meshPieces.begin()+i);
            meshPieces.push_back(piece1);
            meshPieces.push_back(piece2);
        } else {
            qDebug() << "plane did not cross";
            //++i;
        }
    }
}

int Separator::getNumberOfMeshPieces() {
    return meshPieces.size();
}

vector<vector<Vector3f> > Separator::getMeshSegmentFaces(int index) {
    qDebug() << "meshPieces size" << meshPieces.size();
    Nef_polyhedron np = meshPieces.at(index);
    Polyhedron_extcart p;
    np.convert_to_polyhedron(p);

    vector<vector<Vector3f> >facesAndVertices;

    for(Facet_iterator_extcart facet_it = p.facets_begin(); facet_it != p.facets_end(); ++facet_it) {
        //qDebug() << "Looking at facet";
        Halfedge_facet_circulator_extcart h = facet_it->facet_begin();
        vector<Vector3f> vertList;
        do {
            Point_3_extcart point =  h->vertex()->point();
            Vector3f vec(CGAL::to_double(point.x()),CGAL::to_double(point.y()),CGAL::to_double(point.z()));
            vertList.push_back(vec);
        } while ( ++h != facet_it->facet_begin() );
        facesAndVertices.push_back(vertList);
    }
    qDebug() << "convertedPolyToFaces successfully";
    return facesAndVertices;
}


