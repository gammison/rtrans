import numpy as np
cimport numpy as np
cimport libc.stdlib as stdlib

from ..utilities.dataContainer import dataContainer
from ..utilities.dataContainer import dataArray
#something like this needed in dataContainer

cdef class PyTess: #used as template for 2D and 3D tesselations, only 2D used inititially, 3d not implelemented
  cdef void resetTess(self):
      raise NotImplementedError, 'PyTess::resetTess'

  cdef int buildTesselation(self, double *x[3], double *radius2, int numParticles, double maxRadi):
      raise NotImplementedError, 'PyTess::buildTesselation'

  #cdef int update_initial_tess(self, double *x[3], int up_num_particles):, used for updating when particles move accross processors, no parallization used currently
  #    raise NotImplementedError, 'PyTess::update_initial_tess'

  cdef int countNumFaces(self):
      raise NotImplementedError, 'PyTess::countNumFaces'

  cdef int extrGeom(self, double* x[3], double* dCom[3], double* volume,
              double* faceArea, double* faceCom[3], double* faceN[3],
              int* pair_i, int* pair_j, nn_vec &neighbors):
      raise NotImplementedError, 'PyTess::extrGeom'

cdef class PyTess2d(PyTess):
  def __cinit__(self):
      self.thisptr = new Tess2d()

  def __dealloc__(self):
      del self.thisptr

  cdef void resetTess(self):
      self.thisptr.resetTess()

  cdef int buildTesselation(self, double *x[3], double *radius2, int numParticles, double maxRadi):
      return self.thisptr.buildTesselation(x, radius2, numParticles, maxRadi)

  #cdef int update_initial_tess(self, double *x[3], int up_num_particles):
  #    return self.thisptr.update_initial_tess(x, up_num_particles) Not neeeded, no parallelization

  cdef int countNumFaces(self):
      return self.thisptr.countNumFaces()

  cdef int extrGeom(self, double* x[3], double* dCom[3], double* volume,
              double* faceArea, double* faceCom[3], double* faceN[3],
              int* pair_i, int* pair_j, nn_vec &neighbors):
      return self.thisptr.extrGeom(x, dCom, volume,
              faceArea, faceCom, faceN,
              pair_i, pair_j, neighbors)
