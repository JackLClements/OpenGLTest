#pragma once
#ifndef _Planar_H
#define _Planar_H

#include "gl\glew.h"
#include "shaders\Shader.h"
const int NumberOfVertexCoords = 12;
const int NumberOfTriangleIndices = 6;
class Planar {
private:
	unsigned int m_vaoID; //vertex array object
	unsigned int m_vboID[2];// two VBOs - used for colours and vertex data
	GLuint ibo; //identifier for the triangle indices
	Shader* myShader; //SHADER

	static int numOfVerts; //number of verts in obj
	static int numOfTris; //number of triangles in obj

	float verts[NumberOfVertexCoords];
	float cols[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];

public:
	Planar();
	~Planar();


	void loadShader(Shader * shader);
	void constructGeometry();
	void draw();
};

#endif