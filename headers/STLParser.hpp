#ifndef STLPARSER_HPP
#define STLPARSER_HPP

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <vector>
#include "Point3D.hpp"
#include "Triangle.hpp"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

using namespace std;

class STLParser{
  
private:
  const struct aiScene* scene = NULL;
  aiVector3D scene_min, scene_max, scene_center;
  vector<Point3D> points,normals;
  vector<Triangle> triangles;
  
public:
  
  STLParser();
  const aiScene* getScene();
  aiVector3D getScene_min();
  aiVector3D getScene_max();
  aiVector3D getScene_center();
  int loadData(const char* path);
  void get_bounding_box_for_node (const struct aiNode* nd, aiVector3D* min,  aiVector3D* max, aiMatrix4x4* trafo);
  void get_bounding_box ( aiVector3D* min, aiVector3D* max);
  void setPointVector( const struct aiNode* nd );
  vector<Point3D> getPointVector();
  vector<Point3D> getNormalVector();
  vector<Triangle> getTriangles();
  ~STLParser();
  void setLowerJaw( const struct aiNode* nd , const struct aiScene*);
  void setBear(const struct aiNode* nd);
  void setTriangle();
  void setPoints(vector<Point3D>);
  void setNormals(vector<Point3D>);
};

#endif