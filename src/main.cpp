#include <GL/glut.h>
#include<iostream>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../headers/STLParser.hpp"
#include "../headers/DistanceField.hpp"
#include "../headers/BVH.hpp"
#include "Operations.cpp"
#include <typeinfo>
#include <math.h>
#include <time.h>

using namespace std;


  Operations op;
  STLParser bear , ljaw , ujaw;
  DistanceField ufield,lfield;
  BVH bvh;
  GLuint scene_list_bear = 0 , scene_list_ljaw = 0;
  static float angle = 0.f , x = 0.0f, y = 0.0f , z = 0.0f, x12 = 0.0f, y12 = 0.0f , z12 = 0.0f;
  float x_pos = 0;
  float y_pos = 0;
  float z_pos = 0;
  float theta = M_PI/6;
  float phi  = M_PI/2;
  float R = 2;
  bool goDown = true, yes = true;
  static int counter = 0;
  Point3D bearMin(0.f,0.f,0.f),bearMax(0.f,0.f,0.f),minB,maxB;
  vector<Point3D> points;
  int res;
  
void color4_to_float4(const aiColor4D *color, float f[4])
{
	f[0] = color->r;
	f[1] = color->g;
	f[2] = color->b;
	f[3] = color->a;
}

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
  
  
  void apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else 
		glEnable(GL_CULL_FACE);
}
  
  
  void recursive_render (const struct aiScene *sc, const struct aiNode* nd , STLParser object)
{
	unsigned int i;
	unsigned int n = 0, t; 
	aiMatrix4x4 m = nd->mTransformation;
	

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = object.getScene()->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			glBegin(GL_TRIANGLES);

			for(i = 0; i < face->mNumIndices; i++) {
			 
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n], object);
	}

	glPopMatrix();
}

void do_motion (void)
{
	


	if(goDown){
	  angle -= 1.f;
	  if(angle < -30.0f){
	    goDown = false;
	  }
	}
	
	if(!goDown){
	  
	  angle +=1.f;
	  if(angle > 0.0){
	    goDown = true;
	  }
	}

	
	
	glutPostRedisplay ();
}
  
  void draw_bear(){
    
    glPushMatrix();
    float tmp;
    
    
	tmp = bear.getScene_max().x-bear.getScene_min().x;
	tmp = aisgl_max(bear.getScene_max().y - bear.getScene_min().y,tmp);
	tmp = aisgl_max(bear.getScene_max().z - bear.getScene_min().z,tmp);
	tmp = 1.f / tmp;
	
	tmp = tmp*0.2;

	glScalef(tmp , tmp , tmp);
	
	//bearMin.setXYZ(bear.getScene_min().x*tmp -bear.getScene_center().x * tmp, bear.getScene_min().y*tmp -(bear.getScene_center().y - 10.f + y)*tmp,bear.getScene_min().z*tmp-(bear.getScene_center().z - 40.f)*tmp);
	//bearMax.setXYZ(bear.getScene_max().x*tmp -bear.getScene_center().x * tmp,bear.getScene_max().y*tmp -(bear.getScene_center().y - 10.f + y)*tmp,bear.getScene_max().z*tmp-(bear.getScene_center().z - 40.f)*tmp);

	
	
        // center the model
	glTranslatef( -bear.getScene_center().x+x12*(1/tmp), -bear.getScene_center().y + 10.f + y12*(1/tmp), -bear.getScene_center().z + 40.f+z12*(1/tmp));
	
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(bear.getScene(), bear.getScene()->mRootNode , bear);
	
      glPopMatrix();
  }
  
   void draw_ljaw(){
     
     glPushMatrix();
    float tmp;
    
	tmp = ljaw.getScene_max().x-ljaw.getScene_min().x;
	tmp = aisgl_max(ljaw.getScene_max().y - ljaw.getScene_min().y,tmp);
	tmp = aisgl_max(ljaw.getScene_max().z - ljaw.getScene_min().z,tmp);
	tmp = 1.f / tmp;
	glScalef(tmp, tmp, tmp);
	
	glRotatef(90.0 , 1.0 , 0.0 ,0.0);
	glRotatef(180.0 , 0.0 , 1.0 ,0.0);
	glRotatef(angle , 1.0 , 0.0 ,0.0);
	
	//glScalef(0.1f, 0.1f, 0.1f);
	
        // center the model
	glTranslatef( -ljaw.getScene_center().x, -ljaw.getScene_center().y, -ljaw.getScene_center().z );

            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(ljaw.getScene(), ljaw.getScene()->mRootNode , ljaw);
	    
	    glPopMatrix();
    
  }
  
    void draw_ujaw(){
     
     glPushMatrix();
    float tmp;
    
	tmp = ujaw.getScene_max().x-ujaw.getScene_min().x;
	tmp = aisgl_max(ujaw.getScene_max().y - ujaw.getScene_min().y,tmp);
	tmp = aisgl_max(ujaw.getScene_max().z - ujaw.getScene_min().z,tmp);
	tmp = 1.f / tmp;
	glScalef(tmp, tmp, tmp);
	
	glRotatef(180.0 , 0.0 , 1.0 ,0.0);
	glRotatef(270.0 , 1.0 , 0.0 ,0.0);
	
	
        // center the model
	glTranslatef( -ujaw.getScene_center().x, -ujaw.getScene_center().y, -ujaw.getScene_center().z  + 15.0);

            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(ujaw.getScene(), ujaw.getScene()->mRootNode , ujaw);
	    
	    glPopMatrix();
    
  }
  
  void drawAxes()
{  
  
  glBegin( GL_LINES );
  glColor3f( 1., 0., 0. );
  glVertex3f( -50., 0., 0. );
  glVertex3f( 50., 0., 0. );
  glEnd();
  
  glBegin( GL_LINES );
  glColor3f( 0., 0., 1. );
  glVertex3f( 0., -50., 0. );
  glVertex3f( 0., 50., 0. );
  glEnd();
  
  glBegin( GL_LINES );
  glColor3f( 0., 1., 0. );
  glVertex3f( 0., 0., -50. );
  glVertex3f( 0., 0., 50. );
  glEnd();
  
  
}

