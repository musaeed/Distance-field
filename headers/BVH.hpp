#ifndef BVH_HPP
#define BVH_HPP

#include <vector>
#include "Point3D.hpp"
#include "BoundingBox.hpp"
#include "Triangle.hpp"

using namespace std;

class BVH{
  
private:
    


  vector<Point3D> points;
  vector<Triangle> triangles;
  
public:
  
  BVH* b1;
  BVH* b2;
  BVH* b3;
  BVH* b4;    
  BoundingBox box;
  
  BVH();
  void calculateBVH(vector<Point3D> , int depth);
  void setPoints(vector<Point3D>);
  void setTriangles(vector<Triangle>);
  void setBoundingBox(BoundingBox b);
  void allocate_memory();
  void create_box(BoundingBox);
  Point3D mid_point(Point3D,Point3D);
  BoundingBox getBoundingBox();
  vector<Point3D> getPoints();
  vector<Triangle> getTriangles();
  void allot_heirarchy(BVH* , vector<Triangle>);
  void give_points();
  void update_points(float, float, float);
  void update_heirarchy(float, float, float);
  bool isInsideBox(BoundingBox,Point3D);
  ~BVH();
  
};

#endif