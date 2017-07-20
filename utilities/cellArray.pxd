cimport numpy as np

cdef class baseArray

cdef class baseArrayIterator:
  cdef baseArray array
  cdef int index

cdef class baseArray:
  """template class for other data type arrays"""
  cdef readonly int length, alloc
  cdef np.ndarray _numpy_array

  cpdef reserve(self, int size) #look into optimation into pure c++ methods
  cpdef resize(self, int size)
  cpdef np.ndarray getNumPyArray(self)
  cpdef squeeze(self)
  cpdef remove(self, np.ndarray indexList, bint sortedInput=*)
  cpdef extend(self, np.ndarry inputArray)
  cpdef reset(self)
  cpdef shrink(self, long size)

  cpdef alignArray(self, np.ndarray newIndices)
  cpdef str getCtype(self)
  cpdef copyValues(self, intArray indices, baseArray dest)
  cpdef pasteValues(self, intArray indices, baseArray dest)
  cpdef addValues(self, intArray indices, baseArray dest)

#will only use double and int array since test cases are small
cdef class doubleArray(baseArray):
  """manages an array of numpy floats64's"""
  cdef np.float64_t *data
  cdef
