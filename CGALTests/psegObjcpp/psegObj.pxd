cdef extern "psegObj.h" from namespace "test":
  cdef cppclass psegObj:
    psegObj() except +
    psegObj(Point2, Point2, Point2) except +
    Point2 x,y,z
    void isColin()
     
