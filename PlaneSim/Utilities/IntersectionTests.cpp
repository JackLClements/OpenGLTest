/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(double boxcenter[3],      */
/*          double boxhalfsize[3],double triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
#include <math.h>
#include "IntersectionTests.h"

#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;
//I also wrote these, go figure
#define MAGNITUDE(v1) \
	sqrt((v1[0]*v1[0])+(v1[1]*v1[1])+(v1[2]*v1[2]));

#define NORMAL(dest, v1) \
	dest[0] = v1[0]/sqrt((v1[0]*v1[0]) + (v1[1]*v1[1]) + (v1[2]*v1[2])); \
	dest[1] = v1[1]/sqrt((v1[0]*v1[0]) + (v1[1]*v1[1]) + (v1[2]*v1[2])); \
	dest[2] = v1[2]/sqrt((v1[0]*v1[0]) + (v1[1]*v1[1]) + (v1[2]*v1[2])); 

#define ABS(v1) (sqrt(v1*v1))
		
int IntersectionTests::planeBoxOverlap(double normal[3],double d, double maxbox[3])
{
  int q;
  double vmin[3],vmax[3];
  for(q=X;q<=Z;q++)
  {
    if(normal[q]>0.0f)
    {
      vmin[q]=-maxbox[q];
      vmax[q]=maxbox[q];
    }
    else
    {
      vmin[q]=maxbox[q];
      vmax[q]=-maxbox[q];
    }
  }
  if(DOT(normal,vmin)+d>0.0f) return 0;
  if(DOT(normal,vmax)+d>=0.0f) return 1;
  
  return 0;
}

int IntersectionTests::planeBoxOverlap(float normal[3],float d, float maxbox[3])
{
  int q;
  float vmin[3],vmax[3];
  for(q=X;q<=Z;q++)
  {
    if(normal[q]>0.0f)
    {
      vmin[q]=-maxbox[q];
      vmax[q]=maxbox[q];
    }
    else
    {
      vmin[q]=maxbox[q];
      vmax[q]=-maxbox[q];
    }
  }
  if(DOT(normal,vmin)+d>0.0f) return 0;
  if(DOT(normal,vmax)+d>=0.0f) return 1;
  
  return 0;
}
/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

int IntersectionTests::triBoxOverlap(double boxcenter[3],double boxhalfsize[3],double triverts[3][3])
{

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
   double v0[3],v1[3],v2[3];
  // double axis[3];
   double min,max,d,p0,p1,p2,rad,fex,fey,fez;  
   double normal[3],e0[3],e1[3],e2[3];
   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   SUB(v0,triverts[0],boxcenter);
   SUB(v1,triverts[1],boxcenter);
   SUB(v2,triverts[2],boxcenter);

   /* compute triangle edges */
   SUB(e0,v1,v0);      /* tri edge 0 */
   SUB(e1,v2,v1);      /* tri edge 1 */
   SUB(e2,v0,v2);      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = ABS(e0[X]);
   fey = ABS(e0[Y]);
   fez = ABS(e0[Z]);
   AXISTEST_X01(e0[Z], e0[Y], fez, fey);
   AXISTEST_Y02(e0[Z], e0[X], fez, fex);
   AXISTEST_Z12(e0[Y], e0[X], fey, fex);

   fex = ABS(e1[X]);
   fey = ABS(e1[Y]);
   fez = ABS(e1[Z]);
   AXISTEST_X01(e1[Z], e1[Y], fez, fey);
   AXISTEST_Y02(e1[Z], e1[X], fez, fex);
   AXISTEST_Z0(e1[Y], e1[X], fey, fex);

   fex = ABS(e2[X]);
   fey = ABS(e2[Y]);
   fez = ABS(e2[Z]);
   AXISTEST_X2(e2[Z], e2[Y], fez, fey);
   AXISTEST_Y1(e2[Z], e2[X], fez, fex);
   AXISTEST_Z12(e2[Y], e2[X], fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in X-direction */
   FINDMINMAX(v0[X],v1[X],v2[X],min,max);
   if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;

   /* test in Y-direction */
   FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);
   if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;

   /* test in Z-direction */
   FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);
   if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   CROSS(normal,e0,e1);
   d=-DOT(normal,v0);  /* plane eq: normal.x+d=0 */
   if(!planeBoxOverlap(normal,d,boxhalfsize)) return 0;

   return 1;   /* box and triangle overlaps */
}

