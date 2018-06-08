
#ifndef _OCTREE_H
#define _OCTREE_H

#include <vector>
using namespace std;

#include "../3DStruct/threeDModel.h"
#include "../Box/Box.h"

class Shader;

#define MAX_DEPTH  3

class Octree
{
private:
	double minX, minY, minZ, maxX, maxY, maxZ; //bottom left and top right of bounding box, using this, you can calculate all points
	Octree* children[8]; //children of the co-uh, octree
	int Level; //How many you are in
	int* PrimitiveList;
	int PrimitiveListSize;

	int* VertexList;
	int VertexListSize;

	Box* box;

public:
	//get bounding box values drawn at centre, box needs to move with the model, convert to vec4 to scale & rotate
	//does max values, new 4*1 vector for world position, transformation matrix * local-space point, which give world space
	//Order is important, you have bottom left & top right, instead start off doing min/max in local space, then multiply 8 points by world matrix
	//Do checks on key presses
	//Seperating axes, 15 checks need doing, 3 face normals, plus directions and edges. Project all points of 2 boxes onto axes and test for intersection using.
	//check whether parent box of one object intersects with parent box of other object
	Octree();
	~Octree();
	void getMinValues(double* min); 
	void getMaxValues(double* max); 
	int* getPrimitiveList(); 
	int getPrimitiveListSize(); 
	void Delete();
	void set(int L, float x, float y, float z, float X, float Y, float Z, int* PrimList, int PrimListSize, int* vertList, int VertListSize); 
	void start(int L, float x, float y, float z, float X, float Y, float Z, ThreeDModel* model);
	void CreateChildren(vector<Octree*> &stackOctree, ThreeDModel* model);
	void CalculatePrimitiveWithinSubdividedOctreeCells(ThreeDModel* model, int primitiveIndex, int** primitiveSets, int primitiveSetsCount[8]);
	void CalculateVerticesWithinSubdividedOctreeCells(ThreeDModel* model, int vertexIndex, int** vertexSets, int vertexSetsCount[8]);
	void setUpAccessor(int** Acc, float GlobalMinX, float boxWidthX, float GlobalMinY, float boxWidthY, float GlobalMinZ, float boxWidthZ);
	void drawAllBoxes(Shader* myShader);
	void drawBoxesAtLeaves(Shader* myShader);
	void drawBoundingBox(Shader* myShader);
	Octree* getChild(int i);
	int getLevel();
	void processVerticesByLeaf(ThreeDModel* model);
	void translate(double x, double y, double z);

};

#endif _OCTREE_H