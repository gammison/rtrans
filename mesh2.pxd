from libcpp.vector cimport vector
#need to add boundary and particle from phd/boundary
from libcpp.vector cimport vector

from ..boundary.boundary cimport Boundary
from ..containers.containers cimport CarrayContainer

ctypedef vector[int] nn           # nearest neighbors
ctypedef vector[nn] nn_vec

cdef extern from "tess2.h":
    cdef cppclass Tess2d:
        Tess2d() except +
        void reset_tess()
        int buildTesselation(double *x[3], double *radius_sq, int num_particles, double huge)
        int updateTesselation(double *x[3], int up_num_particles)
        int count_number_of_faces()
        int extrGeom(double* x[3], double* dCom[3], double* volume,
                double* faceArea, double* fCom[3], double* faceN[3],
                int* pair_i, int* pair_j, nn_vec &neighbors)

    #cdef cppclass Tess3d:
    #    Tess3d() except +
    #    void reset_tess()
    #    int build_initial_tess(double *x[3], double *radius_sq, int num_particles, double huge)
    #    int update_initial_tess(double *x[3], int up_num_particles)
    #    int count_number_of_faces()
    #    int extract_geometry(double* x[3], double* dcenter_of_mass[3], double* volume,
    #            double* face_area, double* face_com[3], double* face_n[3],
    #            int* pair_i, int* pair_j, nn_vec &neighbors)

cdef class PyTess:

    cdef void reset_tess(self)
    cdef int buildTesselation(self, double *x[3], double *radius, int numParticles, double maxRadi)
    cdef int updateTesselation(self, double *x[3], int upNumParticles)
    cdef int count_number_of_faces(self)
    cdef int extrGeom(self, double* x[3], double* dcenter_of_mass[3], double* volume,
                double* face_area, double* face_com[3], double* face_n[3],
                int* pair_i, int* pair_j, nn_vec &neighbors)

cdef class PyTess2d(PyTess):
    cdef Tess2d *thisptr

#cdef class PyTess3d(PyTess):
#    cdef Tess3d *thisptr

cdef class Mesh:

    #cdef public Boundary boundary
    #cdef public CarrayContainer faces, need new container
    cdef public int dim
    cdef public list fields

    cdef PyTess tess
    cdef nn_vec neighbors

    cdef _tessellate(self, CarrayContainer pc)
    cdef _build_geometry(self, CarrayContainer pc)
    cdef _reset_mesh(self)
