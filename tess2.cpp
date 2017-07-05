//sections used from Ricardo Fernandez moving_mesh
#include "tess.h"
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h> //dual gets voronoi
#include <CGAL/Triangulation_vertex_base_with_info_2.h> //base class for vertices of triangulation
#include <CGAL/number_utils.h>

#include <CGAL/Memory_sizer.h> //measures memory size of process, will use for testing

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>              Tess;
typedef CGAL::Object          Object; //used to store voronoi diagram
typedef Tess::Vertex_handle   vertexHandle;
typedef Tess::Point           Point;
typedef Tess::Edge            Edge;
typedef Tess::Edge_circulator edgeCirc;

tess2::tess2(void){
  ptess = NULL; //the tesselation
  vertList = NULL; //vertex list
}

void tess2::resetTess(){
  delete (tess*)ptess;
  delete (std::vector<Vertex_handle>) vertList;
}

int tess2::buildTesselation(){double *x[3], double *radius,  int particleNum, double huge){ //possible builder pattern switch?
  //huge and radius used for casting rays
  
}