int IntersectionTests::triBoxOverlap(float boxhalfsize[3],float v0[3], float v1[3],float v2[3])
{

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
   //double v0[3],v1[3],v2[3];
  // double axis[3];
   float min,max,d,p0,p1,p2,rad,fex,fey,fez;  
   float normal[3],e0[3],e1[3],e2[3];

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
  // SUB(v0,trivertsA,boxcenter);
  // SUB(v1,trivertsB,boxcenter);
  // SUB(v2,trivertsC,boxcenter);

   /* compute triangle edges */
   //e0[0] = v1[0] - v0[0]; e0[1] = v1[1] - v0[1]; e0[2] = v1[2] - v0[2];
   //e1[0] = v2[0] - v1[0]; e1[1] = v2[1] - v1[1]; e1[2] = v2[2] - v1[2];
   //e2[0] = v0[0] - v2[0]; e2[1] = v0[1] - v2[1]; e2[2] = v0[2] - v2[2];
   SUB(e0,v1,v0);      /* tri edge 0 */
   SUB(e1,v2,v1);      /* tri edge 1 */
   SUB(e2,v0,v2);      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = e0[X];//fABS(e0[X]);
   fey = e0[Y];//fABS(e0[Y]);
   fez = e0[Z];//fABS(e0[Z]);
   if(e0[X] < 0)
	   fex = -e0[X];
   if(e0[Y] < 0)
	   fey = -e0[Y];
   if(e0[Z] < 0)
	   fez = -e0[Z];
   AXISTEST_X01(e0[Z], e0[Y], fez, fey);
   AXISTEST_Y02(e0[Z], e0[X], fez, fex);
   AXISTEST_Z12(e0[Y], e0[X], fey, fex);

   fex = e1[X];//fABS(e1[X]);
   fey = e1[Y];//fABS(e1[Y]);
   fez = e1[Z];//fABS(e1[Z]);
   if(e1[X] < 0)
	   fex = -e1[X];
   if(e1[Y] < 0)
	   fey = -e1[Y];
   if(e1[Z] < 0)
	   fez = -e1[Z];
   AXISTEST_X01(e1[Z], e1[Y], fez, fey);
   AXISTEST_Y02(e1[Z], e1[X], fez, fex);
   AXISTEST_Z0(e1[Y], e1[X], fey, fex);

   fex = e2[X];//fABS(e2[X]);
   fey = e2[Y];//fABS(e2[Y]);
   fez = e2[Z];//fABS(e2[Z]);
   if(e2[X] < 0)
	   fex = -e2[X];
   if(e2[Y] < 0)
	   fey = -e2[Y];
   if(e2[Z] < 0)
	   fez = -e2[Z];
   AXISTEST_X2(e2[Z], e2[Y], fez, fey);
   AXISTEST_Y1(e2[Z], e2[X], fez, fex);
   AXISTEST_Z12(e2[Y], e2[X], fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in X-direction */
   FINDMINMAX(v0[X],v1[X],v2[X],min,max);
   if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;

   /* test in Y-direction */
   FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);
   if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;

   /* test in Z-direction */
   FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);
   if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   CROSS(normal,e0,e1);
   d=-DOT(normal,v0);  /* plane eq: normal.x+d=0 */

   if(!planeBoxOverlap(normal,d,boxhalfsize)) return 0;

   return 1;   /* box and triangle overlaps */
}



