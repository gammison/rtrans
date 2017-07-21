#include <iostream>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;



namespace test{
  class psegObj{
  public:
    Point_2 x,y,z;
    psegObj(Point_2 x1, Point_2 y1, Point_2 z1);
    ~psegObj();
    void isColin();
  };
}
