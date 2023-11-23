#include "stdafx.h"
#include "Billboard.h"
#include "gz.h"
#include "rend.h"
#include "Utils.h"

/*
***************SEE FUCNTION DOCUMENTATION IN BILLBOARD.H***************
*/



GzTextureStruct* modelTexture;


int billboardTextureFunction(float u, float v, GzColor color)
{
	modelTexture->Sample(u, v, color);
	return GZ_SUCCESS;
}
float Abs(float f)
{
	if (f < 0)return -f;
	return f;
}

float Sq(float f)
{
	return f * f;
}
void CalculateCameraPosition(float xRotation, float yRotation, GzCoord camPosition, float* vertices, int numVertices)
{

	//calculate vertical rotation


	float farthestPointFromOrigin = 0;// the distance of the farthest point from the origin
	for (int i = 0; i < numVertices; i++)
	{
		int startIndex = i * 3;



		float distance = sqrt(Sq(vertices[startIndex]) + Sq(vertices[startIndex + 1]) + Sq(vertices[startIndex + 2]));//sqrt(x^2 + y^2 + z^2)

		if (distance > farthestPointFromOrigin)
			farthestPointFromOrigin = distance;
	}

	float camDistance = farthestPointFromOrigin-1;

	Print("Camera distance for creating the billboard is "); PrintFloat(camDistance); Println("");

	/*Print("Camera distance is ");
	PrintFloat(camDistance);
	Println("");*/



	xRotation *= -1;
	yRotation *= -1;

	
	



	//calculate the xyz in unit vector space
	float z = -1;
	float y = 0;
	float x = 0;



	z = -1*cos(xRotation);
	y = sin(xRotation);



	float hrRadius = Abs(z);

	z = hrRadius * sin(yRotation - 3.1415926525f / 2);
	x = hrRadius * cos(yRotation - 3.1415926525f / 2);

	x *= camDistance;
	y *= camDistance;
	z *= camDistance;

	camPosition[X] = x;
	camPosition[Y] = y;
	camPosition[Z] = z;


	Println("HERE");




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
	float specpower,

	float xRotation,
	float yRotation
	)
{



	this->xRotation = xRotation;
	this->yRotation = yRotation;

	GzRender* renderer = new GzRender(billboardWidthPixels, billboardHeightPixels);

	renderer->GzDefault(-4095, -4095, -4095);
	//renderer->GzDefault(0,0,0);
	//renderer->GzDefault();

	





	



	GzCamera cam;


	
	cam.FOV = 90;//45;//THIS WILL DEPEND ON HOW BIG THE MODEL IS
	Println("AA");

	CalculateCameraPosition(xRotation, yRotation, cam.position, vertices, numTriangles*3);
	//Print("Camera: "); PrintCoord(cam.position);

	//cam.position[0] = 0;
	//cam.position[1] = 0;
	//cam.position[2] = -5;//THIS WILL DEPEND ON HOW BIG THE MODEL IS

	cam.lookat[0] = 0;
	cam.lookat[1] = 0;
	cam.lookat[2] = 0;

	cam.worldup[0] = 0;
	cam.worldup[1] = 1;
	cam.worldup[2] = 0;

	renderer->GzPutCamera(cam);

	Println("BB");


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


	
	Println("CC");

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

	renderer->SetBumpMappingType(-1);

	Println("DD");


	float shiftX = 0.0f;
	float shiftY = 0.0f;

	GzToken		nameListOffset[2];		/* Offset attribute names */
	GzPointer	valueListOffset[2];		/* Offset attribute pointers */
	nameListOffset[0] = GZ_AASHIFTX;
	valueListOffset[0] = (GzPointer)&shiftX;
	nameListOffset[1] = GZ_AASHIFTY;
	valueListOffset[1] = (GzPointer)&shiftY;

	renderer->GzPutAttribute(2, nameListOffset, valueListOffset);

	Println("EE");

	

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
	
	Println("FF");
	








	GzTextureStruct* billboardTexture = new GzTextureStruct();
	billboardTexture->pixels = (GzColor*)malloc(sizeof(GzColor) * billboardHeightPixels * billboardWidthPixels);
	billboardTexture->textureWidth = billboardWidthPixels;
	billboardTexture->textureHeight = billboardHeightPixels;

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
	SetRotation(0, 0);

	FILE* f = fopen("result.ppm", "wb");
	for (int x = 0; x < billboardWidthPixels; x++)
		for (int y = 0; y < billboardHeightPixels; y++)
		{
			if (renderer->pixelbuffer[x + y * billboardHeightPixels].red == -4095)
			{
				renderer->pixelbuffer[x + y * billboardHeightPixels].red = 4095;
				renderer->pixelbuffer[x + y * billboardHeightPixels].green = 4095;
				renderer->pixelbuffer[x + y * billboardHeightPixels].blue = 4095;
			}
		}
	renderer->GzFlushDisplay2File(f);
	fclose(f);

	Println("GG");
	


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


void rotateCoord(GzCoord coord, GzMatrix mat);


/*
void Billboard::DoStuff()
{
	Billboard* a = this;
}*/

void Billboard::BillboardDraw(GzRender* renderer) {

	



	GzToken		nameListTriangle[3]; 	/* vertex attribute names */
	GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */
	GzCoord		vertexList[3];	/* vertex position coordinates */
	GzCoord		normalList[3];	/* vertex normals */
	GzTextureIndex  	uvList[3];		/* vertex texture map indices */



	/*
	 * Set up
	 */
	GzCoord     billboardVertices[4]; /* vertex positions for the billboard to be calculated */
	GzCoord     billboardNormal; /* normal direction to the billboard */
	// Billboard vertices indexed as shown below, so that 0,1,2 and 1,2,3 are the two sub-triangles
	/*
	 * 0--------------1
	 * |              |
	 * |              |
	 * 2--------------3
	 */

	GzMatrix    rotXMat; /* matrix for x rotation */
	GzMatrix    rotYMat; /* matrix for y rotation */
	GzMatrix    rotation;

	/*
	 * triangle tokens
	 */

	nameListTriangle[0] = GZ_POSITION;
	nameListTriangle[1] = GZ_NORMAL;
	nameListTriangle[2] = GZ_TEXTURE_INDEX;

	/*
	 * rotations
	 */


	SetToZero(rotXMat);
	SetToZero(rotYMat);
	SetToZero(rotation);


	 // x rotation
	rotXMat[0][0] = 1.0f;
	rotXMat[1][1] = (float)cos(GetXRotation());
	rotXMat[1][2] = (float)-sin(GetXRotation());
	rotXMat[2][1] = (float)sin(GetXRotation());
	rotXMat[2][2] = (float)cos(GetXRotation());
	rotXMat[3][3] = 1.0f;

	// y rotation
	rotYMat[1][1] = 1.0;
	rotYMat[0][0] = (float)cos(GetYRotation());
	rotYMat[2][0] = (float)-sin(GetYRotation());
	rotYMat[0][2] = (float)sin(GetYRotation());
	rotYMat[2][2] = (float)cos(GetYRotation());
	rotYMat[3][3] = 1.0f;

	// total rotation (multiplying matrices)
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// temporary variable to keep the sum from the vector vector multiplication
			float temp_sum = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				temp_sum += rotYMat[i][k] * rotXMat[k][j];
			}

			rotation[i][j] = temp_sum;
		}
	}

	/*
	 * determining billboard normals
	 * all vertices should have the same normal
	 */
	 // no rotation => x = 0, y = 0, z = -1;
	billboardNormal[X] = 0;
	billboardNormal[Y] = 0;
	billboardNormal[Z] = -1;

	rotateCoord(billboardNormal, rotation);

	/*
	 * determining billboard vertices
	 */



	 //vertex 0
	billboardVertices[0][0] = -(GetWidth() / 2.0f);
	billboardVertices[0][1] = (GetHeight() / 2.0f);
	billboardVertices[0][2] = 0;

	//vertex 1
	billboardVertices[1][0] = (GetWidth() / 2.0f);
	billboardVertices[1][1] = (GetHeight() / 2.0f);
	billboardVertices[1][2] = 0;

	//vertex 2
	billboardVertices[2][0] = -(GetWidth() / 2.0f);
	billboardVertices[2][1] = -(GetHeight() / 2.0f);
	billboardVertices[2][2] = 0;

	//vertex 3
	billboardVertices[3][0] = (GetWidth() / 2.0f);
	billboardVertices[3][1] = -(GetHeight() / 2.0f);
	billboardVertices[3][2] = 0;

	// rotation about middle of billboard


	
	for (int i = 0; i < 4; i++) {
		rotateCoord(billboardVertices[i], rotation);

		//TODO: Add coordinate translation
		billboardVertices[i][X] += this->location[X];
		billboardVertices[i][Y] += this->location[Y];
		billboardVertices[i][Z] += this->location[Z];
	}
	



	//set the texture
	modelTexture = texture;

	GzToken		nameListShader[9]; 	    /* shader attribute names */
	GzPointer   valueListShader[9];		/* shader attribute pointers */
	nameListShader[0] = GZ_TEXTURE_MAP;
	valueListShader[0] = (GzPointer)(billboardTextureFunction);
	renderer->GzPutAttribute(1, nameListShader, valueListShader);





	/*
	 * triangle values
	 */

	int numTriangles = 2;
	int numVertices = 3;
	int numCoords = 3;
	for (int i = 0; i < numTriangles; i++) {
		for (int j = 0; j < numVertices; j++) {
			for (int k = 0; k < numCoords; k++) {
				vertexList[j][k] = billboardVertices[j + i][k];
				normalList[j][k] = billboardNormal[k];
			}
		}
		// triangle 1 (vertices 0,1,2)
		if (i == 0) {
			//vertex 0
			uvList[0][0] = 0;
			uvList[0][1] = 0;
			//vertex 1
			uvList[1][0] = 1;
			uvList[1][1] = 0;
			//vertex 2
			uvList[2][0] = 0;
			uvList[2][1] = 1;
		}
		// triangle 2 (vertices 1,2,3)
		else {
			//vertex 1
			uvList[0][0] = 1;
			uvList[0][1] = 0;
			//vertex 2
			uvList[1][0] = 0;
			uvList[1][1] = 1;
			//vertex 3
			uvList[2][0] = 1;
			uvList[2][1] = 1;
		}

		valueListTriangle[0] = (GzPointer)vertexList;
		valueListTriangle[1] = (GzPointer)normalList;
		valueListTriangle[2] = (GzPointer)uvList;


		renderer->SetBumpMappingType(-1);

		renderer->GzPutTriangle(3, nameListTriangle, valueListTriangle);

	}
}




void rotateCoord(GzCoord coord, GzMatrix mat) {
	float inVector[4];
	float outVector[4];

	for (int i = 0; i < 3; i++) {
		inVector[i] = coord[i];
	}
	inVector[3] = 1;

	for (int i = 0; i < 4; i++) {
		outVector[i] = 0;
		for (int j = 0; j < 4; j++) {
			outVector[i] += mat[i][j] * inVector[j];
		}
	}

	for (int i = 0; i < 3; i++) {
		coord[i] = outVector[i] / outVector[3];
	}
}