#include "../headers/Point3D.hpp"


Point3D::Point3D(float x , float y , float z){
  
  this->x = x;
  this->y = y;
  this->z = z;
  distance = 0.f;
}

Point3D::Point3D(){
  
 x=0.f;
 y=0.f;
 z=0.f;
 distance = 0.f; 
}

void Point3D::setXYZ(float x , float y , float z){
  
  this->x = x;
  this->y = y;
  this->z = z;
  
}

void Point3D::setDistance(float distance){
  
  this->distance = distance;
}

void Point3D::setX(float x){
  this->x = x;
}


void Point3D::setY(float y){
  this->y = y;
}

void Point3D::setZ(float z){
  this->z = z;
}


float Point3D::getX(){
  
  return x;
}

float Point3D::getY(){
  
  return y;
}


float Point3D::getZ(){
  
  return z;
}

float Point3D::getDistance(){
 
  return distance;
}

Point3D Point3D::minus(Point3D point){
  
  Point3D temp(this->x-point.getX() , this->y-point.getY(), this->z-point.getZ());
  
  return temp;
}

float Point3D::dot(Point3D p){

  float temp = x*p.getX()+y*p.getY()+z*p.getZ();
  
  return temp;
}

Point3D Point3D::crossProduct(Point3D p1){
  
  double x1 = p1.getY() * z - y * p1.getZ();
  double y1 = p1.getZ() * x - z * p1.getX();
  double z1 = p1.getX() * y - x * p1.getY();
  
  Point3D result(x1,y1,z1);
  
  return result;
}

void Point3D::Normalize(){
  
  float length = sqrt(x*x+y*y+z*z);
  
  x = x / length;
  y = y / length;
  z = z / length;
}

void Point3D::update_point(float nx, float ny, float nz){
  
  x += nx;
  y += ny;
  z += nz;
  
}

Point3D::~Point3D(){
  
}