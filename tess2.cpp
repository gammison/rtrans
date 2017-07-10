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
            edgeCirc ed = tess.incident_edges(vi), done(ed);

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

            radius[i] = 2.01*std::sqrt(rMax2);
        }
        //std::size_t memory = CGAL::Memory_sizer().resident_size();
        //std::cout << "Tessellation size: " << (memory >> 20) <<  " Mib" <<std::endl;
        return 0;
}

int tess2::updateTesselation(
        double *x[3],
        int upNumParticles) {

    int startNum = locNumParticles;
    totNumParticles = locNumParticles + upNumParticles;

    Tess &tess = *static_cast<Tess*>(ptess);

    // create points for ghost particles
    std::vector<Point> particles;
    for (int i=startNum; i<totNumParticles; i++)
        particles.push_back(Point(x[0][i], x[1][i]));

    // add ghost particles to the tess
    Vertex_handle vt;
    for (int i=0, j=local_num_particles; i<up_num_particles; i++, j++) {
        vt = tess.insert(particles[i]);
        vt->info() = j;
    }
    return 0;
}

int tess2::count_number_of_faces(void) {

    Tess &tess = *(Tess*) ptess;
    std::vector<Vertex_handle> &vertList = *static_cast<d::vector<VertexHandle>*>(pVertList);
    int num_faces = 0;

    for (int i=0; i<local_num_particles; i++) {

        const Vertex_handle &vi = vertList[i];

        // find all edges that are incident with particle vertex
        edgeCirc ed = tess.incident_edges(vi), done(ed);

        // process each edge
        do {
            // edge is infinite
            if (tess.is_infinite(ed))
                return -1;

            // extract voronoi face from edge
            CGAL::Object o = tess.dual(*ed);

            // only consider finite faces
            if (CGAL::object_cast<K::Segment_2>(&o)) {

                const Edge e = *ed;
                int id1 = e.first->vertex( (e.second+2)%3 )->info();
                int id2 = e.first->vertex( (e.second+1)%3 )->info();

                if (id1 != i)
                    return -1;
                if (id1 < id2)
                    num_faces++;

            } else {
                // face is a ray, tess is not complete
                return -1;
            }
        } while (++ed != done);
    }
    return num_faces;
}

int tess2::extrGeom(
        double* x[3],
        double* dcom[3],
        double* volume,
        double* faceArea,
        double* faceCom[3],
        double* faceN[3],
        int* pair_i,
        int* pair_j,
        std::vector< std::vector<int> > &neighbors) {

    // face counter
    int fc=0;
    Tess &tess = *(Tess*) ptess;
    std::vector<Vertex_handle> &vt_list = *(std::vector<Vertex_handle>*) pvt_list;


    // only process local particle information
    for (int i=0; i<locNumParticles; i++) {

        const vertexHandle &vi = vertList[i];
        double xp = x[0][i], yp = x[1][i];
        double cx = 0.0, cy = 0.0, vol = 0.0;

        // find all edges that are incident with particle vertex
        edgeCirc ed = tess.incident_edges(vi), done(ed);

        /* process each edge, find voronoi face and neighbor
         that make the edge with current particle. If face
         area is not zero store face area, normal, and centroid
        */
        do {
            // edge that contains infinite vertex
            if (tess.is_infinite(ed)) {
                std::cout << "infinite" << "x: " << xp << " y: " << yp << std::endl;
                return -1;
            }

            // extract voronoi face from edge
            CGAL::Object o = tess.dual(*ed);

            // only consider finite faces
            if (const K::Segment_2 *sg = CGAL::object_cast<K::Segment_2>(&o)) {

                const Edge e = *ed;

                const int id1 = e.first->vertex( (e.second+2)%3 )->info();
                const int id2 = e.first->vertex( (e.second+1)%3 )->info();

                const Point& p1 = sg->point(0);
                const Point& p2 = sg->point(1);

                double xn = x[0][id2], x1 = p1.x(), x2 = p2.x();
                double yn = x[1][id2], y1 = p1.y(), y2 = p2.y();
                //double xn = x[0][id2], x1 = CGAL::to_double(p1.x()), x2 = CGAL::to_double(p2.x());
                //double yn = x[1][id2], y1 = CGAL::to_double(p1.y()), y2 = CGAL::to_double(p2.y());

                // difference vector between particles
                double xr = xn - xp;
                double yr = yn - yp;

                // distance between particles
                double h = std::sqrt(xr*xr + yr*yr);

                // edge vector
                double xe = x2 - x1;
                double ye = y2 - y1;

                // face area in 2d is length between voronoi vertices
                //double area = std::sqrt(xe*xe + ye*ye);
                double area0 = std::sqrt(xe*xe + ye*ye);

                // center of mass of face
                double fx = 0.5*(x1 + x2);
                double fy = 0.5*(y1 + y2);

                // need to work on
                const double SMALLDIFF = 1.0e-10;
                const double L1 = std::sqrt(area0);
                const double L2 = std::sqrt( (fx-xp)*(fx-xp) + (fy-yp)*(fy-yp) );
                const double area = (L1 < SMALLDIFF*L2) ? 0.0 : area0;

                // ignore face
                if (area == 0.0)
                    continue;

                // the volume of the cell is the sum of triangle areas - eq. 27
                vol += 0.25*area*h;

                // center of mass of triangle - eq. 31
                double tx = 2.0*fx/3.0 + xp/3.0;
                double ty = 2.0*fy/3.0 + yp/3.0;

                // center of mass of the celll is the sum weighted center of mass of
                // the triangles - eq. 29
                cx += 0.25*area*h*tx;
                cy += 0.25*area*h*ty;

                // faces are defined by real partilces
                if (id1 < id2) {

                    faceArea[fc] = area;

                    // orientation of the face
                    faceN[0][fc] = xr/h;
                    faceN[1][fc] = yr/h;

                    // center of mass of face
                    faceCom[0][fc] = fx;
                    faceCom[1][fc] = fy;

                    pair_i[fc] = id1;
                    pair_j[fc] = id2;

                    // store neighbors - face id is stored
                    neighbors[id1].push_back(fc);
                    neighbors[id2].push_back(fc);

                    fc++;
                }

            } else {
                std::cout << "infinite face " << "x: " << xp << " y: " << yp << std::endl;
                return -1;
            }
        } while (++ed != done);

        // store volume and delta com
        volume[i] = vol;
        dcom[0][i] = cx/vol - xp;
        dcom[1][i] = cy/vol - yp;

    }

    //std::size_t memory = CGAL::Memory_sizer().resident_size();
    //std::cout << "Tessellation size: " << (memory >> 20) <<  " Mib" <<std::endl;
    return fc;
} //should be split up later for readability, maybe a helper class with all calculations 
