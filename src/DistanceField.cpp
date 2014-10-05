#include "../headers/DistanceField.hpp"
#include "../headers/Prism.hpp"
#include "../headers/BoundingBox.hpp"
#include <float.h>
#include <iostream>

using namespace std;

Operations o;

DistanceField::DistanceField(){
  
}


DistanceField::~DistanceField(){
  
  for(int i = 0; i != resolution; ++i){
    for(int j = 0; j != resolution; ++j){
        
        delete[] field[i][j];
    }
    delete[] field[i];
  }
  
  delete[] field;

}


void DistanceField::recursiveClosestDistance(vector<Point3D> points,  Point3D *point){
  
  Point3D P(point->getX() , point->getY(), point->getZ());
  
  int i = 0;
  
  while(i < points.size()){
    
    float temp = o.pointToTriangleDistance(P , points.at(i) , points.at(i+1),points.at(i+2));
    
    if(fabs(temp) < fabs(point->getDistance())){
      point->setDistance(temp);
    }
    
    i = i + 3;
    
  }
  
}
/*
void DistanceField::calculateDistanceField(vector<Point3D> points , int resolution){
  
  this->resolution = resolution;
  
  field = new Point3D**[resolution];
    
  for(int i =0; i<resolution; i++){
      field[i] = new Point3D*[resolution];
    for(int j =0; j<resolution; j++){
	field[i][j] = new Point3D[resolution];
    }
  }
  
  float step = 1.0f / (float) resolution;
  
  float point = -0.5f;
  
  cout<<"Calculating distance field"<<flush;
  
  for (int i = 0 ; i < resolution ; i++){
    cout<<"."<<flush;
    for (int j = 0 ; j < resolution ; j++){
      for (int k = 0 ; k < resolution ; k++){
	
	Point3D temp(point + step * i, point + step * j, point + step * k);
	temp.setDistance(FLT_MAX);
	recursiveClosestDistance(points, &temp);
	field[i][j][k] = temp;
//	o.printPoint(temp);
      }
      
    }

  }
}*/


float DistanceField::sgn(Point3D P, Point3D Q, Point3D N){
  
  if(P.minus(Q).dot(N) < 0){
    return -1.f;
  }
  else{
    return 1.f;
  }
  
}


void DistanceField::calculateDistanceField(vector<Point3D> points , vector<Point3D> normals,  int resolution){
   this->resolution = resolution;
  
  field = new Point3D**[resolution];
    
  for(int i =0; i<resolution; i++){
      field[i] = new Point3D*[resolution];
    for(int j =0; j<resolution; j++){
	field[i][j] = new Point3D[resolution];
    }
  }
  
  float step = 1.0f / (float) resolution;
  
  float point = -0.5f;
  
  for (int i = 0 ; i < resolution ; i++){
    
    for (int j = 0 ; j < resolution ; j++){
      for (int k = 0 ; k < resolution ; k++){
	
	Point3D temp(point + step * i, point + step * j, point + step * k);
	temp.setDistance(1.f);
	field[i][j][k] = temp;
      }
      
    }

  }
  
  
  int n = 0;
  
  while(n < points.size()){

     Prism prism = o.calculatePrism(points[n] , points[n+1] , points[n+2], normals[n],normals[n+1],normals[n+2], 0.03);
     
     BoundingBox box = o.calculateBoundingBox(prism);
    
      for (int i = 0 ; i < resolution ; i++){ 
	for (int j = 0 ; j < resolution ; j++){
	  for (int k = 0 ; k < resolution ; k++){
		
	    if(o.isInsideBox(box, field[i][j][k])){
	      float dis = o.pointToTriangleDistance(field[i][j][k] , points[n] , points[n+1], points[n+2]);
	      if(fabs(dis) < fabs(field[i][j][k].getDistance())){
		field[i][j][k].setDistance(dis);
	      }
	    }
	}
      }
    }
    
    n += 3;
  }
  
  
}


Point3D*** DistanceField::getField(){
  return field;
}