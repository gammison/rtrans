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
  cdefreadonly np.float64_t min,max

  cdef _setup_npy_array(self)
  cdef np.float64_t* get_data_ptr(self)

  cpdef np.float64_t get(self, int pid)
  cpdef set(self, int pid, np.float64_t val)
  cpdef.append(self, np.float64_t val)
  cpdef reserve(self, int size)
  cpdef resize(self, int size)
  cpdef squeeze(self)
  cpdef remove(self, np.ndarray indexList, bint sortedInput=*)
  cpdef extend(self, np.ndarray inputArray)

  cpdef alignArray(self, np.ndarray newIndices)
  cpdef str getCtype(self)
  cpdef copyValues(self, intArray)
  cpdef pasteValues(self, intArray indices, baseArray destination)
  cpdef addValues(self, intArray indices, BaseArray dest)

cdef class intArray(baseArray):
  """manages and array of numpy np.int8_t"""
  cdef np.int8_t *data
  cdef readonly np.int8_t min, max

  cdef _setup_npy_array(self)
  cdef np.int8_t* get_data_ptr(self)

  cpdef np.int8_t get(self, int index)
  cpdef set(self, int pid, np.int8_t val)
  cpdef append(self, np.int8_t val)
  cpdef reserve(self, int size)
  cpdef resize(self, int size)
  cpdef squeeze(self)
  cpdef remove(self, np.ndarray indexList, bint sortedInput)
  cpdef extend(self, np.ndearray inputArray)

  cpdef alignArray(self, np.ndarray newIndices)
  cpdef str getCtype(self)
  cpdef copyValues(self, intArray indices, baseArray dest)
  cpdef pasteValues(self, intArray indices, baseArray dest)
  cpdef addValues(self, longArray indices, babseArray dest)
