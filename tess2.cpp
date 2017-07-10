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
typedef CGAL::Object Object;          //used to store voronoi diagram
typedef Tess::Vertex_handle vertexHandle;
typedef Tess::Point Point;
typedef Tess::Edge Edge;
typedef Tess::Edge_circulator edgeCirc;

tess2::tess2(void){
        ptess = NULL; //the tesselation
        pVertList = NULL; //vertex list
}

void tess2::resetTess(){
        delete static_cast<tess2*>(ptess);
        delete static_cast<std::vector<vertexHandle>>pVertList;
        ptess = NULL;
        pVertList = NULL;
}

int tess2::buildTesselation(double *x[3], double *radius,  int particleNum, double maxRadi){ //possible builder pattern switch?
        //maxRadi and radius used for casting rays
        locNumParticles=particleNum;

        std::vector<Point> particles;
        for (int i=0; i<locNumParticles; i++)
                particles.push_back(Point(x[0][i], x[1][i]));
        // create tessellation
        ptess = (void*) new Tess;
        pVertList = (void*) (new std::vector<Vertex_handle>(locNumParticles));
        Tess &tess = *static_cast<Tess*>(ptess);
        std::vector<Vertex_handle> &vtList = *static_cast<std::vector<Vertex_handle>*>(pVertList);

        Vertex_handle vt;
        for (int i=0; i<local_num_particles; i++) {
                vt = tess.insert(particles[i]);
                vt->info() = i;
                vt_list[i] = vt;
        }

        for (int i=0; i<local_num_particles; i++) {

            const vertexHandle &vi = vt_list[i];
            const Point& pos = particles[i];
            double rMax2 = 0.0;

            // find all edges that are incident (connect) with particle vertex
            Edge_circulator ed = tess.incident_edges(vi), done(ed);

            // process each edge
            do {
                // skip edge that contains infinite vertex
                if (tess.is_infinite(ed))
                    continue;

                const Edge e = *ed;
                if (e.first->vertex( (e.second+2)%3 )->info() != i)
                    return -1;

                // extract voronoi face from edge
                CGAL::Object o = tess.dual(*ed);

                // finite faces
                if (const K::Segment_2 *sg = CGAL::object_cast<K::Segment_2>(&o)) {

                    // loop over face vertices
                    for (int j=0; j<2; j++) {
                        const Point& pj = sg->point(j);

                        // calculate max radius from particle
                        rMax2 = std::max( rMax2,
                                (pj.x() - pos.x())*(pj.x() - pos.x()) +
                                (pj.y() - pos.y())*(pj.y() - pos.y()) );
                        //radius_max_sq = std::max( radius_max_sq,
                        //        (CGAL::to_double(pj.x()) - CGAL::to_double(pos.x()))
                        //       *(CGAL::to_double(pj.x()) - CGAL::to_double(pos.x())) +
                        //        (CGAL::to_double(pj.y()) - CGAL::to_double(pos.y()))
                        //       *(CGAL::to_double(pj.y()) - CGAL::to_double(pos.y())) );
                    }

                // infinite face case is considered because a particle can
                // have all faces that are rays
                } else if (CGAL::object_cast<K::Ray_2>(&o)) {

                    rMax2 = maxRadi;
                }

            } while (++ed != done);

            radius[i] = 2.01*std::sqrt(radius_max_sq);
        }
        //std::size_t memory = CGAL::Memory_sizer().resident_size();
        //std::cout << "Tessellation size: " << (memory >> 20) <<  " Mib" <<std::endl;
        return 0;
}
