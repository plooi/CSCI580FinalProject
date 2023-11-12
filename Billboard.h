#pragma once
#include "gz.h"

/*



*/
class Billboard
{
public:
	float* bumpMap;
	int bumpMapWidth;
	int bumpMapHeight;
	float GetBumpMap(int r, int c)
	{
		if (r < 0 || r >= bumpMapHeight || c < 0 || c >= bumpMapWidth) throw "Invalid index";
		return bumpMap[r * bumpMapWidth + c];
	}


	GzCoord* normalMap;
	int normalMapWidth;
	int normalMapHeight;
	GzCoord* GetNormalMap(int r, int c)
	{
		if (r < 0 || r >= normalMapHeight || c < 0 || c >= normalMapWidth) throw "Invalid index";
		return normalMap + (r * normalMapWidth + c);
	}

	float billboardWidth;
	float billboardHeight;

	GzColor* texture;
	int textureWidth;
	int textureHeight;
	void SampleTexture(int u, int v, GzColor dest);

};

