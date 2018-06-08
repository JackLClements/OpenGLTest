//OPENGL 3.2 DEMO FOR RENDERING OBJECTS LOADED FROM OBJ FILES

//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

#include <windows.h>		// Header File For Windows
#include <ctime>
#include "gl/glew.h"
#include "gl/wglew.h"
#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */
#pragma comment(lib, "glew32.lib")

#include "console.h"
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"


ConsoleWindow console;
Shader* myShader;  ///shader object 
Shader* skyShader; //skybox shader

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "Player.h"
#include "FrameManager.h"
#include "Planar.h"
#include "Skybox.h"
#include "Floor.h"
#include "Tower.h"
float amount = 0;
float temp = 0.00002;
	
Player player;
Planar plane;
Skybox sky;
Floor grass;
Tower tower;
ThreeDModel model, modelbox;
OBJLoader objLoader;
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
glm::quat q; //Quaternions

//Local space for the plane object
glm::vec3 planePos = glm::vec3(0.0, 0.0, 0.0);//starting vector
glm::vec3 normal = glm::vec3(0, 1, 0); //normal vector
glm::vec3 forward = glm::vec3(0, 0, 1); //forward vector
glm::vec3 side = glm::vec3(-1, 0, 0); //side vector
glm::quat transformationQ; //transformation for object
glm::quat forwardQ; //transformation for forward vector
glm::quat sideQ; //transformation for side vector

//Camera1 properties
float cameraHeight = 0.0;
float cameraObjectHeight = 0.1;
float cameraForward = -50.0;


//Material properties
float Material_Ambient[4] = {0.1, 0.1, 0.1, 1.0};
float Material_Diffuse[4] = {0.8, 0.8, 0.5, 1.0};
float Material_Specular[4] = {0.9,0.9,0.8,1.0};
float Material_Shininess = 50;

//Light Properties
float Light_Ambient_And_Diffuse[4] = {0.8, 0.8, 0.8, 1.0};
float Light_Specular[4] = {1.0,1.0,1.0,1.0};
float LightPos[4] = {0.0, 0.0, 1.0, 0.0};

//Matrices
glm::mat4 rotationMatrix;
glm::mat4 eulerMatrix;
glm::mat4 translation;
glm::mat4 translation2;

//Other
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;
bool keys[256];
float spin=180;
int currentCamera;

