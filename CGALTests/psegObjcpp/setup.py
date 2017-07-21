from distutils.core import setup
from Cython.Build import cythonize
#
setup(ext_modules = cythonize(
           "psegObj.pyx",                 # our Cython source
           sources=["psegObj.cpp"],  # additional source file(s)
           language="c++"     # generate C++ code
      ))
