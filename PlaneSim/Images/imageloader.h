#ifndef TGALOADER_H
#define TGALOADER_H

#include "jpeglib.h"

#include <string>
//#include <GL/glext.h>
#include <gl/gl.h>
#include <gl/glu.h>

// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

// This is our image structure for our targa data
struct tImageTGA
{
	int channels;			// The channels in the image (3 = RGB : 4 = RGBA)
	int sizeX;				// The width of the image in pixels
	int sizeY;				// The height of the image in pixels
	unsigned char *data;	// The image pixel data
};

struct gliGenericImage
{
  GLsizei  width;
  GLsizei  height;
  GLint    components;
  GLenum   format;

  GLsizei  cmapEntries;
  GLenum   cmapFormat;
  GLubyte *cmap;

  GLubyte *pixels;
};

class ImageLoader
{
private:
	static gliGenericImage* readDDSFile(const char *filename, int & bufsize, int & numMipmaps); 
	static void GLErrorReport();

public:
	static void loadExtensions();
	
	static tImageTGA *LoadTGA(const char *filename);
	static int createTGA(const char* strFileName, bool mipmap);
	
	static GLuint loadPNG(const char* filename);

	static GLuint loadDDS(const char *fileName);

	static int createJPG(const char* strFileName);
	static void DecodeJPG(jpeg_decompress_struct* cinfo, tImageJPG *pImageData);
	static tImageJPG* LoadJPG(const char *filename);


};

#endif TGALOADER_H