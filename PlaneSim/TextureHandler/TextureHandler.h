#ifndef _TEXUTUREHANDLER_H
#define _TEXUTUREHANDLER_H

#include "../images/imageLoader.h"
#include <map>
#include <string>

using namespace std;

class TextureHandler
{
private:
	static int boundTextures[4];
	
	static std::map<std::string, int> theTextureMap;	
	
	static int loadTexture(char *fileName, bool buildMipMaps);

	static bool searchMap(char *fileName, int &result);

public:
	static void clearAllBoundTextures();
	static void clearBoundTextures(int textUnit);
	static void bindTexture(int textUnit, int texture);

	static int lookUpTexture(char *fileName, bool buildMipMaps);

	static string reverseLookupTexture(int textID);

	static void destroyAllTextures();



};


#endif