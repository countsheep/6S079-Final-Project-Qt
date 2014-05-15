#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Extended_cartesian.h>
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <vecmath/include/vecmath.h>

using namespace std;

typedef CGAL::Extended_cartesian< CGAL::Lazy_exact_nt<CGAL::Gmpq> > K_extcart;
//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<K_extcart>  Polyhedron_extcart;
typedef CGAL::Nef_polyhedron_3<K_extcart>  Nef_polyhedron;

class Separator
{
public:
    Separator(string);
    void slice(float,float,float,float);
    void convertPolyToFaces(int index,vector<Vector3f>& v, vector<Vector3f>& n,vector<vector<int> >& f);
private:
    vector<Nef_polyhedron> meshPieces;
};

#endif // SEPARATOR_H