void renderPoints(vector<Point3D> points , vector<Point3D> normals){
      glPushMatrix();
     
	glBegin(GL_TRIANGLES);
	  glColor3f(0.f,1.f,0.f);
	for(int i = 0 ; i < points.size() ; i++){
	  glNormal3f(normals.at(i).getX() ,normals.at(i).getY(),normals.at(i).getZ() );
	  glVertex3f(points.at(i).getX() ,points.at(i).getY(),points.at(i).getZ() );
	}
	glEnd();
      
      glPopMatrix();
}

void renderField(Point3D*** points , int res){
   glPushMatrix();
      
	glBegin(GL_POINTS);
	glColor3f(0.0,1.0,0.0);
	
	for(int i = 0 ; i < res ; i++){
	  for(int j = 0 ; j < res ; j++){
	    for(int k = 0 ; k < res ; k++){
	 //     op.rescaleColor(points[i][j][k].getDistance());
	      if(points[i][j][k].getDistance() < 0.f)
	      glVertex3f(points[i][j][k].getX() , points[i][j][k].getY(),points[i][j][k].getZ());
	    }
	  }
	}
	
	glEnd();
      
      glPopMatrix();
}

void renderTriangle(){
  glPushMatrix();
  
  glBegin(GL_TRIANGLES);
    glVertex3f(-0.25,-0.25,-0.25);
    glVertex3f(0.25,-0.25,-0.25);
    glVertex3f(0,0.25,-0.25);
    glVertex3f(-0.25,-0.25,0.25);
    glVertex3f(0.25,-0.25,0.25);
    glVertex3f(0,0.25,0.25);
 //   glVertex3f(0.3,0.5,0.0);
  glEnd();
  
  glPopMatrix();
  
}

void renderLines(){
 glPushMatrix();
  //    glRotatef(angle , 1.0 , 0.0 ,0.0);
 
 glBegin(GL_POINTS);
  
  for(int i = 0; i < points.size(); i++){
    glVertex3f(points[i].getX(), points[i].getY(), points[i].getZ());
  }
  
 glEnd();
 
 glPopMatrix();
}

void draw_boundingbox(BoundingBox b){
  
  vector<Point3D> points = b.getPointVector();
  
  glPushMatrix();
   
  glBegin(GL_POINTS);

  for(int i = 0 ; i< points.size() ; i++){
    glColor3f(1.0,0.0,0.0);
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(points[i].getX() , points[i].getY(), points[i].getZ());
  }
  
  glEnd();
  
  glPopMatrix();
  
}

  
void display(){
  
	float x_pos = R * cos(phi)*cos(theta);
	float y_pos = R * sin(theta);
	float z_pos = R * sin(phi)*cos(theta);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (cos(theta)>=0){

		gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, 1, 0);
	}
	else{
		gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, -1, 0);
	}
	
//	glColor3f(1.0,0.0,0.0);
//	glutWireCube(1);
//	draw_boundingbox(op.rotateBox(bvh.getBoundingBox(),angle));
	
	draw_ljaw();

	//renderLines();
	
//	draw_ujaw();

//	 glPushMatrix();
  //    glRotatef(angle , 1.0 , 0.0 ,0.0);
 
