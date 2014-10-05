#ifndef Triangle_HPP
#define Triangle_HPP

#include "Point3D.hpp"

class Triangle{
  
public:
  
  Point3D x;
  Point3D y;
  Point3D z;
  
  Point3D nx;
  Point3D ny;
  Point3D nz;
  
  Triangle(Point3D, Point3D, Point3D);
  void setNormals(Point3D, Point3D, Point3D);
  void update_triangle(float, float, float);
  ~Triangle();
  
};

#endif
