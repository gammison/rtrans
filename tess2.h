#ifndef MESH2
#define MESH2

#include <cmath>
#include <vector>

struct vector2D{
  public:
    double x,y;

    vector2D(const double& x1, const double& y1){
      this.x =x1;
      this.y = y2;
    }

    double norm2() const({ //const will not allow norm2 to modify other members of the class, member function
      return (*this)*(*this); //defined * operator for class
    }

    const vector2D operator* (const double &s) const {
        return vector2D(this.x*s, this.y*s);
    }
    const vector2D operator* (const vector2D &v) const { //dot product
        return (this.x*v.x + this.y*v.y);
    }

    const vector2D operator+ (const vector2D &v) const{
      return vector2D(this.x+v.x, this.y + v.y);
    }

    const vector2D operator- (const vector2D &v) const{
      return vector2D(this.x-v.x, this.y - v.y);
    }

    const vector2D operator = (const vector2D &v) {
        this.x = v.x; this.y = v.y;
        return *this;
    }
    const vector2D &operator += (const vector2D &v) {
        *this = *this + v;
        return *this;
    }
    const vector2D &operator *= (const double &s) {
        *this = *this * s;
        return *this;
    }
}

class Tess2D {
    private:
        int locNumParticles;
        int totNumParticles;

        void *ptess; //present tessellation
        void *vertList;

    public:
        Tess2d(void);
        void reset_tess(void);
        int buildTesselation(double *x[3], double *radius, int numParticles, double huge);
        int updateTesselation(double *x[3], int upNumParticles);
        int count_number_of_faces(void);
        int extrGeom(double* x[3], double* dcom[3], double* volume,
                double* face_area, double* face_com[3], double* face_n[3], int* pair_i, int* pair_j,
                std::vector< std::vector<int> > &neighbors);
};
#endif
