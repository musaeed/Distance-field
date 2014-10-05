#ifndef OPERATIONS_CPP
#define OPERATIONS_CPP

#include <GL/glut.h>
#include "../headers/Point3D.hpp"
#include "../headers/Prism.hpp"
#include "../headers/BoundingBox.hpp"
#include "../headers/BVH.hpp"
#include <math.h>
#include <float.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>

typedef unsigned long long timestamp_t;

    

using namespace std;

const float  PI = 3.14159265358979f;

class Operations{
  
private:

  int count = 0;
public:

  Operations(){
  }
  
  float pointToTriangleDistance(Point3D P , Point3D x , Point3D y , Point3D z){
  
    Point3D B = x;
    Point3D E0 = y.minus(B);
    Point3D E1 = z.minus(B);
    
    Point3D D = B.minus(P);
    float a = E0.dot(E0);
    float b = E0.dot(E1);
    float c = E1.dot(E1);
    float d = E0.dot(D);
    float e = E1.dot(D);
    float f = D.dot(D);
    
    float det = a*c-b*b;
    float s = b*e-c*d;
    float t = b*d-a*e;
    
    if(s+t <= det){
      if(s < 0.0){
	if(t < 0.0){
	  if(d < 0.0){
	    t = 0.0;
	    if(-d >= a){
	      s = 1.0;
	    }
	    else{
	      s = -d/a;
	    }
	  }
	  else{
	      s = 0.0;
	      if(e >=0.0){
		t = 0.0;
	      }
	      else if(-e >= c){
		t=1.0;
	      }
	      else{
		t = -e/c;
	      }
	  }
	}
	else{
	  //region 3
	  
	  s=0.0;
	  if(e >= 0.0){
	    t = 0.0;
	  }
	  else if(-e >= c){
	    t = 1.0;
	  }
	  else{
	    t = -e/c;
	  }
	}
      }
      else if(t < 0.0){
	//region 5
	
	t=0.0;
	if(d >= 0.0){
	  s = 0.0;
	}
	else if(-d >= a){
	  s = 1.0;
	}
	else{
	  s = -d/a;
	}
      }
      else{
	//region 0
	s*= (1.0/det);
	t*= (1.0/det);
	
      }
    }
    else{
      //region 2
      if(s < 0.0){
	float temp0 = b+d;
	float temp1 = c+e;
	
	if(temp1 > temp0){
	  float numer = temp1 - temp0;
	  float denom = a - 2.0*b+c;
	  
	  if(numer >= denom){
	    s = 1.0;
	  } else{
	    s = numer / denom;
	  }
	  t = 1.0 - s;
	}
	else{
	  s = 0.0;
	  if(temp1 <= 0.0){
	    t = 1.0;
	  } else if(e >= 0.0){
	    t = 0.0;
	  }
	  else{
	    t = -e/c;
	  }
	}
	
      } else if(t < 0.0) {
	  //region 6
	  
	  float temp0 = a+d;
	  float temp1 = b+e;
	  
	  if(temp1 > temp0){
	    float numer = temp1 + temp0;
	    float denom = a - 2.0*b+c;
	    s = 1.0;
	    
	    if(numer >= denom){
	      s = numer / denom;
	    }
	    t = 1.0 - s;
	    
	  }
	  else{
	    t = 0.0;
	    
	    if(temp1 <= temp0){
	      s = 1.0;
	    } else if(e >= 0.0){
	      s = 0.0;
	    }
	    else{
	      s = -e/c;
	    }
	  }
      }
	  else{
	    //region 1
	    float numer = c+e-b-d;
	    if(numer <= 0.0){
	      s = 0.0;
	    }
	    else{
	      float denom = a-2.0*b+c;
	      if(numer >= denom){
		s = 1.0;
	      }
	      else{
		s = numer/denom;
	      }
	    }
	    t = 1.0 - s;
	  }
	}
	float dist = a*s*s + 2.0*b*s*t + c*t*t + 2.0*d*s + 2.0*e*t + f;
	
	Point3D n0 = E0.crossProduct(E1);
	
	n0.Normalize();
	
	float d0 = B.dot(n0);
	
	float check = P.dot(n0) - d0;
	
	dist = sqrt(dist);
	
	if(check < 0){
	  dist = -dist;
	}
	
	
	return dist;
      }
      
      
      static timestamp_t get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }
      
      
  void rescaleColor(float n){
    if(n < 0.0f){
      glColor3f(0.0,1.0,0.0);
    }
     else if(n > 0.f && n < 0.3f){
             glColor3f(1.0,0.0,0.0);
     }
    else{
      glColor3f(1.0,0.0,0.0);
    }
  }
  
  void printPoint(Point3D point){
    cout<<"X: "<<point.getX()<<" Y: "<<point.getY()<<" Z: "<<point.getZ()<<endl;
  }
  
  Point3D getTriangleNormal(Point3D n1 , Point3D n2 , Point3D n3){
   
    Point3D result((n1.getX() + n2.getX() + n3.getX()) / 3.f , (n1.getY() + n2.getY() + n3.getY()) / 3.f, (n1.getZ() + n2.getZ() + n3.getZ()) / 3.f);
    
    return result;
  }
  
  Prism calculatePrism(Point3D p1 , Point3D p2,Point3D p3,Point3D n1,Point3D n2,Point3D n3 , float epsilon){
    
    Point3D normal = getTriangleNormal(n1,n2,n3);
    
    Point3D p_1(p1.getX() + normal.getX() * epsilon, p1.getY() + normal.getY() * epsilon, p1.getZ() + normal.getZ() * epsilon);
    Point3D p_2(p1.getX() - normal.getX() * epsilon, p1.getY() - normal.getY() * epsilon, p1.getZ() - normal.getZ() * epsilon);
    Point3D p_3(p2.getX() + normal.getX() * epsilon, p2.getY() + normal.getY() * epsilon, p2.getZ() + normal.getZ() * epsilon);
    Point3D p_4(p2.getX() - normal.getX() * epsilon, p2.getY() - normal.getY() * epsilon, p2.getZ() - normal.getZ() * epsilon);
    Point3D p_5(p3.getX() + normal.getX() * epsilon, p3.getY() + normal.getY() * epsilon, p3.getZ() + normal.getZ() * epsilon);
    Point3D p_6(p3.getX() - normal.getX() * epsilon, p3.getY() - normal.getY() * epsilon, p3.getZ() - normal.getZ() * epsilon);
    
    Prism prism(p_1 , p_2, p_3, p_4, p_5, p_6 );
    
    return prism;
  }
  
  BoundingBox calculateBoundingBox(Prism prism){
    
    Point3D min(FLT_MAX, FLT_MAX, FLT_MAX), max(-500.f, -500.f, -500.f);
    vector<Point3D> points = prism.getPointVector();
    
    for(int i = 0; i < points.size() ; i++){
      
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
  
  bool isInsideBox(BoundingBox box, Point3D point){
    
    if(point.getX()< box.getMin().getX()) return false;
    if(point.getY()< box.getMin().getY()) return false;
    if(point.getZ()< box.getMin().getZ()) return false;
    
    if(point.getX()> box.getMax().getX()) return false;
    if(point.getY()> box.getMax().getY()) return false;
    if(point.getZ()> box.getMax().getZ()) return false;
    
    return true;
  }
  
  bool isIntersectBox(BoundingBox box, Point3D point){
    vector<Point3D> points = box.getPointVector();
    Point3D min = points[0], max = points[0];
    
    for(int i = 0 ; i < points.size() ; i++){
      
      if(points[i].getX() < min.getX()){
	min.setX(points[i].getX());
      }
      
      if(points[i].getY() < min.getY()){
	min.setY(points[i].getY());
      }
      
      if(points[i].getZ() < min.getZ()){
	min.setZ(points[i].getZ());
      }
      
      if(points[i].getX() > max.getX()){
	max.setX(points[i].getX());
      }
      
      if(points[i].getY() > max.getY()){
	max.setY(points[i].getY());
      }
      
      if(points[i].getZ() > min.getZ()){
	max.setZ(points[i].getZ());
      }
    }
    
    box.setMin(min);
    box.setMax(max);
    
    return isInsideBox(box , point);
  }
  
  float round(float d){
   
    d *= 10.f;
    
    return 0.1f * floor(d + 0.5);
  }
  
  bool isCollision(Point3D point, Point3D*** field , int resolution){
   
    float x = point.getX()+0.5f, y = point.getY()+0.5f, z = point.getZ()+0.5f;
    int a = (int)(x * resolution),b = (int)(y * resolution),c  = (int)(z * resolution);
   // float r = point.getX() - field[a][b][c].getX(), s=point.getY()-field[a][b][c].getY(), t = point.getZ()- field[a][b][c].getZ();
   // float res = 1.f/resolution;
    
/*    float distance = (res-r)*(res-s)*(res-t)*field[a][b][c].getDistance() + (res-r)*(res-s)*t*field[a][b][c+1].getDistance()
		    + (res-r)*s*(res-t)*field[a][b+1][c].getDistance() + (res-r)*s*t*field[a][b+1][c+1].getDistance()
		    + r*(res-s)*(res-t)*field[a+1][b][c].getDistance() + r*(res-s)*t*field[a+1][b][c+1].getDistance()
		    + r*s*(res-t)*field[a+1][b+1][c].getDistance() +  r*s*t*field[a+1][b+1][c+1].getDistance();
*/

    float distance = 1.f;
    Point3D p;
    
    if(field[a][b][c].getDistance() < distance){ distance = field[a][b][c].getDistance(); p = field[a][b][c];}
    if(field[a][b][c+1].getDistance() < distance){ distance = field[a][b][c+1].getDistance(); p = field[a][b][c+1];}
    if(field[a][b+1][c].getDistance() < distance){ distance = field[a][b+1][c].getDistance(); p = field[a][b+1][c];}
    if(field[a][b+1][c+1].getDistance() < distance){ distance = field[a][b+1][c+1].getDistance(); p = field[a][b+1][c+1];}
    if(field[a+1][b][c].getDistance() < distance){ distance = field[a+1][b][c].getDistance(); p = field[a+1][b][c];}
    if(field[a+1][b][c+1].getDistance() < distance){ distance = field[a+1][b][c+1].getDistance(); p = field[a+1][b][c+1];}
    if(field[a+1][b+1][c].getDistance() < distance){ distance = field[a+1][b+1][c].getDistance(); p = field[a+1][b+1][c];}
    if(field[a+1][b+1][c+1].getDistance() < distance){ distance = field[a+1][b+1][c+1].getDistance(); p = field[a+1][b+1][c+1];}
    
		    
/*    cout<<"start.."<<endl;
    printPoint(point);
    cout<<a<<" "<<b<<" "<<c<<endl;
    printPoint(field[a][b][c]);
    cout<<"trilinear: "<<distance<<endl;
    cout<<field[a][b][c].getDistance()<<endl;
    cout<<"end.."<<endl;
  */     
    if(distance < 0.f){
      return true;
    }
    
    return false;
    
    
  }
  
    bool isIntersectionMax(Point3D max, Point3D*** field){
    
    float x = max.getX(), y = max.getY(), z = max.getZ();
    
    int a = (int) ((x+0.5)*10),b = (int) ((y+0.5)*10),c = (int) ((z+0.5)*10);
    
    float r = (max.getX()*10 - floor(max.getX()*10)) / 10.f;
    float s = (max.getY()*10 - floor(max.getY()*10)) / 10.f;
    float t = (max.getZ()*10 - floor(max.getZ()*10)) / 10.f;
    
    float distance = (0.1-r)*(0.1-s)*(0.1-t)*field[a][b][c].getDistance() + (0.1-r)*(0.1-s)*t*field[a][b][c+1].getDistance()
		    + (0.1-r)*s*(0.1-t)*field[a][b+1][c].getDistance() + (0.1-r)*s*t*field[a][b+1][c+1].getDistance()
		    + r*(0.1-s)*(0.1-t)*field[a+1][b][c].getDistance() + r*(0.1-s)*t*field[a+1][b][c+1].getDistance()
		    + r*s*(0.1-t)*field[a+1][b+1][c].getDistance() +  r*s*t*field[a+1][b+1][c+1].getDistance();
		    
		    
    
    if(distance < 0.f){
      return true;
    }
    
    return false;
    
  }
  
  BoundingBox createLowerBox(float maxx, float maxy,float maxz, float minx, float miny, float minz){
    
    Point3D v1(minx, maxy, minz);
    Point3D v2(maxx, maxy, minz);
    Point3D v3(minx, miny, minz);
    Point3D v4(maxx, miny, minz);
    Point3D v5(minx, maxy, maxz+0.8);
    Point3D v6(maxx, maxy, maxz+0.8);
    Point3D v7(minx, miny, maxz+0.8);
    Point3D v8(maxx, miny, maxz+0.8);
    
    Point3D min(minx, miny, minz);
    Point3D max(maxx, maxy, maxz);
    
 /*   printPoint(v1);
    printPoint(v2);
    printPoint(v3);
    printPoint(v4);
    printPoint(v5);
    printPoint(v6);
    printPoint(v7);
    printPoint(v8);
 */   
    BoundingBox box(v1,v2,v3,v4,v5,v6,v7,v8);
    box.setMin(min);
    box.setMax(max);
    
    return box;
  }
  
  pair<Point3D,Point3D> findMinMaxPoint(vector<Point3D> points){
    
    Point3D min = points[0], max = points[0];
    
    for(int i = 0 ; i < points.size() ; i++){
      if(points[i].getX() < min.getX()){
	min.setX(points[i].getX());
      }
      
      if(points[i].getY() < min.getY()){
	min.setY(points[i].getY());
      }
      
      if(points[i].getZ() < min.getZ()){
	min.setZ(points[i].getZ());
      }
      
      if(points[i].getX() > max.getX()){
	max.setX(points[i].getX());
      }
      
      if(points[i].getY() > max.getY()){
	max.setY(points[i].getY());
      }
      
      if(points[i].getZ() > min.getZ()){
	max.setZ(points[i].getZ());
      }
    }
    
    pair<Point3D,Point3D> p(min,max);
    
    return p;
  }
  
  Point3D rotatePoint(float angle , Point3D p){
    
    angle *= PI/180.f;
    
    Point3D tmp(p.getX() , p.getY() * cos(angle) - p.getZ() * sin(angle) , p.getY() * sin(angle) + p.getZ() * cos(angle));
    return tmp;
  }
  
  BoundingBox rotateBox(BoundingBox box, float angle){
 
    BoundingBox boxq;
    
    boxq.setV1(rotatePoint(angle , box.getV1()));
    boxq.setV2(rotatePoint(angle , box.getV2()));
    boxq.setV3(rotatePoint(angle , box.getV3()));
    boxq.setV4(rotatePoint(angle , box.getV4()));
    boxq.setV5(rotatePoint(angle , box.getV5()));
    boxq.setV6(rotatePoint(angle , box.getV6()));
    boxq.setV7(rotatePoint(angle , box.getV7()));
    boxq.setV8(rotatePoint(angle , box.getV8()));
 
    return boxq;
  }
  
  pair<Point3D, Point3D> getMinMaxYPoints(vector<Point3D> points){
    
    Point3D min = points[0],max = points[0];
    
    for(int i = 0 ; i < points.size() ; i++){
      if(points[i].getY() < min.getY()){
	min.setXYZ(points[i].getX(),points[i].getY(),points[i].getZ());
      }
      if(points[i].getY() > max.getY()){
	max.setXYZ(points[i].getX(),points[i].getY(),points[i].getZ());
      }
    }
    
    pair<Point3D , Point3D> p(min,max);
    
    return p;
  }
  
  void draw_sphere_at_intersection(Point3D p){
    
         glPushMatrix();
	  
	  glTranslatef(p.getX() , p.getY() , p.getZ());
	  glutWireSphere(0.01, 10 , 10);
	 
	 glPopMatrix();
  }
  
  Point3D getBaycentricCoordinates(Point3D a , Point3D b, Point3D c , Point3D toCheck){
  
  Point3D v0 = b.minus(a);
  Point3D v1 = c.minus(a);
  Point3D v2 = toCheck.minus(a);
  
  double d00 = v0.dot(v0);
  double d01 = v0.dot(v1);
  double d11 = v1.dot(v1);
  double d20 = v2.dot(v0);
  double d21 = v2.dot(v1);
  
  double denom = d00 * d11 - d01 * d01;
  
  double b1 = (d11 * d20 - d01 * d21) / denom;
  double b2 = (d00 * d21 - d01 * d20) / denom;
  double b3 = 1.0 - b1 - b2;
    
  Point3D result(b1,b2,b3);
  
  return result;
}

  
  bool checkInside(Point3D point){
  bool result = true;
  
  if(point.getX() <= 0 || point.getX() >= 1)
    result &= false;
  if(point.getY() <= 0 || point.getY() >= 1)
    result &= false;
  if(point.getZ() <= 0 || point.getZ() >= 1)
    result &= false;
  
  return result;
}

  
  bool isPointInsideTriangle(Point3D toCheck , Point3D x, Point3D y, Point3D z){
   
  return checkInside(getBaycentricCoordinates(x , y , z , toCheck));
}
  
  
  Point3D findIntersection(Point3D init , Point3D v , Point3D p1, Point3D p2, Point3D p3){
  
  Point3D p12 = p1.minus(p2);

  Point3D p13 = p1.minus(p3);
  
  Point3D normal = p12.crossProduct(p13);
  
  double t = (normal.getX() * p1.getX() + normal.getY() * p1.getY() + normal.getZ() * p1.getZ() - normal.getX() * init.getX() - normal.getY() * init.getY() - normal.getZ() * init.getZ())/(normal.getX()*v.getX() + normal.getY() * v.getY() + normal.getZ() * v.getZ());
  
  Point3D result( init.getX() + t * v.getX() , init.getY() + t * v.getY() , init.getZ() + t * v.getZ()); 
  
  return result;
}

  
  void getIntersection(Point3D point, vector<Triangle> triangles , int state){
    
    Point3D center;

    if(state){
          center.setXYZ(0.0,0.f,0.5f);
    }
    else{
          center.setXYZ(0.0,0.f,-0.5f);
    }

    
    int i = 0;
   
    for(int i = 0 ; i < triangles.size() ; i++){
      
     Point3D temp = findIntersection(point, center, triangles[i].x ,triangles[i].y ,triangles[i].z);
	
	cout<<"Intersection: "<<flush;
	printPoint(temp);
	
	if(isPointInsideTriangle(temp , triangles[i].x ,triangles[i].y ,triangles[i].z)){
	  draw_sphere_at_intersection(temp);
	}
      
    }
    
  }
  
  
  vector<Triangle> rotateTriangles(vector<Triangle> triangles , float angle){
    
    vector<Triangle> nvc;
    
    for(int i = 0 ; i < triangles.size() ; i++){
      
      Triangle temp(rotatePoint(angle, triangles[i].x) , rotatePoint(angle, triangles[i].y), rotatePoint(angle, triangles[i].z));
      nvc.push_back(temp);
    }
    
    return nvc;    
  }
  
  void checkCollision(Point3D*** field, BVH* bvh , int res ){
    
    for(int i = 0 ; i < res ; i++){
     for(int j = 0 ; j < res ; j++){
       for(int k = 0 ; k < res ; k++){
	 
	 Point3D temp = field[i][j][k];
	 
	 if(isInsideBox(bvh->getBoundingBox() , temp)){
	   
	  if(temp.getDistance() < 0.f){
	    
	    if(isInsideBox(bvh->b1->getBoundingBox(), temp)){
	    
	      if(isInsideBox(bvh->b1->b1->getBoundingBox(), temp)){
		
		  getIntersection(temp , bvh->b1->b1->getTriangles() , 0);
		
	      }
	      else if(isInsideBox(bvh->b1->b2->getBoundingBox(), temp)){
		  getIntersection(temp , bvh->b1->b2->getTriangles() , 0);
	      }
	      else if(isInsideBox(bvh->b1->b3->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b1->b3->getTriangles(), 0);		
	      }
	      else if(isInsideBox(bvh->b1->b4->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b1->b4->getTriangles(), 0);		
	      }
	    }
	   else if(isInsideBox(bvh->b2->getBoundingBox(), temp)){
	    
	      if(isInsideBox(bvh->b2->b1->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b2->b1->getTriangles(), 0);		
	      }
	      else if(isInsideBox(bvh->b2->b2->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b2->b2->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b2->b3->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b2->b3->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b2->b4->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b2->b4->getTriangles(), 0);
	      }
	      
	    }
	    else if(isInsideBox(bvh->b3->getBoundingBox(), temp)){
	    
	      if(isInsideBox(bvh->b3->b1->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b3->b1->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b3->b2->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b3->b2->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b3->b3->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b3->b3->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b3->b4->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b4->b4->getTriangles(), 0);
	      }
	    }
	    else if(isInsideBox(bvh->b4->getBoundingBox(), temp)){
	    
	      if(isInsideBox(bvh->b4->b1->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b4->b1->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b4->b2->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b4->b2->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b4->b3->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b4->b3->getTriangles(), 0);
	      }
	      else if(isInsideBox(bvh->b4->b4->getBoundingBox(), temp)){
		getIntersection(temp , bvh->b4->b4->getTriangles(), 0);
	      }
	    }
	    	    
	   }
	 }
	 
       }
     }
    
  }

}
  
  
  void checkLowerCollision(Point3D*** field, BVH* bvh, int res, float angle){
    
    for(int i = 0 ; i < res ; i++){
     for(int j = 0 ; j < res ; j++){
       for(int k = 0 ; k < res ; k++){
	 
	 Point3D temp = field[i][j][k];
	 BoundingBox result = rotateBox(bvh->box , angle);
	 
	 if(isIntersectBox(result,temp)){
	   
	   if(temp.getDistance() < 0.f){
	     
	     if(isInsideBox(rotateBox(bvh->b1->getBoundingBox(), angle), temp)){
	    
	      if(isInsideBox(rotateBox(bvh->b1->b1->getBoundingBox(), angle), temp)){
		
		  getIntersection(temp , rotateTriangles(bvh->b1->b1->getTriangles() , angle),1);
		
	      }
	      else if(isInsideBox(rotateBox(bvh->b1->b2->getBoundingBox(), angle), temp)){
		  getIntersection(temp , rotateTriangles(bvh->b1->b2->getTriangles(),angle),1);
	      }
	      else if(isInsideBox(rotateBox(bvh->b1->b3->getBoundingBox(), angle), temp)){
		getIntersection(temp , rotateTriangles(bvh->b1->b3->getTriangles(),angle),1);		
	      }
	      else /*if(isInsideBox(rotateBox(bvh->b1->b4->getBoundingBox(), angle), temp))*/{
		getIntersection(temp , rotateTriangles(bvh->b1->b4->getTriangles(),angle),1);		
	      }
	    }
	    else if(isInsideBox(rotateBox(bvh->b2->getBoundingBox(), angle), temp)){
	    
	      if(isInsideBox(rotateBox(bvh->b2->b1->getBoundingBox(), angle), temp)){
		
		  getIntersection(temp , rotateTriangles(bvh->b2->b1->getTriangles(),angle),1);
		
	      }
	      else if(isInsideBox(rotateBox(bvh->b2->b2->getBoundingBox(), angle), temp)){
		  getIntersection(temp , rotateTriangles(bvh->b2->b2->getTriangles(),angle),1);
	      }
	      else if(isInsideBox(rotateBox(bvh->b2->b3->getBoundingBox(), angle), temp)){
		getIntersection(temp , rotateTriangles(bvh->b2->b3->getTriangles(),angle),1);		
	      }
	      else /*if(isInsideBox(rotateBox(bvh->b2->b4->getBoundingBox(), angle), temp))*/{
		getIntersection(temp , rotateTriangles(bvh->b2->b4->getTriangles(),angle),1);		
	      }
	    }
	    else if(isInsideBox(rotateBox(bvh->b3->getBoundingBox(), angle), temp)){
	    
	      if(isInsideBox(rotateBox(bvh->b3->b1->getBoundingBox(), angle), temp)){
		
		  getIntersection(temp , rotateTriangles(bvh->b3->b1->getTriangles(),angle),1);
		
	      }
	      else if(isInsideBox(rotateBox(bvh->b3->b2->getBoundingBox(), angle), temp)){
		  getIntersection(temp , rotateTriangles(bvh->b3->b2->getTriangles(),angle),1);
	      }
	      else if(isInsideBox(rotateBox(bvh->b3->b3->getBoundingBox(), angle), temp)){
		getIntersection(temp , rotateTriangles(bvh->b3->b3->getTriangles(),angle),1);		
	      }
	      else if(isInsideBox(rotateBox(bvh->b3->b4->getBoundingBox(), angle), temp)){
		getIntersection(temp , rotateTriangles(bvh->b3->b4->getTriangles(),angle),1);		
	      }
	    }
	     else if(isInsideBox(rotateBox(bvh->b4->getBoundingBox(), angle), temp)){
	    
	      if(isInsideBox(rotateBox(bvh->b4->b1->getBoundingBox(), angle), temp)){
		
		  getIntersection(temp , rotateTriangles(bvh->b4->b1->getTriangles(),angle),1);
		
	      }
	      else if(isInsideBox(rotateBox(bvh->b4->b2->getBoundingBox(), angle), temp)){
		  getIntersection(temp , rotateTriangles(bvh->b4->b2->getTriangles(),angle),1);
	      }
	      else if(isInsideBox(rotateBox(bvh->b4->b3->getBoundingBox(), angle), temp)){
		getIntersection(temp , rotateTriangles(bvh->b4->b3->getTriangles(),angle),1);		
	      }
	      else if(isInsideBox(rotateBox(bvh->b4->b4->getBoundingBox(), angle), temp)){
		getIntersection(temp , rotateTriangles(bvh->b4->b4->getTriangles(),angle),1);		
	      }
	    }
	    else{
	      getIntersection(temp ,bvh->b4->b4->getTriangles(),1 );
	    }
	     
	   } 
	   
	 }
	 
       }
     }
    }
	 
  }
  
  
};

#endif