//movement variables
double moveX;
double moveY;
double moveZ;
glm::vec4 forwardVector;
double speed; //used for movement, could not get working
double spinXinc;
double spinYinc;
double spinZinc;
int engine;
int crash;
//fullscreen toggle
bool fullscreen;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape();				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void update();				//called in winmain to update variables
void updateCol();
void updateSpin(float xinc, float yinc, float zinc); //called in keys to update spin using quarternions
void updateMovement(float xinc, float yinc, float zinc);
glm::mat4 updateCamera(int cam);
/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	FrameManager::getInstance()->calculateFrameRate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);

	//glUseProgram(myShader->handle());  // use the shader

	amount += temp;
	if(amount > 1 || amount < -0.5)
		temp = -temp;
	//amount = 0;
	glUniform1f(glGetUniformLocation(myShader->handle(), "displacement"), amount);

	
	GLuint matLocation2 = glGetUniformLocation(skyShader->handle(), "ProjectionMatrix");
	
	glUniformMatrix4fv(matLocation2, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//when looking in direction of normal vector, glm will try to divide by zero, try to use an offset
	glm::mat4 viewingMatrix;
	//glm::mat4 matrix = glm::inverseTranspose(translation);
	//I have a world forward, but getting the camera to follow it is proving... difficult.
	glm::vec3 normal = glm::cross(glm::vec3(forwardVector[0],forwardVector[1],forwardVector[2]), glm::vec3(eulerMatrix[0][0], eulerMatrix[0][1], eulerMatrix[0][2]));
	
	/*
	if(currentCamera == 0){
		//normal is rotation matrix x y and z coods, as a rotation matrix contains all 3 vectors needed
		//viewingMatrix = glm::lookAt(glm::vec3(translation2[3].x, translation2[3].y, translation2[3].z), glm::vec3(translation2[3].x+0.0000001f, translation2[3].y+0.0000001f, translation2[3].z), glm::vec3(normal[0], normal[1], normal[2])); //to rotate, you want it ot be the plane's up vector, store plane's up vector then multiply by rotation matrix

	}
	else if(currentCamera == 1){
		viewingMatrix = glm::lookAt(glm::vec3(moveX+0.0000001f, moveY, moveZ), glm::vec3(moveX, moveY, moveZ), glm::vec3(0.0, 1.0f, 0.0)); //for cockpit view unrender plane and render a cockpit view, just apply translation and rotation?
	}
	*/
	viewingMatrix = updateCamera(currentCamera);


	//DRAW SKYBOX
	glm::mat4 skyboxM = glm::translate(viewingMatrix, glm::vec3(translation2[3].x+0.0000001f, translation2[3].y, translation2[3].z));
	skyboxM = skyboxM * glm::scale(skyboxM, glm::vec3(200.0f, 200.0f, 200.0f));
	ModelViewMatrix = skyboxM*viewingMatrix;
	//Set up skybox shader
	glUseProgram(skyShader->handle());  // use the shader
	//glUniformMatrix4fv(glGetUniformLocation(skyShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &skyboxM[0][0]); //sends model view matrix to shader
	sky.drawSkybox();

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glUseProgram(myShader->handle()); //this should switch shaders
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);

	//DRAW THE MODEL
	glm::mat4 ModelMatrix = translation*rotationMatrix;
	ModelViewMatrix = viewingMatrix*translation*rotationMatrix; //translations/rotations
	//ModelViewMatrix = glm::rotate(viewingMatrix,spin, glm::vec3(0,1,0));
	
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]); //sends model view matrix to shader

	glm::mat3 normalPlayerMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	normalPlayerMatrix = glm::inverseTranspose(glm::mat3(viewingMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalPlayerMatrix[0][0]);
	player.draw();
	player.drawBoundingBox();
	player.rotatePlayer(ModelMatrix, rotationMatrix);


	ModelViewMatrix = glm::mat4();
	ModelViewMatrix = glm::translate(viewingMatrix, glm::vec3(0.0, -100.0, 0.0));
	glm::mat4 grassMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, -100.0, 0.0));
	glm::mat3 normalGrassMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalGrassMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]); //sends model view matrix to shader
	grass.draw();
	grass.drawBoundingBox();
	grass.rotate(grassMatrix);


	ModelViewMatrix = glm::mat4();
	ModelViewMatrix = glm::translate(viewingMatrix, glm::vec3(50.0, -20.0, -100.0)); ;
	glm::mat4 towerMatrix = glm::translate(glm::mat4(), glm::vec3(50.0, -20.0, -100.0)); ;
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]); //sends model view matrix to shader
	tower.draw();
	tower.drawBoundingBox();
	tower.rotate(towerMatrix);

	updateCol();
	glFlush();
	
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(60.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 20000.0f);
}
void init()
{
	glClearColor(0.9,0.95,0.98,0.0);						//sets the clear colour to blue-white
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);
	crash = 0;
	myShader = new Shader;

	//if(!myShader->load("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
	
    if(!myShader->load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}		
	
	//glUseProgram(myShader->handle());  // use the shader
	glEnable(GL_TEXTURE_2D);
	cout << " loading model " << endl;
	
	skyShader = new Shader;
	if(!skyShader->load("Skybox", "glslfiles/skybox.vert", "glslfiles/skybox.frag"))
	{
		cout << "failed to load shader" << endl;
	}		
	
	player.loadShader(myShader);
	player.loadModel("TestModels/plane.obj");
	player.drawVerts();
	grass.loadShader(myShader);
	grass.loadModel();
	grass.drawVerts();
	tower.loadShader(myShader);
	tower.loadModel();
	tower.drawVerts();
	sky.loadShader(skyShader);
	sky.loadSkybox();

	currentCamera = 1;
	//plane.loadShader(myShader);
	//plane.constructGeometry();
	
}
void processKeys()
{
	spinXinc = 0;
	spinYinc = 0;
	spinZinc = 0;
	//moveX = 0;
	moveY = 0;
	//moveZ = 0;
	if(crash < 3){ //bit of a hack for this line, as objects will always collide on the first frame of the program
	if(keys[VK_UP])
	{
		spinXinc = 0.1 * FrameManager::getInstance()->getDeltaTime();
	}
	if(keys[VK_DOWN])
	{
		spinXinc = -0.1 * FrameManager::getInstance()->getDeltaTime();
	}
	if (keys[VK_LEFT]){
		spinZinc = 0.1 * FrameManager::getInstance()->getDeltaTime(); //is this the right place to apply it? It crashes used on quatermains and matrices
	}
	if (keys[VK_RIGHT]){
		spinZinc = -0.1 * FrameManager::getInstance()->getDeltaTime();
	}
	if(keys[0x31]){
		currentCamera = 1;
	}
	if(keys[0x32]){
		currentCamera = 2;
	}
	if(keys[0x33]){
		currentCamera = 3;
	}
	if (keys[0x57]){
		if(moveZ >= -0.3){
			moveZ -= 0.005;
			//speed -= 0.005;
		}
	}
	if (keys[0x53]){
		if(moveZ <= 0.3){
			moveZ += 0.005;
			//speed += 0.005;
		}
	}
	if (keys[0x41]){
		if(moveX >= -0.3){
			moveX -= 0.01;
		}
	}
	if (keys[0x44]){
		if(moveX <= 0.3){
			moveX += 0.01;
		}	
	}
	if(keys[VK_SPACE]){
		moveY += 0.2;
	}
	if(keys[VK_TAB]){
		moveY -= 0.2;
	}
	if(keys[0x55]){   //increase camera y reference point
		cameraObjectHeight +=.2;
	}
	if(keys[0x4A]){
		cameraObjectHeight -=.2;	//decrease camera y reference point
	}
	if(keys[0x4F]){
		cameraForward +=.2;		//move camera forwardVector
	}
	if(keys[0x4C]){
		cameraForward -=.2;		//move camera backward
	}
	if(keys[0x49]){
		cameraHeight +=.2;             //move camera up
	}
	if(keys[0x4B]){
		cameraHeight -=.2;             //move camera down
	}}
	else{
		moveX = 0;
		moveY = 0;
		moveZ = 0;
	}
	updateSpin(spinXinc, spinYinc, spinZinc);
	updateMovement(moveX, moveY, moveZ);
	player.moveBoundingBox(glm::vec3(moveX, moveY, moveZ));
}

