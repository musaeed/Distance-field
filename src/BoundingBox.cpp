#include "../headers/BoundingBox.hpp"
#include "Operations.cpp"

BoundingBox::BoundingBox(){
  
}


BoundingBox::BoundingBox(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, Point3D v6, Point3D v7, Point3D v8){
  
  this->v1 = v1;
  this->v2 = v2;
  this->v3 = v3;
  this->v4 = v4;
  this->v5 = v5;
  this->v6 = v6;
  this->v7 = v7;
  this->v8 = v8;
  
}

void BoundingBox::setAll(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Point3D v5, Point3D v6, Point3D v7, Point3D v8){
  
  this->v1 = v1;
  this->v2 = v2;
  this->v3 = v3;
  this->v4 = v4;
  this->v5 = v5;
  this->v6 = v6;
  this->v7 = v7;
  this->v8 = v8;
  
}

Point3D BoundingBox::getV1(){
 return v1; 
}

Point3D BoundingBox::getV3(){
 return v3; 
}

Point3D BoundingBox::getV2(){
 return v2; 
}

Point3D BoundingBox::getV4(){
 return v4; 
}

Point3D BoundingBox::getV5(){
 return v5; 
}

Point3D BoundingBox::getV6(){
 return v6; 
}

Point3D BoundingBox::getV7(){
 return v7; 
}

Point3D BoundingBox::getV8(){
 return v8; 
}

Point3D BoundingBox::getMin(){
 return min; 
}

Point3D BoundingBox::getMax(){
 return max; 
}

void BoundingBox::setV1(Point3D p){
  v1 = p;
}

void BoundingBox::setV2(Point3D p){
  v2 = p;
}

void BoundingBox::setV3(Point3D p){
  v3 = p;
}

void BoundingBox::setV4(Point3D p){
  v4 = p;
}

void BoundingBox::setV5(Point3D p){
  v5 = p;
}

void BoundingBox::setV6(Point3D p){
  v6 = p;
}

void BoundingBox::setV7(Point3D p){
  v7 = p;
}

void BoundingBox::setV8(Point3D p){
  v8 = p;
}
void BoundingBox::setMin(Point3D min){
  this->min = min;
}

void BoundingBox::setMax(Point3D max){
  this->max = max;
}

vector<Point3D> BoundingBox::getPointVector(){
  
  points.clear();
 
  points.push_back(v1);
  points.push_back(v2);
  points.push_back(v3);
  points.push_back(v4);
  points.push_back(v5);
  points.push_back(v6);
  points.push_back(v7);
  points.push_back(v8);
  
  return points;
}

void BoundingBox::print(){
 
  Operations op;
  op.printPoint(v1);
  op.printPoint(v2);
  op.printPoint(v3);
  op.printPoint(v4);
  op.printPoint(v5);
  op.printPoint(v6);
  op.printPoint(v7);
  op.printPoint(v8);
  
}

void BoundingBox::printMinMax(){
  Operations op;
  op.printPoint(max);
  op.printPoint(min);
}

void BoundingBox::update_box(float nx, float ny, float nz){
  
  v1.update_point(nx,ny,nz);
  v2.update_point(nx,ny,nz);
  v3.update_point(nx,ny,nz);
  v4.update_point(nx,ny,nz);
  v5.update_point(nx,ny,nz);
  v6.update_point(nx,ny,nz);
  v7.update_point(nx,ny,nz);
  v8.update_point(nx,ny,nz);
  
  min.update_point(nx,ny,nz);
  max.update_point(nx,ny,nz);
}

BoundingBox::~BoundingBox(){ 
  points.clear();
}