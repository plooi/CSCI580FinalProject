#pragma once
#include "gz.h"

/*
Billboard: Each object of this class represents a single billboard that is rendered at a specific location


To create a Billboard from a 3D model, first use the empty constructor
	Billboard* billboard = new Billboard();

Then, call the CreateFromModel method to load and render a given model
and then copy the rendered image into this billboard object. Now, this 
billboard has a 2D image of the model stored, and whenever we ask to 
render this billboard in the world, it will render that 2D image of 
the model.


	billboard->CreateFromModel(
		numTriangles,
		vertices,
		uvs,
		normals,
		&texture,
		billboardWidthPixels,
		billboardHeightPixels,
		&ambientLight,
		lights,
		numLights,
		ambientCoefficient,
		specularCoefficient,
		diffuseCoefficient,
		specpower
	);


Next, call SetLocation to specify where in the world this billboard should be rendered, 
call SetRotation to specify which direction it's facing, and call SetDimensions to specify
how large (in world space) the billboard should be
*/


void SetVector3(float* vector, float a, float b, float c);
class Billboard
{
public:



	GzTextureStruct* texture;//texture: The texture that will be drawn on this billboard
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
	Renders the given model with the given parameters, and then assigns the output image to this billboard's 
	texture field
	*/
	Billboard* CreateFromModel(
		int numTriangles,
		float* vertices,
		float* uvs,
		float* normals,
		GzTextureStruct* texture,
		int billboardWidthPixels,
		int billboardHeightPixels,
		GzLight* ambientLight,
		GzLight* lights,
		int numLights,
		GzColor ambientCoefficient,
		GzColor specularCoefficient,
		GzColor diffuseCoefficient,
		float specpower
	);



	Billboard()
	{
		location[0] = 0;
		location[1] = 0;
		location[2] = 0;
		width = 0;
		height = 0;
		texture = NULL;
		yRotation = 0;
		xRotation = 0;
	}
	Billboard(GzCoord _location, float _width, float _height, float _xRotation, float _yRotation, GzTextureStruct* _texture)
	{
		location[0] = _location[0];
		location[1] = _location[1];
		location[2] = _location[2];
		width = _width;
		height = _height;
		texture = _texture;
		yRotation = _xRotation;
		xRotation = _yRotation;
	};
	GzCoord* GetLocation() { return &location; }
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetYRotation(){ return yRotation; }
	float GetXRotation() { return xRotation; }
	GzTextureStruct* GetTexture() { return texture; }
	void SetLocation(GzCoord _location){SetVector3(location, _location[0], _location[1], _location[2]);}
	void SetRotation(float yRotation, float xRotation) { this->xRotation = xRotation; this->yRotation = yRotation; }
	void SetDimensions(float width, float height) { this->width = width; this->height = height; }
	

	

	





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