void updateSpin(float xinc, float yinc, float zinc){
	glm::mat4 xSpin;
	glm::mat4 xySpin;

	q = glm::angleAxis(xinc, glm::vec3(rotationMatrix[0][0], rotationMatrix[0][1], rotationMatrix[0][2])); 
	xSpin = glm::mat4_cast(q) * rotationMatrix;

	q = glm::angleAxis(yinc, glm::vec3(rotationMatrix[1][0], rotationMatrix[1][1], rotationMatrix[1][2])); 
	xySpin = glm::mat4_cast(q) * xSpin;

	q = glm::angleAxis(zinc, glm::vec3(rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2])); 
	//player.rotatePlayer(glm::vec3(xinc, yinc, zinc));
	rotationMatrix = glm::mat4_cast(q) * xySpin;
	transformationQ = transformationQ*q;
	transformationQ = glm::normalize(transformationQ);
}

void updateMovement(float xinc, float yinc, float zinc){
	/*
	translation = glm::translate(translation, glm::vec3(xinc*FrameManager::getInstance()->getDeltaTime(), yinc*FrameManager::getInstance()->getDeltaTime(), zinc*FrameManager::getInstance()->getDeltaTime()));
	glm::mat4 mat(1.0f);
	forwardVector = mat*glm::vec4(rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2], 0.0); //direction vector of the object 
	translation2 = glm::translate(translation2, glm::vec3(-forwardVector[0], -forwardVector[1], forwardVector[2]*zinc*FrameManager::getInstance()->getDeltaTime())); //not sure why, but this has to be negative. I blame the rendering pipeline. Handles forward movement, not skybox/camera.
	*/

	
	planePos.x += xinc*FrameManager::getInstance()->getDeltaTime();
	planePos.y += yinc*FrameManager::getInstance()->getDeltaTime();
	planePos.z += zinc*FrameManager::getInstance()->getDeltaTime();
	//translation = glm::translate(translation, glm::vec3(xinc*FrameManager::getInstance()->getDeltaTime(), yinc*FrameManager::getInstance()->getDeltaTime(), zinc*FrameManager::getInstance()->getDeltaTime()));
	glm::mat4 mat(1.0f);
	forwardVector = mat*glm::vec4(rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2], 0.0); //direction vector of the object 
	translation = glm::translate(translation, glm::vec3(-forwardVector[0], -forwardVector[1], forwardVector[2]*zinc*FrameManager::getInstance()->getDeltaTime()));
	transformationQ = transformationQ * glm::quat_cast(translation); //prefix doesn't work with compiler - weird.
	


	/*
	//attempt to do proper forward facing movement
	glm::mat4 mat(1.0f);
	forwardVector = mat*glm::vec4(rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2], 0.0); //direction vector of the object 
	planePos.x += -forwardVector[0]*FrameManager::getInstance()->getDeltaTime();
	planePos.y += -forwardVector[1]*FrameManager::getInstance()->getDeltaTime();
	planePos.z +=  forwardVector[2]*zinc*FrameManager::getInstance()->getDeltaTime();
	translation2 = glm::translate(translation2, glm::vec3(-forwardVector[0], -forwardVector[1], forwardVector[2]*zinc*FrameManager::getInstance()->getDeltaTime()));
	transformationQ = transformationQ * glm::quat_cast(translation2);
	//Translate all camera values by forward vector, make foward-increment times every value
	*/
}

