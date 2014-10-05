#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "Point3D.hpp"
#include <vector>


using namespace std;

class BoundingBox{

private:
  
 Point3D  v1,v2,v3,v4,v5,v6,v7,v8;
 Point3D min,max;
 vector<Point3D> points;
 
public:
  
  BoundingBox();
  BoundingBox(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, Point3D v6, Point3D v7, Point3D v8);
  Point3D getV1();
  Point3D getV2();
  Point3D getV3();
  Point3D getV4();
  Point3D getV5();
  Point3D getV6();
  Point3D getV7();
  Point3D getV8();
  Point3D getMin();
  Point3D getMax();
  void setAll(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, Point3D v6, Point3D v7, Point3D v8);
  void setV1(Point3D p);
  void setV2(Point3D p);
  void setV3(Point3D p);
  void setV4(Point3D p);
  void setV5(Point3D p);
  void setV6(Point3D p);
  void setV7(Point3D p);
  void setV8(Point3D p);
  void setMin(Point3D min);
  void setMax(Point3D min);
  vector<Point3D> getPointVector();
  void print();
  void printMinMax();
  void update_box(float,float,float);
  ~BoundingBox();
};

#endif