//  	 glBegin(GL_POINTS);
//  	  glVertex3f(0.0+x , 0.15+y , 0.38+z);
//  	 glEnd();
//  	 glPopMatrix();
 
 
	//renderPoints(ljaw.getPointVector() , ljaw.getNormalVector());
	//renderPoints(ujaw.getPointVector() , ujaw.getNormalVector());
	draw_bear();
	
	bvh.update_heirarchy(x,y,z);
	
	op.checkCollision(ufield.getField() , &bvh , res);
 	op.checkLowerCollision(lfield.getField() , &bvh, res, angle);
	
  	/*if(op.isIntersectionMax(bearMax,dfield.getField())){
  	  cout<<"Intersection point: "<<flush;
  	  op.printPoint(bearMax);
  	}*/
 	
 	//if(op.isIntersectBox(lowerBox , bearMin)){
	//  cout<<"Intersection point: "<<flush;
	 // op.printPoint(bearMin);
	//}
	
	renderField(lfield.getField() , res);
	
// 	renderTriangle();
      if(yes){
	renderPoints(ujaw.getPointVector() , ujaw.getNormalVector());
//	renderPoints(ljaw.getPointVector() , ljaw.getNormalVector());
      }
	
	x = 0.f; y = 0.f, z = 0.f;

	glutSwapBuffers();
}



void reshape(int w, int h){
	
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.6);
}

void keyboard (unsigned char key, int xs, int yval)
{

	switch (key) {
		
	  case 'w': theta += 0.1;break;
	  case 's': theta -= 0.1; break;
	  case 'a': phi += 0.1; break;
	  case 'd': phi -= 0.1; break;
	  case 'l': R += 0.1; break;
	  case 'k': R-= 0.1; break;
	  case 'z': y += 0.01f; y12 += 0.01f; break;
	  case 't': y -= 0.01f; y12 -= 0.01f; break;
	  case 'g': x += 0.01f; x12 += 0.01f; break;
	  case 'h': x-= 0.01f; x12 -= 0.01f; break;
	  case 'b': z += 0.01f; z12 += 0.01f; break;
	  case 'n': z-= 0.01f; z12 -= 0.01f; break;
	  case 'm': do_motion();break;
	  case 'y': if(yes) yes = false; else yes = true; break;
	  case 27:
		 exit(0);
		break;
	  default:
		break;
	}
	
	glutPostRedisplay();
}


void init(){
	
	glClearColor (0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters

	glEnable(GL_DEPTH_TEST);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	// XXX docs say all polygons are emitted CCW, but tests show that some aren't.
	if(getenv("MODEL_IS_BROKEN"))  
		glFrontFace(GL_CW);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glutGet(GLUT_ELAPSED_TIME);
 
}





int main(int argc , char** argv){

	glutInit(&argc, argv);
	glutInitWindowSize(900,600);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Distance field method");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	
	init();
	bear.loadData("gummybear_3.stl");
	ljaw.loadData("lower_jaw.stl");
	ujaw.loadData("upper_jaw.stl");
	
	ujaw.setPointVector(ujaw.getScene()->mRootNode);
	//ujaw.setLowerJaw(ljaw.getScene()->mRootNode , ljaw.getScene());
	ljaw.setLowerJaw(ujaw.getScene()->mRootNode , ljaw.getScene());
	bear.setBear(bear.getScene()->mRootNode);
	//ujaw.setTriangle();
	cout<<"Calculating distance field"<<endl;
	res = 20;
//	clock_t tStart = clock();
	ufield.calculateDistanceField(ujaw.getPointVector(),ujaw.getNormalVector(),res);
	lfield.calculateDistanceField(ljaw.getPointVector() , ljaw.getNormalVector(),res);
//	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	cout<<"Done"<<endl;
	
	bvh.calculateBVH(bear.getPointVector(), 10);
	bvh.setTriangles(bear.getTriangles());
	bvh.give_points();
	
// 	clock_t tStart = clock();
// 	
// 	for(int i = 0; i < 3 ; i++)
// 	  op.isInsideBox(bvh.getBoundingBox(),bvh.getPoints()[0]);
// 	
// 	Point3D p(0.f,0.f,-0.5f);
// 	
// 
// 	 op.getIntersection(bear.getPointVector()[0] , bear.getPointVector());
// 	
// 	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
// 	
// 	exit(0);
	
	glutMainLoop();
	
	// cleanup - calling 'aiReleaseImport' is important, as the library 
	// keeps internal resources until the scene is freed again. Not 
	// doing so can cause severe resource leaking.
	aiReleaseImport(bear.getScene());
	aiReleaseImport(ljaw.getScene());
	aiReleaseImport(ujaw.getScene());
	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();


  return 0;
}