void IntersectionTests::minMaxToFaceNormals(double **faceNormals, double min[3], double max[3]){
	/*
	double** faceNormals = new double*[3];
	for(int i = 0; i < 3; i++){
		faceNormals[i] = new double[3];
	}
	*/
	double A[3]; //Vertex 1
	double B[3]; //Vertex 2
	double C[3]; //Vertex 3

	
	//Extrapolate 3 vectors from min/max that represent a face
	//Vector 1 = Vertex B - Vertex A
	//Vector 2 = Vertex C - Vertex A
	//FOR EACH X, Y & Z
	//Cross product
	
	//Face 1
	//A = min.x, min.y, min.z
	//B = min.x, min.y, max.z
	//C = min.x, max.y, max.z
	A[0] = min[0];
	A[1] = min[1];
	A[2] = min[2];

	B[0] = min[0];
	B[1] = min[1];
	B[2] = max[2];

	C[0] = min[0];
	C[1] = max[1];
	C[2] = max[2];

	double vec1[3];
	double vec2[3];
	SUB(vec1, B, A);
	SUB(vec2, C, A);
	double normal1[3];
	CROSS(normal1, vec1, vec2);
	//cout << "X " << normal1[0] << " Y " << normal1[1] << " Z " << normal1[2] << endl;
	for(int i = 0; i < 3; i++){
		faceNormals[0][i] = normal1[i];
		vec1[i] = 0; //reset memory values
		vec2[i] = 0; //reset memory values
	}
	//Face 2
	A[0] = min[0];
	A[1] = max[1];
	A[2] = max[2];

	B[0] = max[0];
	B[1] = max[1];
	B[2] = max[2];

	C[0] = max[0];
	C[1] = max[1];
	C[2] = min[2];
	
	SUB(vec1, B, A);
	SUB(vec2, C, A);
	double normal2[3];
	CROSS(normal2, vec1, vec2);
	for(int i = 0; i < 3; i++){
		faceNormals[1][i] = normal2[i];
		vec1[i] = 0; //reset memory values
		vec2[i] = 0; //reset memory values
	}
	//Face 3
	A[0] = min[0];
	A[1] = min[1];
	A[2] = max[2];

	B[0] = max[0];
	B[1] = min[1];
	B[2] = max[2];

	C[0] = max[0];
	C[1] = max[1];
	C[2] = max[2];

	
	SUB(vec1, B, A);
	SUB(vec2, C, A);
	double normal3[3];
	CROSS(normal3, vec1, vec2);
	for(int i = 0; i < 3; i++){
		faceNormals[2][i] = normal3[i];
	}
	//do these need normalising?
	//Find out
	//I think so?
	
	double temp1[3];
	double temp2[3];
	double temp3[3];
	NORMAL(temp1, normal1);
	NORMAL(temp2, normal2);
	NORMAL(temp3, normal3);
	//cout << "FINAL -- " << "X " << temp1[0] << " Y " << temp1[1] << " Z " << temp1[2] << endl;
	
	for(int i = 0; i < 3; i++){
		faceNormals[0][i] = temp1[i];
		faceNormals[1][i] = temp2[i];
		faceNormals[2][i] = temp3[i];
	}
	//cout << "FINAL 2 -- " << "X " << faceNormals[0][0] << " Y " << faceNormals[0][1] << " Z " << faceNormals[0][2] << endl;
	//dest = &(*faceNormals); //NOTE TO FUTURE JACK - THIS NEEDS TO BE A ** NOT A * AS AN ARRAY OF ARRAYS IS A POINTER TO A POINTER, QUID PRO QUO
}


