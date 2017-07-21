#include <iostream>
#include <CGAL/Simple_cartesian.h>
using namespace std;
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;
bool test();

bool test(){
  cout<<"yeet"<<endl;
  return true;
}
int main()
{
  Point_2 p(1,1), q(10,10);
  cout << "p = " << p << endl;
  cout << "q = " << q.x() << " " << q.y() << endl;
  cout << "sqdist(p,q) = "
            << CGAL::squared_distance(p,q) << endl;

  Segment_2 s(p,q);
  Point_2 m(5, 9);

  cout << "m = " << m << endl;
  cout << "sqdist(Segment_2(p,q), m) = "
            << CGAL::squared_distance(s,m) << endl;
  cout << "p, q, and m ";
  switch (CGAL::orientation(p,q,m)){
  case CGAL::COLLINEAR:
    cout << "are collinear\n";
    break;
  case CGAL::LEFT_TURN:
    cout << "make a left turn\n";
    break;
  case CGAL::RIGHT_TURN:
    cout << "make a right turn\n";
    break;
  }
  cout << " midpoint(p,q) = " << CGAL::midpoint(p,q) << test()<<endl;

    return 0;
}
