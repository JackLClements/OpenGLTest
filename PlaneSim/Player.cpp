#include "Player.h"
#include "Utilities\IntersectionTests.h"
#include "Box\Box.h"
Player::Player(){
	obj = new OBJLoader();
	model = new ThreeDModel();
}

Player::~Player(){
	delete obj;
	delete model; 
}

void Player::loadShader(Shader * shader){
	myShader = shader;
}

void Player::loadModel(char * fileName){
	model = new ThreeDModel();
	if(obj->loadModel(fileName, *model))//returns true if the model is loaded, puts the model in the model parameter
		{
		cout << " model loaded " << endl;		
		
		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		model->calcCentrePoint();
		model->centreOnZero();
		model->scale(50.0);
	
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
	
}

void Player::draw(){
	model->drawElementsUsingVBO(myShader);
}

void Player::drawVerts(){
	model->calcVertNormalsUsingOctree();
	model->calcFaceNormals();
	faceNormals = new double*[3];
	for(int i = 0; i < 3; i++){
		faceNormals[i] = new double[3];
	}
	centerPoint = new double[3];
}

void Player::moveBoundingBox(glm::vec3 vector){
	model->translateBB(Vector3d(vector.x, vector.y, vector.z));
}

void Player::drawBoundingBox(){
	//model->drawBoundingBox(myShader);
	IntersectionTests::minMaxToFaceNormals(faceNormals, min, max);
	for(int i = 0; i < 3; i++){
		centerPoint[i] = (worldPoints[7][i] + worldPoints[0][i])/2;
	}
	//cout << "Center Point - " << centerPoint[0] << " " << centerPoint[1] << " " << centerPoint[2] << endl;
	//model->drawOctreeLeaves(myShader);
}

void Player::rotatePlayer(glm::mat4 matrix, glm::mat4 rotation){
	//take transformation matrix = translation*rotation
	//glm::mat3 transforms = glm::mat3(matrix);

	for(int i = 0; i < 3; i++){
		glm::vec4 normals = glm::vec4(faceNormals[i][0], faceNormals[i][1], faceNormals[i][2], 1);
		normals = rotation*normals;
		faceNormals[i][0] = normals.x;
		faceNormals[i][1] = normals.y;
		faceNormals[i][2] = normals.z;
	}
	for(int i = 0; i < 8; i++){
		glm::vec4 edges = glm::vec4(originalPoints[i].x, originalPoints[i].y, originalPoints[i].z, 1);
		edges = matrix * edges;
		worldPoints[i] = glm::vec3(edges.x, edges.y, edges.z);
	}
	//cout << "MIN " << worldPoints[0].x << " " << worldPoints[0].y << " " << worldPoints[0].z << endl;
}

//depricated
void Player::checkCollision(){
	double** faceNormals2 = new double*[3];
	for(int i = 0; i < 3; i++){
		faceNormals2[i] = new double[3];
	}
	double* centerPoint2 = new double[3];
	bool thing = IntersectionTests::AABB_AABB_Intersect(faceNormals, faceNormals, centerPoint, centerPoint, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	if(thing){
		//cout << "COLLIDING" << endl;
	}
	else{
		//cout << "NOT COLLIDING" << endl;
	}
}

//used
bool Player::checkCollision(Tower* tower){
	//CO-ORDINATE CALCULATION NEEDS TO BE GLOBAL FOR CENTRE POINT, NOT RELATIVE, THIS IS WHERE YOU ARE WRONG
	double testPoints[3];
	testPoints[0] = (max[0] - min[0])/2;
	testPoints[1] = (max[1] - min[1])/2;
	testPoints[2] = (max[2] - min[2])/2;
	for(int i = 0; i < 3; i++){
		testPoints[i] = testPoints[i]/sqrt(((max[0] - min[0])/2)*((max[0] - min[0])/2) + (((max[1] - min[1])/2)*((max[1] - min[1])/2)) + (((max[2] - min[2])/2)*((max[2] - min[2])/2)));
	}
	//double extentA[3];
	//extentA[0] = (max[0] - min[0])/2;
	//extentA[1] = (max[1] - min[1])/2;
	//extentA[2] = (max[2] - min[2])/2;
	//cout << "Plane values " << extentA[0] << " " << extentA[1] << " " << extentA[2] << endl;
	//double extentB[3];
	//cout << "Tower values " << tower->getWidth() << " " << tower->getHeight() << " " << tower->getDepth() << endl;
	//extentB[0] = tower->getWidth();
	//extentB[1] = tower->getHeight();
	//extentB[2] = tower->getDepth();
	//cout << "Center Plane " << centerPoint[0] << " " << centerPoint[1] << " " << centerPoint[2] << endl;
	//cout << "Center Tower " << tower->getCenter()[0] << " " << tower->getCenter()[1] << " " << tower->getCenter()[2] << endl;
	//bool thing = IntersectionTests::AABB_AABB_Intersect(faceNormals, tower->getFaceNormals(), extentA, extentB, centerPoint, tower->getCenter());
	double width = originalPoints[7].x - originalPoints[0].x;
	//cout << "Width - " << width << endl;
	double height = originalPoints[7].y - originalPoints[0].y;
	//cout << "Height - " << height << endl;
	double depth = originalPoints[7].z - originalPoints[0].z;
	//cout << "Depth - " << depth << endl;
	bool thing = IntersectionTests::AABB_AABB_Intersect(faceNormals, tower->getFaceNormals(), centerPoint, tower->getCenter(), width/2, depth/2, height/2, tower->getWidth()/2, tower->getDepth()/2, tower->getHeight()/2);
	return thing;

}

bool Player::checkCollision(Floor* floor){
	//CO-ORDINATE CALCULATION NEEDS TO BE GLOBAL FOR CENTRE POINT, NOT RELATIVE, THIS IS WHERE YOU ARE WRONG
	double testPoints[3];
	testPoints[0] = (max[0] - min[0])/2;
	testPoints[1] = (max[1] - min[1])/2;
	testPoints[2] = (max[2] - min[2])/2;
	for(int i = 0; i < 3; i++){
		testPoints[i] = testPoints[i]/sqrt(((max[0] - min[0])/2)*((max[0] - min[0])/2) + (((max[1] - min[1])/2)*((max[1] - min[1])/2)) + (((max[2] - min[2])/2)*((max[2] - min[2])/2)));
	}
	double width = originalPoints[7].x - originalPoints[0].x;
	//cout << "Width - " << width << endl;
	double height = originalPoints[7].y - originalPoints[0].y;
	//cout << "Height - " << height << endl;
	double depth = originalPoints[7].z - originalPoints[0].z;
	//cout << "Depth - " << depth << endl;
	bool thing = IntersectionTests::AABB_AABB_Intersect(faceNormals, floor->getFaceNormals(), centerPoint, floor->getCenter(), width/2, depth/2, height/2, floor->getWidth()/2, floor->getDepth()/2, floor->getHeight()/2);
	return thing;
}

glm::vec3 Player::getCPoint(){
	return glm::vec3(centerPoint[0], centerPoint[1], centerPoint[2]);
}

glm::vec3 Player::getYFaceNormal(){
	return glm::vec3(faceNormals[1][0], faceNormals[1][1], faceNormals[1][2]);
}
