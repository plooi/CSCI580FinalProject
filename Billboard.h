#pragma once
#include "gz.h"

/*
Billboard: Each object of this class represents a single billboard that is rendered at a specific location
*/
class Billboard
{
public:

	Billboard(GzCoord _location, float _width, float _height, float _xRotation, float _yRotation, GzTextureStruct* _texture)
	{
		location[0] = _location[0];
		location[1] = _location[1];
		location[2] = _location[2];
		width = _width;
		height = _height;
		texture = _texture;
	};
	GzCoord* GetLocation() { return &location; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetYRotation(){ return yRotation; }
	float GetXRotation() { return xRotation; }
	GzTextureStruct* GetTexture() { return texture; }
	


	GzCoord location;//billboard's location in world space
	float width;//billboard's width in world space
	float height;//billboard's height in world space
	

	/*
	yRotation: Rotation of the billboard around the y axis in radians
	yRotation of 0 means the billboard is facing the positive x direction
	yRotation of pi/2 means the billboard is facing the forward z axis
	yRotation of pi means the billboard is facing for negative x direction
	etc...
	*/
	float yRotation;

	/*
	xRotation (-pi/2 <= xRotation <= pi/2): Rotation of the billboard around the x axis (appplied before yRotation)
	xRotation of 0 means the billboard is facing horizontally.
	xRotation of pi/2 means the billboard is facing up
	xRotation of -pi/2 means the billboard is facing down
	*/
	float xRotation;


	/*
	texture: The texture that will be drawn on this billboard
	*/
	GzTextureStruct* texture;










	/*
	
	
	
	***IGNORE THIS COMMENTED CODE IT DOESNT EXIST YOU SAW NOTHING***

	float* bumpMap;
	int bumpMapWidth;
	int bumpMapHeight;

	//TODO: THIS SHOULD INTERPOLATE, as well as return GZ_FAILURE on failure
	float GetBumpMap(int r, int c)
	{
		if (r < 0 || r >= bumpMapHeight || c < 0 || c >= bumpMapWidth) throw "Invalid index";
		return bumpMap[r * bumpMapWidth + c];
	}


	GzCoord* normalMap;
	int normalMapWidth;
	int normalMapHeight;

	//TODO: THIS SHOULD INTERPOLATE, as well as return GZ_FAILURE on failure
	GzCoord* GetNormalMap(int r, int c)
	{
		if (r < 0 || r >= normalMapHeight || c < 0 || c >= normalMapWidth) throw "Invalid index";
		return normalMap + (r * normalMapWidth + c);
	}*/

};

