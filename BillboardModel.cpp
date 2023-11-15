#include "stdafx.h"
#include "BillboardModel.h"
#include "rend.h"


GzTextureStruct currentTexture;

int tempTextureFunction(float u, float v, GzColor color)
{
	currentTexture.Sample(u, v, color);

	return GZ_SUCCESS;
}


int BillboardModel::CreateBillboardModelFromRegularModel(int numVertices, float* vertices, float* uvs, float* normals, GzTextureStruct* texture, int width, int height)
{
	//1. Create a gzRenderer object
	//3. Load the texture into the static buffer (which is currentTexture)
	//4. Put the texture function into the gzRenderer object
	//5. Calculate what distance from the origin the camera should be at to barely fit the model within the output image
		//camera should be very far away with very small fov
	//6. Put the camera at that location and have the camera look at the origin and set the fov of the camera
	//7. Render the image using the vertices, uvs, and normals via the GzPutTriangle function. Don't show the image on the screen though.
	//8. Somehow get ahold of the image space version of the output and create a billboard texture with that image. Calculate bump map using the z buffer and calculate normals by looking at neighboring heights
	//9. add that information to this object's fields
	//10. repeat steps 6-9 for each angle of the model that you want to create a billboard for
	GzRender* renderer = new GzRender(width, height);

	renderer->GzDefault();


	//setup camera



	renderer->GzBeginRender();
	return GZ_SUCCESS;
}