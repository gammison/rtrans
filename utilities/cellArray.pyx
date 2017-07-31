import numpy as np

cimport numpy as np
cimport libc.stdlib as stdlib
cimport libc.string as cString

cdef extern from "numpy/arrayobject.h": #imports numpy capi functions
  cdef void import_array()

  ctypedef struct pyArrayObject:
    char *data
    np.npy_intp *dimensions #will be 2 until 3d implemented

  np.ndarray PyArray_SimpleNewFromData(int, np.npy_intp*, int, void*)

#numpy module initialization call
import_array()

cdef class baseArray:
  #base calss of managed cellArrays

  cpdef str getCtype(self):
    #returns the c data type of array, onliy ints and double currently.
    raise NotImplementedError("baseArray::getCtype")
  cpdeef reserve(self, int size):
    #resizes data to required size.
    raise NotImplementedError("baseArray::reserve")
  cpdef resize(self, int size):
    #resizes internal data to required size
    raise NotImplementedError("baseArray::resize")
  cpdef np.ndarray getNumPyArray(self):
    #returns the numpy array of the data, deos not keeps its reference
    return self._npy_array
  cpdef squeeze(self):
    #release unused memory
    raise NotImplementedError("baseArray::squeeze")

  cpdef remove(self, np.ndarray indexList, bint sortedInput=0):
    #remove particles with given indices in indexList
    raise NotImplementedError("baseArray::remove")
  cpdef extend(self, np.ndarray inputArray):
    #Extend array with data from inputArray
    raise NotImplementedError("baseArray::extend")
  cpdef alignArray(self, np.ndarray newIndices):
    #rearrange array contents according to new indices
    raise NotImplementedError("baseArray::alignArray")
  cpdef reset(self):
    #reset the lenght of the array to 0
    cdef PyArrayObject* arr = <PyArrayObject*> self._npy_array
    self.length = 0
    arr.dimensions[0] = self.length
  cpdef shrink(self, int size):
    #reset length of array to give size
    cdef PyArrayObject* arr = <PyArrayObject*> self._npy_array
