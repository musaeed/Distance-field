#include "../headers/BVH.hpp"
#include "Operations.cpp"

BVH::BVH(){
  
  b1 = NULL;
  b2 = NULL;
  b3 = NULL;
  b4 = NULL;
  
}

void BVH::setPoints(vector<Point3D> points){
  this->points = points;
}

void BVH::setTriangles(vector<Triangle> triangles){
  this->triangles = triangles;
}

vector<Point3D> BVH::getPoints(){
  return points;
}

vector<Triangle> BVH::getTriangles(){
 return triangles; 
}

void BVH::setBoundingBox(BoundingBox b){
  box = b;
}

BoundingBox BVH::getBoundingBox(){
 return box; 
}

void BVH::allocate_memory(){
 
 b1 = new BVH; 
 b2 = new BVH;
 b3 = new BVH;
 b4 = new BVH;
  
}

BVH::~BVH(){
 
  if(b1 != NULL && b2 != NULL && b3 != NULL &&  b4 != NULL){
    
    
    delete b1;
    delete b2;
    delete b3;
    delete b4;
    
    b1 = NULL;
    b2 = NULL;
    b3 = NULL;
    b4 = NULL;
    
  }
  
}

BoundingBox calculateBox(vector<Point3D> points){
  
  Point3D min(FLT_MAX, FLT_MAX, FLT_MAX), max(-500.f, -500.f, -500.f);
  
  for(int i = 0 ; i < points.size() ; i++){
      
      if(points[i].getX() < min.getX()) min.setX(points[i].getX());
      if(points[i].getY() < min.getY()) min.setY(points[i].getY());
      if(points[i].getZ() < min.getZ()) min.setZ(points[i].getZ());
      
      if(points[i].getX() > max.getX()) max.setX(points[i].getX());
      if(points[i].getY() > max.getY()) max.setY(points[i].getY());
      if(points[i].getZ() > max.getZ()) max.setZ(points[i].getZ());
  }
  
    Point3D v1(min.getX(), max.getY(), min.getZ());
    Point3D v2(max.getX(), max.getY(), min.getZ());
    Point3D v3(min.getX(), min.getY(), min.getZ());
    Point3D v4(max.getX(), min.getY(), min.getZ());
    Point3D v5(min.getX(), max.getY(), max.getZ());
    Point3D v6(max.getX(), max.getY(), max.getZ());
    Point3D v7(min.getX(), min.getY(), max.getZ());
    Point3D v8(max.getX(), min.getY(), max.getZ());
    
    BoundingBox box(v1,v2,v3,v4,v5,v6,v7,v8);
    box.setMin(min);
    box.setMax(max);
    
    return box;
}


Point3D BVH::mid_point(Point3D a , Point3D b){
  
  Point3D result((a.getX() + b.getX()) / 2.f , (a.getY() + b.getY()) / 2.f, (a.getZ() + b.getZ()) / 2.f);
  
  return result;
}

void BVH::create_box(BoundingBox box){
  
  Point3D M12 = mid_point(box.getV1(),box.getV2());
  Point3D M34 = mid_point(box.getV3(),box.getV4());
  Point3D M56 = mid_point(box.getV5(),box.getV6());
  Point3D M78 = mid_point(box.getV7(),box.getV8());
  Point3D M24 = mid_point(box.getV2(),box.getV4());
  Point3D M68 = mid_point(box.getV6(),box.getV8());
  Point3D M13 = mid_point(box.getV1(),box.getV3());
  Point3D M57 = mid_point(box.getV5(),box.getV7());
  Point3D M12_34 = mid_point(M12,M34);
  Point3D M56_78 = mid_point(M56,M78);
  
  BoundingBox bb1(box.getV1() , M12, M13, M12_34, box.getV5(), M56, M57, M56_78);
  bb1.setMin(M13);
  bb1.setMax(M56);
  
  BoundingBox bb2(M13, M12_34, box.getV3(), M34, M57, M56_78, box.getV7(), M78);
  bb2.setMin(box.getV3());
  bb2.setMax(M56_78);
  
  BoundingBox bb3(M12, box.getV2(), M12_34, M24, M56, box.getV6(), M56_78, M68);
  bb3.setMin(M12_34);
  bb3.setMax(box.getV6());
  
  BoundingBox bb4(M12_34, M24, M34, box.getV4(), M56_78, M68, M78, box.getV8());
  bb4.setMin(M34);
  bb4.setMax(M68);
  
  b1->setBoundingBox(bb1);
  b2->setBoundingBox(bb2);
  b3->setBoundingBox(bb3);
  b4->setBoundingBox(bb4);
  
}


