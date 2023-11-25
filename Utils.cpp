#include "stdafx.h"
#include "Utils.h"
#include <stdio.h>
#include "stdlib.h"

/*
***************SEE UTILS.H FOR DOCUMENTATION***************
*/

void PrintingInit()
{
	FILE* f = fopen("./output.txt", "w");
	fputs("", f);
	fclose(f);
}


void Println(char* str)
{
	FILE* f = fopen("./output.txt", "a+");
	fputs(str, f);
	fputs("\n", f);
	fclose(f);
}



void Print(char* str)
{
	FILE* f = fopen("./output.txt", "a+");
	fputs(str, f);
	fclose(f);
}

void PrintFloat(float fl)
{
	FILE* f = fopen("./output.txt", "a+");
	char buffer[500];
	sprintf(buffer, "%f", fl);
	fputs(buffer, f);
	fclose(f);
}
void PrintMatrix(GzMatrix mat)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			PrintFloat(mat[r][c]);
			Print(" ");
		}
		Print("\n");
	}
}

void PrintCoord(GzCoord coord)
{
	Print("(");
	for (int i = 0; i < 3; i++)
	{
		PrintFloat(coord[i]);
		Print(" ");
	}
	Print(")");
}

void PrintLight(GzLight* light)
{
	Print("Light: ");
	PrintCoord(light->color);
	Print(" : ");
	PrintCoord(light->direction);
	Print("\n");
}

void PrintInt(int i)
{
	FILE* f = fopen("./output.txt", "a+");
	char buffer[500];
	sprintf(buffer, "%d", i);
	fputs(buffer, f);
	fclose(f);
}

void SetVector3(float* vector, float a, float b, float c)
{
	vector[0] = a;
	vector[1] = b;
	vector[2] = c;
}
void SetToZero(GzMatrix mat)
{
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
			mat[r][c] = 0;
	
}


void PrintModel(Model* m)
{
	Print("Model with ");
	PrintInt(m->numTriangles);
	Println(" vertices:");
	for (int i = 0; i < m->numTriangles; i++)
	{
		PrintCoord(m->vertices + i * 9);
		PrintCoord(m->vertices + i * 9 + 3);
		PrintCoord(m->vertices + i * 9 + 6);
		Println("");
	}
	
}


Model* Model::Load(char* fileName)
{
	char dummy[256];
	FILE* infile;
	GzCoord vertexList[3];	
	GzCoord	normalList[3];	
	GzTextureIndex uvList[3];


	int numTriangles = 0;
	//count lines
	if ((infile = fopen(fileName, "r")) == NULL)
	{
		Print("File ");
		Print(fileName);
		Println(" could not be opened (it probably doesn't exist)");
		return NULL;
	}
	while (fscanf(infile, "%s", dummy) == 1) 
	{
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[0][0]), &(vertexList[0][1]),
			&(vertexList[0][2]),
			&(normalList[0][0]), &(normalList[0][1]),
			&(normalList[0][2]),
			&(uvList[0][0]), &(uvList[0][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[1][0]), &(vertexList[1][1]),
			&(vertexList[1][2]),
			&(normalList[1][0]), &(normalList[1][1]),
			&(normalList[1][2]),
			&(uvList[1][0]), &(uvList[1][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[2][0]), &(vertexList[2][1]),
			&(vertexList[2][2]),
			&(normalList[2][0]), &(normalList[2][1]),
			&(normalList[2][2]),
			&(uvList[2][0]), &(uvList[2][1]));

		numTriangles++;
	}

	fclose(infile);






	//now allocate the buffers
	float* vertices = (float*)malloc(sizeof(float) * numTriangles * 9);
	float* normals = (float*)malloc(sizeof(float) * numTriangles * 9);
	float* uvs = (float*)malloc(sizeof(float) * numTriangles * 6);
	


	//now fill the buffers
	if ((infile = fopen(fileName, "r")) == NULL)
	{
		Print("File ");
		Print(fileName);
		Println(" could not be opened (it probably doesn't exist)");
		return NULL;
	}


	int i = 0;//which vertex number we're on
	while (fscanf(infile, "%s", dummy) == 1)
	{
		for (int j = 0; j < 3; j++)
		{
			fscanf(infile, "%f %f %f %f %f %f %f %f",
				&(vertexList[0][0]), &(vertexList[0][1]),
				&(vertexList[0][2]),
				&(normalList[0][0]), &(normalList[0][1]),
				&(normalList[0][2]),
				&(uvList[0][0]), &(uvList[0][1]));



			vertices[i * 3] = vertexList[0][0];
			vertices[i * 3 + 1] = vertexList[0][1];
			vertices[i * 3 + 2] = vertexList[0][2];
			normals[i * 3] = normalList[0][0];
			normals[i * 3 + 1] = normalList[0][1];
			normals[i * 3 + 2] = normalList[0][2];
			uvs[i * 2] = uvList[0][0];
			uvs[i * 2 + 1] = uvList[0][1];

			i++;
		}
		
	}

	fclose(infile);


	this->vertices = vertices;
	this->normals = normals;
	this->uvs = uvs;
	this->numTriangles = numTriangles;
	return this;
}




