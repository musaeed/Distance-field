#ifndef POINT3D_HPP
#define POINT3D_HPP

#include <math.h>

class Point3D{
  
private:

  float x,y,z;
  float distance;
  
public:
  
  Point3D();
  Point3D(float x , float y, float z);
  float getX();
  float getY();
  float getZ();
  void setX(float x);
  void setY(float y);
  void setZ(float z);
  void setDistance(float distance);
  float getDistance();
  void setXYZ(float, float, float);
  Point3D minus(Point3D point);
  float dot(Point3D point);
  Point3D crossProduct(Point3D p1);
  void Normalize();
  void update_point(float,float,float);
  ~Point3D();
    
};

#endif