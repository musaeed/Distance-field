#include "../headers/STLParser.hpp"
#include <iostream>
#include "Operations.cpp"

using namespace std;

STLParser::STLParser(){
  
  
}


int STLParser::loadData(const char* path){
  
	
  
	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return 0;
	}
	return 1;
}


const aiScene* STLParser::getScene(){
 return scene; 
}

aiVector3D STLParser::getScene_min(){
 return scene_min; 
}

aiVector3D STLParser::getScene_max(){
 return scene_max; 
}

aiVector3D STLParser::getScene_center(){
 return scene_center; 
}

void STLParser::get_bounding_box ( aiVector3D* min,  aiVector3D* max)
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}

void STLParser::get_bounding_box_for_node (const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo)
{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

void STLParser::setPointVector( const struct aiNode* nd ){

  for (int n = 0 ; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    
    for (int t = 0; t < mesh->mNumFaces; ++t) {
      
      const struct aiFace* face = &mesh->mFaces[t];
       
	Point3D p1(mesh->mVertices[face->mIndices[0]].x * -0.0164194, (mesh->mVertices[face->mIndices[0]].z + scene_center.z + 13.0) * 0.0164194,mesh->mVertices[face->mIndices[0]].y * 0.0164194- 0.025);
	Point3D p2(mesh->mVertices[face->mIndices[1]].x * -0.0164194, (mesh->mVertices[face->mIndices[1]].z + scene_center.z + 13.0) * 0.0164194,mesh->mVertices[face->mIndices[1]].y * 0.0164194- 0.025);
	Point3D p3(mesh->mVertices[face->mIndices[2]].x * -0.0164194, (mesh->mVertices[face->mIndices[2]].z + scene_center.z + 13.0) * 0.0164194,mesh->mVertices[face->mIndices[2]].y * 0.0164194 - 0.025);
	
	Point3D n1(mesh->mNormals[face->mIndices[0]].x * -60.9036 , mesh->mNormals[face->mIndices[0]].z * 60.9036, mesh->mNormals[face->mIndices[0]].y  * 60.9036);
	Point3D n2(mesh->mNormals[face->mIndices[1]].x * -60.9036 , mesh->mNormals[face->mIndices[1]].z * 60.9036, mesh->mNormals[face->mIndices[1]].y  * 60.9036);
	Point3D n3(mesh->mNormals[face->mIndices[2]].x * -60.9036 , mesh->mNormals[face->mIndices[2]].z * 60.9036, mesh->mNormals[face->mIndices[2]].y  * 60.9036);
	
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	
	n1.Normalize();
	n2.Normalize();
	n3.Normalize();
	
	normals.push_back(n1);
	normals.push_back(n2);
	normals.push_back(n3);
	
    }
 }
  
  for (int n = 0; n < nd->mNumChildren; ++n) {
		setPointVector(nd->mChildren[n]);
  }
  
}


void STLParser::setLowerJaw(const struct aiNode* nd , const struct aiScene* scene){
  
   for (int n = 0 ; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    
    for (int t = 0; t < mesh->mNumFaces; ++t) {
      const struct aiFace* face = &mesh->mFaces[t];
       
	Point3D p1(mesh->mVertices[face->mIndices[0]].x *0.0141845, mesh->mVertices[face->mIndices[0]].z * 0.0141845 + 0.13,mesh->mVertices[face->mIndices[0]].y *0.0141845);
	Point3D p2(mesh->mVertices[face->mIndices[1]].x *0.0141845, mesh->mVertices[face->mIndices[1]].z * 0.0141845+ 0.13,mesh->mVertices[face->mIndices[1]].y *0.0141845);
	Point3D p3(mesh->mVertices[face->mIndices[2]].x *0.0141845, mesh->mVertices[face->mIndices[2]].z * 0.0141845+ 0.13,mesh->mVertices[face->mIndices[2]].y *0.0141845);
	
	Point3D n1(mesh->mNormals[face->mIndices[0]].x * -70.4995, mesh->mNormals[face->mIndices[0]].z * 70.4995, mesh->mNormals[face->mIndices[0]].y  * 70.4995);
	Point3D n2(mesh->mNormals[face->mIndices[1]].x * -70.4995, mesh->mNormals[face->mIndices[1]].z * 70.4995, mesh->mNormals[face->mIndices[1]].y * 70.4995);
	Point3D n3(mesh->mNormals[face->mIndices[2]].x * -70.4995, mesh->mNormals[face->mIndices[2]].z * 70.4995, mesh->mNormals[face->mIndices[2]].y * 70.4995);
      
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	
	n1.Normalize();
	n2.Normalize();
	n3.Normalize();
	
	normals.push_back(n1);
	normals.push_back(n2);
	normals.push_back(n3);
	
    }
 }
  
  for (int n = 0; n < nd->mNumChildren; ++n) {
		setLowerJaw(nd->mChildren[n] , scene);
  }
  
}

void STLParser::setBear(const struct aiNode* nd){
  
    for (int n = 0 ; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    
    for (int t = 0; t < mesh->mNumFaces; ++t) {
      const struct aiFace* face = &mesh->mFaces[t];
       
	Point3D p1(mesh->mVertices[face->mIndices[0]].x *0.00942867 - 71.4631*0.00942867, mesh->mVertices[face->mIndices[0]].y * 0.00942867 + 104.8957* 0.00942867,mesh->mVertices[face->mIndices[0]].z *0.00942867 + 21.5687*0.00942867);
	Point3D p2(mesh->mVertices[face->mIndices[1]].x *0.00942867 -71.4631*0.00942867, mesh->mVertices[face->mIndices[1]].y * 0.00942867 + 104.8957* 0.00942867,mesh->mVertices[face->mIndices[1]].z *0.00942867+ 21.5687*0.00942867);
	Point3D p3(mesh->mVertices[face->mIndices[2]].x *0.00942867 -71.4631*0.00942867, mesh->mVertices[face->mIndices[2]].y * 0.00942867 + 104.8957* 0.00942867,mesh->mVertices[face->mIndices[2]].z *0.00942867+ 21.5687*0.00942867);
	
	Point3D n1(mesh->mNormals[face->mIndices[0]].x , mesh->mNormals[face->mIndices[0]].y, mesh->mNormals[face->mIndices[0]].z);
	Point3D n2(mesh->mNormals[face->mIndices[1]].x , mesh->mNormals[face->mIndices[1]].y, mesh->mNormals[face->mIndices[1]].z);
	Point3D n3(mesh->mNormals[face->mIndices[2]].x , mesh->mNormals[face->mIndices[2]].y, mesh->mNormals[face->mIndices[2]].z);
      
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	
	normals.push_back(n1);
	normals.push_back(n2);
	normals.push_back(n3);
	
		
	Triangle temp(p1,p2,p3);
	temp.setNormals(n1,n2,n3);
	
	triangles.push_back(temp);
	
    }
 }
  
  for (int n = 0; n < nd->mNumChildren; ++n) {
		setBear(nd->mChildren[n]);
  }
  
}


void STLParser::setTriangle(){
  
  Point3D p1(-0.5,-0.5,0.0);
  Point3D p2(-0.25,0.25,0.0);
  Point3D p3(0.3,0.5,0.0);
  
  Point3D n1(0.f,0.f,0.5f);
  Point3D n2(0.f,0.f,0.5f);
  Point3D n3(0.f,0.f,0.5f);
  
  points.push_back(p1);
  points.push_back(p2);
  points.push_back(p3);
  
  normals.push_back(n1);
  normals.push_back(n2);
  normals.push_back(n3);
  
}


vector<Point3D> STLParser::getPointVector(){
 return points; 
}

vector<Point3D> STLParser::getNormalVector(){
 return normals; 
}

vector<Triangle> STLParser::getTriangles(){
 return triangles; 
}

void STLParser::setPoints(vector<Point3D> p){
  points = p;
}

void STLParser::setNormals(vector<Point3D> p){
  normals = p;
}

STLParser::~STLParser(){
  points.clear();
}