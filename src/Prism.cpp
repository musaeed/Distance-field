#include "../headers/Prism.hpp"
#include "Operations.cpp"

Prism::Prism(){
  
}


Prism::Prism(Point3D p1,Point3D p2,Point3D p3,Point3D p4,Point3D p5,Point3D p6 ){
  
  this->p1 = p1;
  this->p2 = p2;
  this->p3 = p3;
  this->p4 = p4;
  this->p5 = p5;
  this->p6 = p6;
  
}

Point3D Prism::getP1(){
 return p1; 
}

Point3D Prism::getP2(){
 return p2; 
}

Point3D Prism::getP3(){
 return p3; 
}

Point3D Prism::getP4(){
 return p4; 
}

Point3D Prism::getP5(){
 return p5; 
}

Point3D Prism::getP6(){
 return p6; 
}

vector<Point3D> Prism::getPointVector(){
  
  points.push_back(p1);
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);
  points.push_back(p6);
  
  return points;
}

void Prism::print(){
  Operations op;
  
  op.printPoint(p1);
  op.printPoint(p2);
  op.printPoint(p3);
  op.printPoint(p4);
  op.printPoint(p5);
  op.printPoint(p6);
 
}

Prism::~Prism(){
  
}