/**
  Optimised calculations kindly provided by http://www.jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf
  normal1 is relative surface normals for obj1
  normal2 is relative surface normals for obj2
  p1 is the center point of obj1 (global space)
  p2 is center point of obj2 (global space)
  width1 is half width of obj1
  width2 is half width of obj2
  depth1 is half depth of obj1
  depth2 is half depth of obj2
  height1 is half height of obj1
  height2 is half height of obj2
*/
bool IntersectionTests::AABB_AABB_Intersect(double **normal1, double **normal2, double *p1, double *p2, double width1, double depth1, double height1, double width2, double depth2, double height2){
	/* Two types of possible seperating axes, the first being face normals of either object, the second being edges */
	double dist[3];
	for(int i = 0; i < 3; i++){
		dist[i] = p2[i] - p1[i];	//minkowski sum
	}

	//normal1[0] = the X-vector
	//normal1[1] = the Y-vector
	//normal1[2] = the Z-vector
	//NOTE DEPTH, HEIGHT ETC. MUST BE HALVED

	/* --- Check face normals of each bounding box --- */
	//L is our axis, width is X, height is Y and depth is Z
	//Dot product of local x-axis and global co-ordinate difference T
	//Local Axes for AABB 1
	//X
	//Project Points onto axis
	if(ABS(DOT(dist, normal1[0])) > width1 + ABS((width2 * DOT(normal1[0], normal2[0]))) + ABS((height2 * DOT(normal1[0], normal2[1]))) + ABS((depth2 * DOT(normal1[0], normal2[2])))){
		//cout << "X NORMAL TRIGGERED" << endl;
		return false;
	}
	//Y
	//Project Points onto axis
	//if(t . x2y > heightA + (widthB * Ay DOT Bx) + (heightB * Ay DOT By) + (depthB * Ay DOT Bz) 
	if(ABS(DOT(dist, normal1[1])) > height1 + ABS((width2 * DOT(normal1[1], normal2[0]))) + ABS((height2 * DOT(normal1[1], normal2[1]))) + ABS((depth2 * DOT(normal1[1], normal2[2])))){
		//cout << "Y NORMAL TRIGGERED" << endl;
		return false;
	}
	//Z
	//Project Points onto axis
	//if(t . x2z > depthA + (widthB * Az DOT Bx) + (heightB * Az DOT By) + (depthB * Az DOT Bz) 
	//cout << "ABS of normal2 plus z axis " << ABS(DOT(dist, normal1[2])) << endl;
	//cout << "Extent A + B " << depth1 + ABS((width2 * DOT(normal1[2], normal2[0]))) + ABS((height2 * DOT(normal1[2], normal2[1]))) + ABS((depth2 * DOT(normal1[2], normal2[2]))) << endl;
	if(ABS(DOT(dist, normal1[2])) > depth1 + ABS((width2 * DOT(normal1[2], normal2[0]))) + ABS((height2 * DOT(normal1[2], normal2[1]))) + ABS((depth2 * DOT(normal1[2], normal2[2])))){
		//cout << "Z NORMAL TRIGGERED" << endl;
		return false;
	}
	//Local Axes for AABB 2
	//X
	//Project Points onto axis
	//if(t . y2x > widthB + (widthA * Ax DOT Bx) + (heightA * Ay DOT Bx) + (depthA * Az DOT Bx) 
	if(ABS(DOT(dist, normal2[0])) > width2 + ABS((width1 * DOT(normal1[0], normal2[0]))) + ABS((height1*DOT(normal1[1], normal2[0]))) + ABS((depth1*DOT(normal1[2], normal2[0])))){
		//cout << "X NORMAL 2 TRIGGERED" << endl;
		return false;
	}
	//Y
	//Project Points onto axis
	//if(t . y2y > heightB + (widthA * Ax DOT By) + (heightA * Ay DOT By) + (depthA * Az DOT By) 
	if(ABS(DOT(dist, normal2[1])) > height2 + ABS((width1 * DOT(normal1[0], normal2[1]))) + ABS((height1 * DOT(normal1[1], normal2[1]))) + ABS((depth1 * DOT(normal1[2], normal2[1])))){
		//cout << "Y NORMAL 2 TRIGGERED" << endl;
		return false;
	}
	//Z
	//Project Points onto axis
	//if(t . y2z > depthB + (widthA * Ax DOT Bz) + (heightA * Ay DOT Bz) + (depthA * Az DOT Bz) 
	//cout << "DOT OF DIST/NORMAL2 Z " << DOT(dist, normal2[2]) << endl;
	//cout << "PLANE CHECK " << depth2 + ABS((width1 * DOT(normal1[0], normal2[2]))) + ABS((height1 * DOT(normal1[1], normal2[2]))) + ABS((depth1 * DOT(normal1[2], normal2[2]))) << endl;
	if(ABS(DOT(dist, normal2[2])) > depth2 + (width1 * ABS(DOT(normal1[0], normal2[2]))) + ABS((height1 * DOT(normal1[1], normal2[2]))) + ABS((depth1 * DOT(normal1[2], normal2[2])))){
		//cout << "Z NORMAL 2 TRIGGERED" << endl;
		return false;
	}

	/* --- Cross Product of 3*3 points on each bounding box --- */
	//if((t . (x2x X y2x)) > (heightA * Az DOT Bx) + (depthA *  Ay DOT Bx) + (heightB * Ax DOT Bz) + (depthB * Ax DOT By))
	double destinationT[3];
	double destination[3];
	CROSS(destinationT, normal1[0], normal2[0]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((height1 * DOT(normal1[2], normal2[0]))) + ABS((depth1 * DOT(normal1[1], normal2[0]))) + ABS((height2 * DOT(normal1[0], normal2[2]))) + ABS((depth2 * DOT(normal1[0], normal2[1])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2x X y2y)) > (heightA * Az DOT By) + (depthA *  Ay DOT By) + (widthB * Ax DOT Bz) + (depthB * Ax DOT Bx))
	CROSS(destinationT, normal1[0], normal2[1]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((height1 * DOT(normal1[2], normal2[1]))) + ABS((depth1 * DOT(normal1[1], normal2[1]))) + ABS((width2 * DOT(normal1[0], normal2[2]))) + ABS((depth2 * DOT(normal1[0], normal2[0])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2x X y2z)) > (heightA * Az DOT Bz) + (depthA *  Ay DOT Bz) + (widthB * Ax DOT By) + (heightB * Ax DOT Bx))
	CROSS(destinationT, normal1[0], normal2[2]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((height1 * DOT(normal1[2], normal2[2]))) + ABS((depth1 * DOT(normal1[1], normal2[2]))) + ABS((width2 * DOT(normal1[0], normal2[1]))) + ABS((height2 * DOT(normal1[0], normal2[0])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2y X y2x)) > (widthA * Az DOT Bx) + (depthA *  Ax DOT Bx) + (heightB * Ay DOT Bz) + (depthB * Ay DOT By))
	CROSS(destinationT, normal1[1], normal2[0]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((width1 * DOT(normal1[2], normal2[0]))) + ABS((depth1 * DOT(normal1[0], normal2[0]))) + ABS((height2 * DOT(normal1[1], normal2[2]))) + ABS((depth2 * DOT(normal1[1], normal2[1])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2y X y2y)) > (widthA * Az DOT By) + (depthA *  Ax DOT By) + (widthB * Ay DOT Bz) + (depthB * Ay DOT Bx))
	CROSS(destinationT, normal1[1], normal2[1]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((width1 * DOT(normal1[2], normal2[1]))) + ABS((depth1 * DOT(normal1[0], normal2[1]))) + ABS((width2 * DOT(normal1[1], normal2[2]))) + ABS((depth2 * DOT(normal1[1], normal2[0])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2y X y2z)) > (widthA * Az DOT Bz) + (depthA *  Ax DOT Bz) + (widthB * Ay DOT By) + (heightB * Ay DOT Bx))
	CROSS(destinationT, normal1[1], normal2[2]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((width1 * DOT(normal1[2], normal2[2]))) + ABS((depth1 * DOT(normal1[0], normal2[2]))) + ABS((width2 * DOT(normal1[1], normal2[1]))) + ABS((height2 * DOT(normal1[1], normal2[0])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2z X y2x)) > (widthA * Ay DOT Bx) + (heightA *  Ax DOT Bx) + (heightB * Az DOT Bz) + (depthB * Az DOT By))
	CROSS(destinationT, normal1[2], normal2[0]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((width1 * DOT(normal1[1], normal2[0]))) + ABS((height1 * DOT(normal1[0], normal2[0]))) + ABS((height2 * DOT(normal1[2], normal2[2]))) + ABS((depth2 * DOT(normal1[2], normal2[1])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2z X y2y)) > (widthA * Ay DOT By) + (heightA *  Ax DOT By) + (widthB * Az DOT Bz) + (depthB * Az DOT Bx))
	CROSS(destinationT, normal1[2], normal2[1]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((width1 * DOT(normal1[1], normal2[1]))) + ABS((height1 * DOT(normal1[0], normal2[1]))) + ABS((width2 * DOT(normal1[2], normal2[2]))) + ABS((depth2 * DOT(normal1[2], normal2[0])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//if((t . (x2z X y2z)) > (widthA * Ay DOT Bz) + (heightA *  Ax DOT Bz) + (widthB * Az DOT By) + (heightB * Az DOT Bx))
	CROSS(destinationT, normal1[2], normal2[2]);
	NORMAL(destination, destinationT);
	if(ABS(DOT(dist, destination)) > ABS((width1 * DOT(normal1[1], normal2[2]))) + ABS((height1 * DOT(normal1[0], normal2[2]))) + ABS((width2 * DOT(normal1[2], normal2[1]))) + ABS((height2 * DOT(normal1[2], normal2[0])))){
		cout << " MIDPLANE TRIGGERED " << endl;
		return false;
	}
	//cout << "No axis found" << endl;
	return true;
}

bool IntersectionTests::AABB_AABB_Intersect(double **normal1, double **normal2, double* extentA, double* extentB, double* center1, double* center2){
	double ra, rb;
	double R [3][3];
	double ABSR [3][3];

	//Compute rotation matrix
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 0; j++){
			R[i][j] = DOT(normal1[i], normal2[j]);
		}
	}

	//Translation vector T
	double t[3];
	for(int i = 0; i < 3; i++){
		t[i] = center2[i] - center1[i];
	}

	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null
	// ASBR is all the cross products that need calculating
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 0; j++){
			ABSR[i][j] = ABS(R[i][j]) + 0.05; //+ EPSILON
		}
	}

	
	// Test axes L = A0, L = A1, L = A2
	
	for(int i = 0; i < 3; i++){
		ra = extentA[i];
		rb = extentB[0] * ABSR[i][0] + extentB[1] * ABSR[i][1] + extentB[2] * ABSR[i][2];
		if (ABS(t[i]) > (ra + rb)){
			return false;
		}
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++) {
		ra = extentA[0] * ABSR[0][i] + extentA[1] * ABSR[1][i] + extentA[2] * ABSR[2][i];
		rb = extentB[i];
		if (ABS(t[i]) > (ra + rb)) return false; //checking dist*relative normals to axes
	}
	
	
	// Test axis L = A0 x B0
	ra = extentA[1] * ABSR[2][0] + extentA[2] * ABSR[1][0];
	rb = extentB[1] * ABSR[0][2] + extentB[2] * ABSR[0][1];
	if (ABS(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;
	// Test axis L = A0 x B1
	ra = extentA[1] * ABSR[2][1] + extentA[2] * ABSR[1][1];
	rb = extentB[0] * ABSR[0][2] + extentB[2] * ABSR[0][0];
	if (ABS(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;
	// Test axis L = A0 x B2
	ra = extentA[1] * ABSR[2][2] + extentA[2] * ABSR[1][2];
	rb = extentB[0] * ABSR[0][1] + extentB[1] * ABSR[0][0];
	if (ABS(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;
	// Test axis L = A1 x B0
	ra = extentA[0] * ABSR[2][0] + extentA[2] * ABSR[0][0];
	rb = extentB[1] * ABSR[1][2] + extentB[2] * ABSR[1][1];
	if (ABS(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

	// Test axis L = A1 x B2
	ra = extentA[0] * ABSR[2][2] + extentA[2] * ABSR[0][2];
	rb = extentB[0] * ABSR[1][1] + extentB[1] * ABSR[1][0];
	if (ABS(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;
	// Test axis L = A2 x B0
	ra = extentA[0] * ABSR[1][0] + extentA[1] * ABSR[0][0];
	rb = extentB[1] * ABSR[2][2] + extentB[2] * ABSR[2][1];
	if (ABS(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;
	// Test axis L = A2 x B1
	ra = extentA[0] * ABSR[1][1] + extentA[1] * ABSR[0][1];
	rb = extentB[0] * ABSR[2][2] + extentB[2] * ABSR[2][0];
	if (ABS(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;
	// Test axis L = A2 x B2
	ra = extentA[0] * ABSR[1][2] + extentA[1] * ABSR[0][2];
	rb = extentB[0] * ABSR[2][1] + extentB[1] * ABSR[2][0];
	if (ABS(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;
	// Since no separating axis is found, the OBBs must be intersecting
	return true;

}
