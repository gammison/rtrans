# distutils: language=c++
# distutils: sources = psesgObj.cpp

cdef class pyPSegObj:
  cdef psegObj c_seg
  def __cinit__(self, *args):#expects tuple of tuples for the 3 points  #can only expose simple C types, probably can't do arbitrary ones
    c_seg = new psegObj(Point2(args[0][0],args[0][1]),Point2(args[1][0],args[1][1]),Point2[args[2][0],args[2][1]])
  def isColin():
    self.c_seg.isColin()