void GzTextureStruct::Sample(float u, float v, GzColor dest)
{

	if (u > 1) u = 1; if (u < 0) u = 0; if (v > 1) v = 1; if (v < 0) v = 0;
	if (textureWidth <= 0 || textureHeight <= 0)
	{
		dest[RED] = 0;
		dest[GREEN] = 0;
		dest[BLUE] = 0;
		return;
	}

	float xPixel = u * (textureWidth - 1);
	float yPixel = v * (textureHeight - 1);
	if (xPixel > textureWidth - 1) xPixel = textureWidth - 1;
	if (yPixel > textureHeight - 1) yPixel = textureHeight - 1;

	GzColor* upperLeft;
	GzColor* upperRight;
	GzColor* lowerLeft;
	GzColor* lowerRight;
	upperLeft = pixels + floor(yPixel) * textureWidth + floor(xPixel);
	upperRight = pixels + floor(yPixel) * textureWidth + ceil(xPixel);
	lowerLeft = pixels + ceil(yPixel) * textureWidth + floor(xPixel);
	lowerRight = pixels + ceil(yPixel) * textureWidth + ceil(xPixel);

	float xInterp = xPixel - floor(xPixel);
	float yInterp = yPixel - floor(yPixel);
	/*if (
		((*upperLeft)[RED] == -1 && xInterp <= .5f && yInterp <= .5f) ||
		((*upperRight)[RED] == -1 && xInterp >= .5f && yInterp <= .5f) ||
		((*lowerLeft)[RED] == -1 && xInterp <= .5f && yInterp >= .5f) ||
		((*lowerRight)[RED] == -1 && xInterp >= .5f && yInterp >= .5f)
		)*/
	if (
		((*upperLeft)[RED] == -1) ||
		((*upperRight)[RED] == -1) ||
		((*lowerLeft)[RED] == -1) ||
		((*lowerRight)[RED] == -1)
		)
	{
		dest[RED] = -1;
		dest[GREEN] = -1;
		dest[BLUE] = -1;
		return;
	}
	GzColor topInterp = {
		xInterp * (*upperRight)[RED] + (1 - xInterp) * (*upperLeft)[RED],
		xInterp * (*upperRight)[GREEN] + (1 - xInterp) * (*upperLeft)[GREEN],
		xInterp * (*upperRight)[BLUE] + (1 - xInterp) * (*upperLeft)[BLUE],
	};
	GzColor botInterp = {
		xInterp * (*lowerRight)[RED] + (1 - xInterp) * (*lowerLeft)[RED],
		xInterp * (*lowerRight)[GREEN] + (1 - xInterp) * (*lowerLeft)[GREEN],
		xInterp * (*lowerRight)[BLUE] + (1 - xInterp) * (*lowerLeft)[BLUE],
	};
	dest[RED] = yInterp * botInterp[RED] + (1 - yInterp) * topInterp[RED];
	dest[GREEN] = yInterp * botInterp[GREEN] + (1 - yInterp) * topInterp[GREEN];
	dest[BLUE] = yInterp * botInterp[BLUE] + (1 - yInterp) * topInterp[BLUE];

}

GzTextureStruct* LoadTexture(char* textureFile) { return (new GzTextureStruct())->InitFromPPMFile(textureFile); }
Model* LoadModel(char* fileName) { return (new Model())->Load(fileName); }







float Abs(float f)
{
	if (f < 0)return -f;
	return f;
}

float Sq(float f)
{
	return f * f;
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
