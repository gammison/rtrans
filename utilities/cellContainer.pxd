cimport numpy as np
from cellArray cimport baseArray, longArray

cdef class cellContainer:
  cdef readonly dict physProperties
  cdef readonly dict cellArrayInfo
  cdef readonly dict namedGroups

  cpdef registerProperty(self, int size, str name, str dtype=*)

  cpdef getitems(self)
  cpdef removeItems(self, np.ndarray indexList)
  cpdef extend(self, int numParticles)
  cdef void pointerGroups(self, np.float64_t *vec[], list fieldNames)

  cpdef baseArray getCellArray(self, str prop)
  cdef _checkProperty(self, str prop)
  cpdef resize(self, int size)

  cpdef CellarrayContainer extractItems(self, longArray index_array, list fieldNames)
  cpef int appendContainer(self, CellarrayContainer cellArray)
  #copy, paste and add not needed currently
