#include "Skybox.h"
#include "Images\nvImage.h"
int Skybox::numOfTris = 12;
int Skybox::numOfVerts = 8;

Skybox::Skybox(){
	obj = new OBJLoader();
	model = new ThreeDModel();
}

Skybox::~Skybox(){
	delete obj;
	delete model;
}

void Skybox::loadShader(Shader* shader){
	myShader = shader;
}

void Skybox::loadSkybox(){
	/*
	verts[0] = -10.0f;   verts[ 1] = -10.0f;  verts[ 2] = -10.0f;
	verts[3] = -10.0f;   verts[ 4] =  10.0f;  verts[ 5] = -10.0f;
	verts[6] =  10.0f;   verts[ 7] =  10.0f;  verts[ 8] = -10.0f;
	verts[9] =  10.0f;   verts[10] = -10.0f;  verts[11] = -10.0f;

	verts[12] = -10.0f;   verts[13] = -10.0f;  verts[14] = 10.0f;
	verts[15] = -10.0f;   verts[16] =  10.0f;  verts[17] = 10.0f;
	verts[18] =  10.0f;   verts[19] =  10.0f;  verts[20] = 10.0f;
	verts[21] =  10.0f;   verts[22] = -10.0f;  verts[23] = 10.0f;
	
	tris[0]=0; tris[1]=1; tris[2]=2;
	tris[3]=0; tris[4]=2; tris[5]=3;
	tris[6]=4; tris[7]=6; tris[8]=5;
	tris[9]=4; tris[10]=7; tris[11]=6;
	tris[12]=1; tris[13]=5; tris[14]=6;
	tris[15]=1; tris[16]=6; tris[17]=2;
	tris[18]=0; tris[19]=7; tris[20]=4;
	tris[21]=0; tris[22]=3; tris[23]=7;
	tris[24]=0; tris[25]=5; tris[26]=1;
	tris[27]=0; tris[28]=4; tris[29]=5;
	tris[30]=3; tris[31]=2; tris[32]=7;
	tris[33]=2; tris[34]=6; tris[35]=7;
		
	// VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);
	
	glGenBuffers(2, m_vboID);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, numOfVerts*3*sizeof(GLfloat), verts, GL_STATIC_DRAW);
	GLint vertexLocation= glGetAttribLocation(myShader->handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(vertexLocation);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tris, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	nv::Image img;

	if(img.loadImageFromFile("skybox.png")){
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	*/

//	model = new ThreeDModel();
	if(obj->loadModel("TestModels/skybox3.obj", *model))//returns true if the model is loaded, puts the model in the model parameter
		{
		cout << " Skybox loaded " << endl;		
		
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
		cout << " Skybox failed to load " << endl;
		}


}

void Skybox::drawSkybox(){
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(0); 
	model->drawElementsUsingVBO(myShader);
	//glDepthMask(1);
	//glEnable(GL_DEPTH_TEST);
}

void Skybox::releaseSkybox(){

}