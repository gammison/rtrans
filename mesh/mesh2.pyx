import numpy as np
cimport numpy as np
cimport libc.stdlib as stdlib

from ..utilities.cellContainer import cellContainer
from ..utilities.cellContainer import doubleArray
#something like this needed in cellContainer

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

  #cdef int update_initial_tess(self, double *x[3], int upNumParticles):
  #    return self.thisptr.updateTesselation(x, upNumParticles) Not neeeded, no parallelization

  cdef int countNumFaces(self):
      return self.thisptr.countNumFaces()

  cdef int extrGeom(self, double* x[3], double* dCom[3], double* volume,
              double* faceArea, double* faceCom[3], double* faceN[3],
              int* pair_i, int* pair_j, nn_vec &neighbors):
      return self.thisptr.extrGeom(x, dCom, volume,
              faceArea, faceCom, faceN,
              pair_i, pair_j, neighbors)

#cdef class PyTess3d(PyTess): Not implemented yet, doing 2D grid first

cdef class Mesh2:
    def __init__(self, int numNeighbors):
      cdef nn nearestNeigh = nn()
      self.neighbors == nn_vec(numNeighbors,nearestNeigh)

    def _initialize(self):
      faceVars = {
      "area": "double", #perhaps should be length?
      "pair-i": "long", #the cells on inside and outside of a face
      "pair-j": "long",
      "com-x": "double",
      "com-y": "double", #really center of area
      "velocity-x": "double",
      "velocity-y": "double",
      "norm-x": "double",
      "norm-y": "double"
      }

      self.fields = ["volume","dcom-x","dcom-y"] #volume perhaps area or use volume for generalization
      self.faces = cellContainer(varDict=faceVars)
      self.faces.namedGroups['velocity'] = ['velocity-x', 'velocity-y']
      self.faces.namedGroups['norm'] = ['norm-x', 'norm-y']
      self.faces.namedGroups['com'] = ['com-x', 'com-y']

      self.tess = PyTess2d

    def tessellate(self, cellContainer pc):
        self._tessellate(pc)
    cdef _tessellate(self, cellContainer pc):
