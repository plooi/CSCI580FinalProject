#include "stdafx.h"
#include "Billboard.h"
#include "gz.h"
#include "rend.h"
#include "Utils.h"

GzTextureStruct* modelTexture;

void SetVector3(float* vector, float a, float b, float c);

int billboardTextureFunction(float u, float v, GzColor color)
{

	modelTexture->Sample(u, v, color);
	return GZ_SUCCESS;
}


Billboard* Billboard::CreateFromModel(
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
	)
{
	GzRender* renderer = new GzRender(billboardWidthPixels, billboardHeightPixels);

	//renderer->GzDefault(-4095, -4095, -4095);
	renderer->GzDefault();

	


	GzCamera cam;


	
	cam.FOV = 45;//THIS WILL DEPEND ON HOW BIG THE MODEL IS
	
	cam.position[0] = 0;
	cam.position[1] = 0;
	cam.position[2] = -5;//THIS WILL DEPEND ON HOW BIG THE MODEL IS

	cam.lookat[0] = 0;
	cam.lookat[1] = 0;
	cam.lookat[2] = 0;

	cam.worldup[0] = 0;
	cam.worldup[1] = 1;
	cam.worldup[2] = 0;

	renderer->GzPutCamera(cam);




	renderer->GzBeginRender();

	GzToken* nameListLights = (GzToken*)malloc((numLights+1) * sizeof(GzToken));
	GzPointer* valueListLights = (GzPointer*)malloc((numLights+1) * sizeof(GzPointer));

	for (int i = 0; i < numLights; i++)
	{
		nameListLights[i] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[i] = (GzPointer)(lights + i);
	}

	nameListLights[numLights] = GZ_AMBIENT_LIGHT;
	valueListLights[numLights] = ambientLight;

	renderer->GzPutAttribute(numLights+1, nameListLights, valueListLights);


	


	GzToken		nameListShader[9]; 	    /* shader attribute names */
	GzPointer   valueListShader[9];		/* shader attribute pointers */

	modelTexture = texture;

	int			shaderType, interpStyle;
	nameListShader[0] = GZ_DIFFUSE_COEFFICIENT;
	valueListShader[0] = (GzPointer)diffuseCoefficient;
	nameListShader[1] = GZ_INTERPOLATE;
	//interpStyle = GZ_COLOR;         /* Gouraud shading */
	interpStyle = GZ_NORMALS;         /* Phong shading */
	valueListShader[1] = (GzPointer)&interpStyle;
	nameListShader[2] = GZ_AMBIENT_COEFFICIENT;
	valueListShader[2] = (GzPointer)ambientCoefficient;
	nameListShader[3] = GZ_SPECULAR_COEFFICIENT;
	valueListShader[3] = (GzPointer)specularCoefficient;
	nameListShader[4] = GZ_DISTRIBUTION_COEFFICIENT;
	valueListShader[4] = (GzPointer)&specpower;
	nameListShader[5] = GZ_TEXTURE_MAP;
	valueListShader[5] = (GzPointer)(billboardTextureFunction);	/* or use ptex_fun */

	renderer->GzPutAttribute(6, nameListShader, valueListShader);




	float shiftX = 0.0f;
	float shiftY = 0.0f;

	GzToken		nameListOffset[2];		/* Offset attribute names */
	GzPointer	valueListOffset[2];		/* Offset attribute pointers */
	nameListOffset[0] = GZ_AASHIFTX;
	valueListOffset[0] = (GzPointer)&shiftX;
	nameListOffset[1] = GZ_AASHIFTY;
	valueListOffset[1] = (GzPointer)&shiftY;

	renderer->GzPutAttribute(2, nameListOffset, valueListOffset);



	

	//for each triangle, draw it!
	for (int triangleNum = 0; triangleNum < numTriangles; triangleNum++)
	{
		GzToken		nameListTriangle[3]; 	/* vertex attribute names */
		GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */
		GzCoord		vertexList[3];	/* vertex position coordinates */
		GzCoord		normalList[3];	/* vertex normals */
		GzTextureIndex  	uvList[3];		/* vertex texture map indices */


		int triStart = triangleNum * 9;
		int triStartUV = triangleNum * 6;
		vertexList[0][X] = vertices[triStart + 0];
		vertexList[0][Y] = vertices[triStart + 1];
		vertexList[0][Z] = vertices[triStart + 2];
		vertexList[1][X] = vertices[triStart + 3];
		vertexList[1][Y] = vertices[triStart + 4];
		vertexList[1][Z] = vertices[triStart + 5];
		vertexList[2][X] = vertices[triStart + 6];
		vertexList[2][Y] = vertices[triStart + 7];
		vertexList[2][Z] = vertices[triStart + 8];

		normalList[0][X] = normals[triStart + 0];
		normalList[0][Y] = normals[triStart + 1];
		normalList[0][Z] = normals[triStart + 2];
		normalList[1][X] = normals[triStart + 3];
		normalList[1][Y] = normals[triStart + 4];
		normalList[1][Z] = normals[triStart + 5];
		normalList[2][X] = normals[triStart + 6];
		normalList[2][Y] = normals[triStart + 7];
		normalList[2][Z] = normals[triStart + 8];

		uvList[0][U] = uvs[triStartUV + 0];
		uvList[0][V] = uvs[triStartUV + 1];
		uvList[1][U] = uvs[triStartUV + 2];
		uvList[1][V] = uvs[triStartUV + 3];
		uvList[2][U] = uvs[triStartUV + 4];
		uvList[2][V] = uvs[triStartUV + 5];

		nameListTriangle[0] = GZ_POSITION;
		nameListTriangle[1] = GZ_NORMAL;
		nameListTriangle[2] = GZ_TEXTURE_INDEX;
		valueListTriangle[0] = (GzPointer)vertexList;
		valueListTriangle[1] = (GzPointer)normalList;
		valueListTriangle[2] = (GzPointer)uvList;

		renderer->GzPutTriangle(3, nameListTriangle, valueListTriangle);
	}
	FILE* f = fopen("result.ppm", "wb");
	
	renderer->GzFlushDisplay2File(f);

	fclose(f);

	








	GzTextureStruct* billboardTexture = new GzTextureStruct();
	billboardTexture->pixels = (GzColor*)malloc(sizeof(GzColor) * billboardHeightPixels * billboardWidthPixels);


	for(int x = 0; x < billboardWidthPixels; x++)
		for (int y = 0; y < billboardHeightPixels; y++)
		{
			SetVector3(billboardTexture->pixels[x + y * billboardHeightPixels],
				renderer->pixelbuffer[x + y * billboardHeightPixels].red / 4095.0f,
				renderer->pixelbuffer[x + y * billboardHeightPixels].green / 4095.0f,
				renderer->pixelbuffer[x + y * billboardHeightPixels].blue / 4095.0f
				);
			 
		}
	this->texture = billboardTexture;
	SetVector3(this->location, 0, 0, 0);
	SetDimensions(1, 1);
	SetRotation(3 * 3.14159f / 2, 0);


	


	free(nameListLights);
	free(valueListLights);
	delete renderer;
	/*
	for (int x = 0; x < billboardWidthPixels; x++)
		for (int y = 0; y < billboardHeightPixels; y++)
		{
			Print("Pixel "); PrintInt(x); Print(", "); PrintInt(y); Print(": "); PrintCoord(billboardTexture->pixels[x + y * billboardHeightPixels]); Println("");
		}*/

	return this;
	
	
		
}