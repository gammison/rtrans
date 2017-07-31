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
    raise NotImplementedError("baseArray:reserve")
