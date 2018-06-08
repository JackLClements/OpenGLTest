#pragma once
#ifndef _Skybox_H
#define _Skybox_H
#include "gl\glew.h"
#include "shaders\Shader.h"
#include "3dStruct\threeDModel.h"
#include "Obj\OBJLoader.h"

class Skybox{
private:
	static const int NumberOfVertexCoords = 24;
	static const int NumberOfTriangleIndices = 36;
	ThreeDModel* model;
	OBJLoader* obj;
	Shader* myShader;


	GLuint ibo; //identifier for the triangle indices
	GLuint tex;
	unsigned int m_vaoID; //vertex array object
	unsigned int m_vboID[2];// two VBOs - used for colours and vertex data
	char sFront [40];
	char sBack [40];
	char sRight [40];
	char sLeft [40];
	char sTop [40];
	char sBottom [40];

	static int numOfVerts; //number of verts in obj
	static int numOfTris; //number of triangles in obj

	float verts[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];
public:
	Skybox();
	~Skybox();
	void loadShader(Shader* shader);
	void loadSkybox();
	void drawSkybox();
	void releaseSkybox();	
};
#endif