glm::mat4 updateCamera(int cam){
	switch(cam)
	{
	case 1:
		{
		glLoadIdentity ();     //loads identiy to reset an transformations on the martrix stack	
		glm::vec3 move = glm::vec3(0,0,1);  //gets the base direction of the plane
		move = transformationQ * move;		//applies tranformation to diretion vector
		glm::normalize(move);				//normalize the vector
	
		glm::vec3 dir;				//Get the reverise of the direction vector
		dir.x = -1 * move.x;
		dir.y = -1 * move.y;
		dir.z = -1 * move.z;

		dir.x *= cameraForward;					//scale the vector to set forwardVector, backward camera position
		dir.y *= cameraForward;
		dir.z *= cameraForward;

		normal = glm::vec3(0,1,0);	//get the normal vector of the airplane
		normal = transformationQ * normal;
		normal = glm::normalize(normal);
		//cout << normal.x << " " << normal.y << " " << normal.z << endl;
		double aty = normal.y * cameraHeight;	//set camera hiehgt
		double looky = normal.y * cameraObjectHeight;	//set height of camera reference point
		//gluLookAt(planePos[0] + dir.x , planePos[1] + dir.y + aty,  planePos[2] + dir.z , 
		//planePos[0]+ move.x, planePos[1] + move.y + looky, planePos[2] + move.z ,
	    //normal.x,normal.y,normal.z);
		//need case to stop co-linear gimbal locking
		return glm::lookAt(glm::vec3(planePos[0] + dir.x , planePos[1] + dir.y + aty,  planePos[2] + dir.z), glm::vec3(planePos[0]+ move.x + 0.000001, planePos[1] + move.y + looky + 0.000001, planePos[2] + move.z + 0.000001), glm::vec3(normal.x,normal.y,normal.z));
		break;
		}
	case 2:
		{
		glLoadIdentity(); 
		return glm::lookAt(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.000000001f, 0.000000001f, 0.000000001f), glm::vec3(0,1,0));
		break;
		}
	case 3:
		{
			glLoadIdentity(); 
			return glm::lookAt(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.000000001f, 0.000000001f, 0.000000001f), glm::vec3(0,0,1));
			break;
		}
	}
}


void updateCol(){
	if(player.checkCollision(&grass) && (player.getYFaceNormal()[1] >= 0.8f)){
		cout << "Landing" << endl;
	}
	else if (player.checkCollision(&grass)){
		crash++;
	}
	if(player.checkCollision(&tower)){
		crash++;
	}
	//cout << "FACE NORMAL " << player.getYFaceNormal()[0] << " " << player.getYFaceNormal()[1] << " " << player.getYFaceNormal()[2] << endl;
}

void update()
{
	/*
	glm::vec3 move = glm::vec3(0,0,1);  //gets the base direction of the plane
	move = transformationQ * move;		//applies tranformation to diretion vector
	glm::normalize(move);				//normalize the vector

	planePos[0] = planePos[0] + move.x * speed; 
	planePos[1] = planePos[0] + move.y * speed; 
	planePos[2] = planePos[0] + move.z * speed; 
	*/
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	console.Open();


	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard
			
			display();					// Draw The Scene
			update();					// update variables
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if(fullscreen){
		ChangeDisplaySettings(NULL,0);                  //Switch Back To Desktop
		ShowCursor(TRUE);								//Show cursor 
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << GLVersionString << endl;

	//OpenGL 3.2 way of checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}



