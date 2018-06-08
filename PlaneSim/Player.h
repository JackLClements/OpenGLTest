#pragma once
#ifndef _Player_H
#define _Player_H

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "3dStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "Tower.h"
#include "Floor.h"
class Player {
private:
	ThreeDModel* model;
	OBJLoader* obj;
	Shader* myShader;
	double min[3]; //min point 4 bounding box
	double max[3]; //max point 4 bounding box
	double** faceNormals;
	glm::vec3 worldPoints[8];
	glm::vec3 originalPoints[8];
	//these three values need to be controlled via frame intervals
	float roll; //left/right spin
	float pitch; //forward/backward tilt
	float yaw; //left/right turn
	float speed; //acceleration speed
	double *centerPoint;

	glm::vec3 rotation;
public:
	Player();
	~Player();

	void loadShader(Shader * shader);
	void loadModel(char * fileName);
	void draw();
	void drawVerts();
	void drawBoundingBox();
	void moveBoundingBox(glm::vec3 vector);
	void rotatePlayer(glm::mat4 matrix, glm::mat4 rotation);
	bool checkCollision(Tower* tower);
	bool checkCollision(Floor* floor);
	void checkCollision();
	glm::vec3 getCPoint();
	glm::vec3 getYFaceNormal();
};
#endif
