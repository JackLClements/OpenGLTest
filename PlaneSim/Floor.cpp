#include "Floor.h"
#include "Utilities\IntersectionTests.h"
Floor::Floor(){
	planePos = glm::vec3(0.0, -20.0, 0.0);//starting vector
	normal = glm::vec3(0, 1, 0); //normal vector
	forward = glm::vec3(0, 0, 1); //forward vector
	side = glm::vec3(-1, 0, 0); //side vector
	obj = new OBJLoader();
	model = new ThreeDModel();
}

Floor::~Floor(){
	delete obj;
	delete model; 
}

void Floor::loadShader(Shader * shader){
	myShader = shader;
}


void Floor::loadModel(){
	//ThreeDModel tempModel; //temporary model object
	model = new ThreeDModel();
	if(obj->loadModel("TestModels/floor.obj", *model))//returns true if the model is loaded, puts the model in the model parameter
		{
		cout << " model loaded " << endl;		
		
		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		model->calcCentrePoint();
		//model->centreOnZero();
		model->scale(20.0);
	
		model->calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.
				

		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model->initDrawElements();
		model->initVBO(myShader);
		model->deleteVertexFaceData();

		}
	else{
		cout << " model failed to load " << endl;
	}
	model->getMinMaxValues(min, max);
	//cout << "MIN " << min[0] << " " << " " << min[1] << " " << min[2] << endl;
	originalPoints[0] = glm::vec3(min[0], min[1], min[2]);
	originalPoints[1] = glm::vec3(max[0], min[1], min[2]);
	originalPoints[2] = glm::vec3(min[0], max[1], min[2]);
	originalPoints[3] = glm::vec3(min[0], min[1], max[2]);
	originalPoints[4] = glm::vec3(max[0], max[1], min[2]);
	originalPoints[5] = glm::vec3(max[0], min[1], max[2]);
	originalPoints[6] = glm::vec3(min[0], max[1], max[2]);
	originalPoints[7] = glm::vec3(max[0], max[1], max[2]);
	//set update points
	
	worldPoints[0] = glm::vec3(min[0], min[1], min[2]);
	worldPoints[1] = glm::vec3(max[0], min[1], min[2]);
	worldPoints[2] = glm::vec3(min[0], max[1], min[2]);
	worldPoints[3] = glm::vec3(min[0], min[1], max[2]);
	worldPoints[4] = glm::vec3(max[0], max[1], min[2]);
	worldPoints[5] = glm::vec3(max[0], min[1], max[2]);
	worldPoints[6] = glm::vec3(min[0], max[1], max[2]);
	worldPoints[7] = glm::vec3(max[0], max[1], max[2]);
	//model = &tempModel; //pass model object memory location to model pointer
}

void Floor::draw(){
	model->drawElementsUsingVBO(myShader);
}

void Floor::drawVerts(){
	model->calcVertNormalsUsingOctree();
	model->calcFaceNormals();
	faceNormals = new double*[3];
	for(int i = 0; i < 3; i++){
		faceNormals[i] = new double[3];
	}
	centerPoint = new double[3];
}

void Floor::drawBoundingBox(){
	model->drawBoundingBox(myShader);
	//model->getMinMaxValues(min, max);
	for(int i = 0; i < 3; i++){
		centerPoint[i] = (worldPoints[7][i] + worldPoints[0][i])/2;
	}
	faceNormals = new double*[3];
	for(int i = 0; i < 3; i++){
		faceNormals[i] = new double[3];
	}
	IntersectionTests::minMaxToFaceNormals(faceNormals, min, max);
	for(int i = 0; i < 3; i++){
		centerPoint[i] = (worldPoints[7][i] + worldPoints[0][i])/2;
	}
}

ThreeDModel* Floor::getModel(){
	return model;
}

double Floor::getWidth(){
	//return (max[0] - min[0])/2;
	return originalPoints[7].x - originalPoints[0].x;
}

double Floor::getHeight(){
	//return (max[1] - min[1])/2;
	return originalPoints[7].y - originalPoints[0].y;
}

double Floor::getDepth(){
	//return (max[2] - min[2])/2;
	return originalPoints[7].z - originalPoints[0].z;
}

double* Floor::getCenter(){
	return centerPoint;
}

double** Floor::getFaceNormals(){
	return faceNormals;
}
void Floor::moveBoundingBox(glm::vec3 vector){
	model->translateBB(Vector3d(vector.x, vector.y, vector.z));
}

void Floor::rotate(glm::mat4 matrix){ //can be used to rotate object, will not be used in this isntance, but is theoretically possible
	for(int i = 0; i < 8; i++){
		glm::vec4 edges = glm::vec4(originalPoints[i].x, originalPoints[i].y, originalPoints[i].z, 1);
		edges = matrix * edges;
		worldPoints[i] = glm::vec3(edges.x, edges.y, edges.z);
	}
}