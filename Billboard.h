#pragma once
#include "gz.h"
#include "rend.h"
#include "Utils.h"

/*
Billboard: Each object of this class represents a single billboard that is rendered at a specific location


To create a Billboard from a 3D model, first use the empty constructor
	Billboard* billboard;

Then, call the CreateFromModel method ( billboard.CreateFromModel(...) )
to load and render a given model
and then copy the rendered image into this billboard object. Now, this 
billboard has a 2D image of the model stored, and whenever we ask to 
render this billboard in the world, it will render that 2D image of 
the model.

CreateFromModel(
		int numTriangles,
		float* vertices,
		float* uvs,
		float* normals,
		GzTextureStruct* texture,
		int billboardWidthPixels,
		int billboardHeightPixels,
		GzLight ambientLight,
		GzLight* lights,
		int numLights,
		GzColor ambientCoefficient,
		GzColor specularCoefficient,
		GzColor diffuseCoefficient,
		float specpower,
		float xRotation,//radians
		float yRotation//radians
	)
	'numTriangles' is the number of triangles that are in this model
	'vertices' buffer contains vertex coordinates in the form of x1, y1, z1, x2, y2, z2, x3, y3....
		length of this buffer must be 'numTriangles'*9
	'uvs' buffer for uv coordinates in the form of u1, v1, u2, v2... 
		length must = 'numTriangles' * 6
	'normals' buffer for normals in the form of point1xnorm, point1ynorm, point1znorm, point2xnorm, point2ynorm, point2znorm...
		length must = 'numTriangles' * 9
	'texture' The texture referenced by all uv coordinates of every triangle in the model
	'billboardWidthPixels' and 'billboardHeightPixels'
		The resolution of the texutre on the billboard (which is going to be created)
	'ambientLight' ambientLight used to render model onto the billboard
	'lights' directional lights used to render model onto the billboard
	'numLights' number of directional lights
	'specpower' specular power
	'xRotation' model x rotation for rendering to billboard aka what perspective of the model 
		does the billboard show (RADIANS)
	'yRotation' model x rotation for rendering to billboard aka what perspective of the model 
		does the billboard show (RADIANS)



Next, call SetLocation to specify where in the world this billboard should be rendered, 
call SetRotation to specify which direction the billboard faces in 
the world, and call SetDimensions to specify how large (in world space) 
the billboard should be

For the next step, it is required that a GzRender object is already set up using
	renderer->GzDefault()
	renderer->GzPutCamera(...)
	renderer->GzBeginRender()

	renderer->GzPutAttribute 
		to add lights and aafilter offset (need at least 1 directional light i believe)

	then you call billboardObject.BillboardDraw(renderer)


*******************IMPORTANT*************************
For clarification:
	xRotation and yRotation parameters in CreateFromModel specify how much you want to 
	rotate the model before it is rendered onto the billboard

	xRotation and yRotation in SetRotation specify how much you want the billboard 
	plane itself to be rotated when it is drawn into the scene

*/


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
		float specpower,

		float xRotation,
		float yRotation
	);

	

	void BillboardDraw(GzRender* renderer);
	


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
	void SetRotation(float yRotation, float xRotation) { this->xRotation = xRotation; this->yRotation = -yRotation; }
	void SetDimensions(float width, float height) { this->width = width; this->height = height; }
	


};

