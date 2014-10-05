#ifndef PRISM_HPP
#define PRISM_HPP

#include "Point3D.hpp"
#include <vector>

using namespace std;

class Prism{
  
private:
  
  Point3D p1,p2,p3,p4,p5,p6;
  vector<Point3D> points;
  
public:
  
  Prism();
  Prism(Point3D p1,Point3D p2, Point3D p3, Point3D p4 ,Point3D p5 ,Point3D p6);
  Point3D getP1();
  Point3D getP2();
  Point3D getP3();
  Point3D getP4();
  Point3D getP5();
  Point3D getP6();
  vector<Point3D> getPointVector();
  void print();
  ~Prism();  
};

#endif