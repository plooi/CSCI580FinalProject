#include "stdafx.h"
#include "Billboard.h"
#include "gz.h"
#include "rend.h"
#include "Utils.h"

GzTextureStruct* modelTexture;

void SetVector3(float* vector, float a, float b, float c);
void rotateCoord(GzCoord coord, GzMatrix mat);

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

void Billboard::Draw(GzRender* renderer) {
    /*
     * Set up
     */

    GzToken		nameListTriangle[3]; 	/* vertex attribute names */
    GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */
    GzCoord		vertexList[3];	/* vertex position coordinates */
    GzCoord		normalList[3];	/* vertex normals */
    GzTextureIndex  	uvList[3];		/* vertex texture map indices */

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

    // x rotation
    rotXMat[0][0] = 1.0f;
    rotXMat[1][1] = (float)cos(this->GetXRotation());
    rotXMat[1][2] = (float)-sin(this->GetXRotation());
    rotXMat[2][1] = (float)sin(this->GetXRotation());
    rotXMat[2][2] = (float)cos(this->GetXRotation());
    rotXMat[3][3] = 1.0f;

    // y rotation
    rotYMat[1][1] = 1.0;
    rotYMat[0][0] = (float)cos(this->GetYRotation());
    rotYMat[2][0] = (float)-sin(this->GetYRotation());
    rotYMat[0][2] = (float)sin(this->GetYRotation());
    rotYMat[2][2] = (float)cos(this->GetYRotation());
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
    billboardNormal[0] = 0;
    billboardNormal[0] = 0;
    billboardNormal[0] = -1;

    rotateCoord( billboardNormal, rotation);

    /*
     * determining billboard vertices
     */

    // before rotation

    //vertex 0
    billboardVertices[0][0] = - (this->GetWidth() / 2.0f);
    billboardVertices[0][1] = (this->GetHeight() / 2.0f);
    billboardVertices[0][2] = 0;

    //vertex 1
    billboardVertices[1][0] = (this->GetWidth() / 2.0f);
    billboardVertices[1][1] = (this->GetHeight() / 2.0f);
    billboardVertices[1][2] = 0;

    //vertex 2
    billboardVertices[2][0] = - (this->GetWidth() / 2.0f);
    billboardVertices[2][1] = - (this->GetHeight() / 2.0f);
    billboardVertices[2][2] = 0;

    //vertex 3
    billboardVertices[3][0] = (this->GetWidth() / 2.0f);
    billboardVertices[3][1] = - (this->GetHeight() / 2.0f);
    billboardVertices[3][2] = 0;

    // rotation about middle of billboard

    for (int i = 0; i < 4; i++) {
        rotateCoord(billboardVertices[i], rotation);
    }

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
                normalList[j][k] = billboardNormal;
            }
        }
        // triangle 1 (vertices 0,1,2)
        if (i == 0) {
            //vertex 0
            uvList[0][0] = 0;
            uvList[0][1] = 0;
            //vertex 1
            uvlist[1][0] = 1;
            uvList[1][1] = 0;
            //vertex 2
            uvList[2][0] = 0;
            uvList[2][1] = 1;
        }
        // triangle 2 (vertices 1,2,3)
        else {
            //vertex 1
            uvlist[0][0] = 1;
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