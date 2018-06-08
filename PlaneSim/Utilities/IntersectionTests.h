/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/* Jack L. Clements wrote the AABB intersection code    */
/* What a clever chap									*/
/********************************************************/
#include <iostream>
#include <string>
using namespace std;

class IntersectionTests
{
public:
	static int planeBoxOverlap(float normal[3],float d, float maxbox[3]);
	static int planeBoxOverlap(double normal[3],double d, double maxbox[3]);
	static int triBoxOverlap(float boxhalfsize[3],float v0[3], float v1[3],float v2[3]);
	static int triBoxOverlap(double boxcenter[3],double boxhalfsize[3],double triverts[3][3]);
	static bool OBB_RAY_Intersect(float fromRay[3], float toRay[3]);
	static void minMaxToFaceNormals(double **faceNormals, double min[3], double max[3]);
	static bool AABB_AABB_Intersect(double **normal1, double **normal2, double *p1, double *p2, double width1, double depth1, double height1, double width2, double depth2, double height2); //I wrote this!
	static bool AABB_AABB_Intersect(double **normal1, double **normal2, double *extentA, double *extentB, double* center1, double* center2);
};

//3 axes from object A (face normals)
//3 axes from object B (face normals)
//9 axes from all the pairs of edges of A and edges of B (3x3)
//cross product of 3 edges of easch
//do we need 3 axes?
//Likely
//Normal1/2 contain the x, y and z face normal vectors in relative space - data structure will need a rework
//points contain the x, y and z vectors representing each edge. These edges need to be combined using cross product, if A is object and B is another
//A[points[1]] X B[points[1]]
//A[points[1]] X B[points[2]]
//A[points[1]] X B[points[3]]
//A[points[2]] X B[points[1]]
//A[points[2]] X B[points[2]]
//A[points[2]] X B[points[3]]
//A[points[3]] X B[points[1]]
//A[points[3]] X B[points[2]]
//A[points[3]] X B[points[3]]

//Face normals used to check if any big intersections
//9 axes from cross products used to check edge-on-edge collision
//http://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat
//Calculating face normals http://fullonsoftware.co.uk/snippets/content/Math_-_Calculating_Face_Normals.pdf

/*
STEPS TO DO
CHANGE CO-ORDINATES OF OCTREE TO 3 FACE NORMALS
REMEMBER THAT THEY ARE REGULAR, SO MIN/MAX DOESN'T MATTER
Face normals are the cross the product, of two anti-clockwise vectors
For one face normal...
1. get min/max
2. make 2 vectors in anti-clockwise direction
3. cross product

Pass to aabb test surface normals of objections, center point of object, and half the width, height and depth of objects
*/