void BVH::calculateBVH(vector<Point3D> points, int depth){
  
  box = calculateBox(points);
  
  allocate_memory();
  
  b1->allocate_memory();
  b2->allocate_memory();
  b3->allocate_memory();
  b4->allocate_memory();
  
  create_box(box);
  
  b1->create_box(b1->getBoundingBox());
  b2->create_box(b2->getBoundingBox());
  b3->create_box(b3->getBoundingBox());
  b4->create_box(b4->getBoundingBox());
    
}

bool BVH::isInsideBox(BoundingBox box, Point3D point){
    
    if(point.getX()< box.getMin().getX()) return false;
    if(point.getY()< box.getMin().getY()) return false;
    if(point.getZ()< box.getMin().getZ()) return false;
    
    if(point.getX()> box.getMax().getX()) return false;
    if(point.getY()> box.getMax().getY()) return false;
    if(point.getZ()> box.getMax().getZ()) return false;
    
    return true;
  }

void BVH::allot_heirarchy(BVH *bvh , vector<Triangle> triangles){
  
  BoundingBox box = bvh->getBoundingBox();
  
  for(int i = 0; i < triangles.size() ; i++){
    
    if(isInsideBox(box, triangles[i].x) || isInsideBox(box, triangles[i].y) || isInsideBox(box, triangles[i].z)){
      
      bvh->triangles.push_back(triangles[i]);
      
    }
    
  }
  
  
  
  
}
  
 void BVH::give_points(){
   
    allot_heirarchy(b1->b1 , triangles);
    allot_heirarchy(b1->b2 , triangles);
    allot_heirarchy(b1->b3 , triangles);
    allot_heirarchy(b1->b4 , triangles);
    
    allot_heirarchy(b2->b1 , triangles);
    allot_heirarchy(b2->b2 , triangles);
    allot_heirarchy(b2->b3 , triangles);
    allot_heirarchy(b2->b4 , triangles);
    
    allot_heirarchy(b3->b1 , triangles);
    allot_heirarchy(b3->b2 , triangles);
    allot_heirarchy(b3->b3 , triangles);
    allot_heirarchy(b3->b4 , triangles);
    
    allot_heirarchy(b4->b1 , triangles);
    allot_heirarchy(b4->b2 , triangles);
    allot_heirarchy(b4->b3 , triangles);
    allot_heirarchy(b4->b4 , triangles);
  
 }

 
void BVH::update_points(float x, float y, float z){
  
  for(int i = 0 ; i < points.size() ; i++){
    
    points[i].setXYZ(points[i].getX() + x , points[i].getY() + y, points[i].getZ() + z);
    
  }
  
  for(int i = 0 ; i < triangles.size() ; i++){
    
    triangles[i].update_triangle(x,y,z);
    
  }
  
}

void BVH::update_heirarchy(float x, float y, float z){
  
  box.update_box(x,y,z);
  
  b1->box.update_box(x,y,z);
  b2->box.update_box(x,y,z);
  b3->box.update_box(x,y,z);
  b4->box.update_box(x,y,z);
  
  b1->b1->box.update_box(x,y,z);
  b1->b2->box.update_box(x,y,z);
  b1->b3->box.update_box(x,y,z);
  b1->b4->box.update_box(x,y,z);
  
  b1->b1->update_points(x,y,z);
  b1->b2->update_points(x,y,z);
  b1->b3->update_points(x,y,z);
  b1->b4->update_points(x,y,z);
  
  b2->b1->box.update_box(x,y,z);
  b2->b2->box.update_box(x,y,z);
  b2->b3->box.update_box(x,y,z);
  b2->b4->box.update_box(x,y,z);
  
  b2->b1->update_points(x,y,z);
  b2->b2->update_points(x,y,z);
  b2->b3->update_points(x,y,z);
  b2->b4->update_points(x,y,z);
  
  b3->b1->box.update_box(x,y,z);
  b3->b2->box.update_box(x,y,z);
  b3->b3->box.update_box(x,y,z);
  b3->b4->box.update_box(x,y,z);
  
  b3->b1->update_points(x,y,z);
  b3->b2->update_points(x,y,z);
  b3->b3->update_points(x,y,z);
  b3->b4->update_points(x,y,z);
  
  b4->b1->box.update_box(x,y,z);
  b4->b2->box.update_box(x,y,z);
  b4->b3->box.update_box(x,y,z);
  b4->b4->box.update_box(x,y,z);
  
  b4->b1->update_points(x,y,z);
  b4->b2->update_points(x,y,z);
  b4->b3->update_points(x,y,z);
  b4->b4->update_points(x,y,z);
  
}

