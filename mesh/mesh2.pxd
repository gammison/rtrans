from libcpp.vector cimport vector
from libcpp.vector cimport vector

from ..utilities.datacontainer cimport dataContainer
ctypedef vector[int] nn           # nearest neighbors, given as cell indices
ctypedef vector[nn] nn_vec

cdef extern from "tess2.h":
    cdef cppclass Tess2d:
        Tess2d() except +
        void reset_tess()
        int buildTesselation(double *x[3], double *radius2, int num_particles, double maxRadi)
      #  int updateTesselation(double *x[3], int up_num_particles) Not used since only one box, no boundary
        int countNumFaces()
        int extrGeom(double* x[3], double* dCom[3], double* volume,
                double* faceArea, double* fCom[3], double* faceN[3],
                int* pair_i, int* pair_j, nn_vec &neighbors)


cdef class PyTess:

    cdef void reset_tess(self)
    cdef int buildTesselation(self, double *x[3], double *radius, int numParticles, double maxRadi)
    cdef int updateTesselation(self, double *x[3], int upNumParticles)
    cdef int count_number_of_faces(self)
    cdef int extrGeom(self, double* x[3], double* dCom[3], double* volume,
                double* faceArea, double* faceCom[3], double* faceN[3],
                int* pair_i, int* pair_j, nn_vec &neighbors)

cdef class PyTess2d(PyTess):
    cdef Tess2d *thisptr

#cdef class PyTess3d(PyTess):
#    cdef Tess3d *thisptr

cdef class Mesh:


    cdef public dataContainer faces
    #cdef public int dim, not used since starting with 2D only
    cdef public list fields

    cdef PyTess tess
    cdef nn_vec neighbors

    cdef _tessellate(self, CarrayContainer pc)
    cdef _build_geometry(self, CarrayContainer pc)
    cdef _reset_mesh(self)
