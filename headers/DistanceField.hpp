#ifndef DISTANCE_FIELD_HPP
#define DISTANCE_FIELD_HPP

#include "Point3D.hpp"
#include "../src/Operations.cpp"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class DistanceField{
  
private:
  Point3D ***field;
  int resolution;
  
public:

  DistanceField();
  void calculateDistanceField(vector<Point3D>, vector<Point3D> normals, int resolution);
  void recursiveClosestDistance(vector<Point3D>,  Point3D *point);
  Point3D*** getField();
  float sgn(Point3D , Point3D, Point3D);
  ~DistanceField();  
  
};

#endif