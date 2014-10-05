#include "../headers/Triangle.hpp"


Triangle::Triangle(Point3D x, Point3D y, Point3D z){
  
  this->x = x;
  this->y = y;
  this->z = z;
  
}


void Triangle::setNormals(Point3D x, Point3D y, Point3D z){
  
  this->nx = x;
  this->ny = y;
  this->nz = z;
  
}

void Triangle::update_triangle(float x, float y, float z){
  
  this->x.update_point(x,y,z);
  this->y.update_point(x,y,z);
  this->z.update_point(x,y,z);
  
}

Triangle::~Triangle(){
  
}