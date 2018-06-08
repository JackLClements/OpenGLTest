#pragma once
#ifndef _TOWER_H
#define _TOWER_H
#include "glm\glm.hpp"
#include "3dStruct\threeDModel.h"
#include "Obj\OBJLoader.h"

class Tower{
private:
	ThreeDModel* model;
	OBJLoader* obj;
	Shader* myShader;
	//Local space attributes
	glm::vec3 planePos;
	glm::vec3 normal; //normal vector
	glm::vec3 forward; //forward vector
	glm::vec3 side; //side vector

	double min[3]; //min point 4 bounding box
	double max[3]; //max point 4 bounding box
	glm::vec3 worldPoints[8];
	glm::vec3 originalPoints[8];
	double *centerPoint;
	double** faceNormals;

	double testPoints[3]; //used for testing collisions
public:
	Tower();
	~Tower();
	void rotate(glm::mat4 matrix);
	void loadShader(Shader * shader);
	void loadModel();
	void draw();
	void getInfoForCollisions();
	void drawBoundingBox();
	double getWidth();
	double getHeight();
	double getDepth();
	double* getCenter();
	double** getFaceNormals();
	ThreeDModel* getModel();
	void moveBoundingBox(glm::vec3 vector);
	void drawVerts();
};
#endif