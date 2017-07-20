cimport numpy as np

cdef class baseArray

cdef class baseArrayIterator:
  cdef baseArray array
  cdef int index

cdef class baseArray:
  """template class for other data type arrays"""
