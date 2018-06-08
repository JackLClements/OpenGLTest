#pragma once
#ifndef _FLOOR_H
#define _FLOOR_H
#include "glm\glm.hpp"
#include "3dStruct\threeDModel.h"
#include "Obj\OBJLoader.h"


class Floor{
private:
	ThreeDModel* model;
	OBJLoader* obj;
	Shader* myShader;

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
	Floor();
	~Floor();
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
	void rotate(glm::mat4 matrix);
};
#endif