#pragma once
#include "gz.h"
#ifndef UTILS
#define UTILS

/*


***************DOCUMENTATION OF CONTENTS***************





PrintingInit()
	Must be called when the program starts (or before any
	print statements are executed). This function clears the output.txt so that
	the future print statements can print to a blank file

Println(char* str)
	Prints str to the output.txt file with a newline at the end

Print(char* str)
	Prints str to the output.txt file without a newline

PrintFloat(float f)
	Prints the float 'f' to the output.txt file

PrintMatrix(GzMatrix m)
	Prints the contents of 4x4 matrix 'm' to the output.txt file

PrintCoord(GzCoord coord)
	Prints the values of coord[0] coord[1] and coord[2] to the output.txt file
	Works for GzCoord and GzColor

PrintLight(GzLight l)
	Prints the direction and color of light 'l' to the output.txt file

PrintInt(int i)
	Prints int 'i' to output.txt

SetVector3(float* vector, float a, float b, float c)
	'vector' must be an array/buffer of floats of size 3. Thus, 'vector' can also be a GzCoord
	or GzColor. This function will reassign the values of 'vector' so that the first
	value is 'a', the second value is 'b', and the 3rd value is 'c'

SetToZero(GzMatrix mat)
	Sets all values in 4x4 matrix 'mat' to 0



struct Model
	This class represents a model, and it stores information for vertex locations (field 'vertices'),
	normals, and uvs. It also keeps track of the total number of triangles in the model 
	(field 'numTriangles').
	The length of the vertices buffer must be/will be 'numTriangles' * 9
	The length of the normals buffer must be/will be 'numTriangles' * 9
	The length of the uvs buffer must be/will be 'numTriangles' * 6

	To create an instance of the model class on the stack, say

		Model m;
		m.Load("file/path/to/my/model/file.asc");


	To create an instance of the Model class on the heap, say
		Model m = LoadModel("file/path/to/my/model/file.asc");

	The file format of the model must be 

		line1:any text here is fine
		line2:point1_x point1_y point1_z point1_xnorm point1_ynorm point1_znorm point1_u point1_v
		line3:point2_x point2_y point2_z point2_xnorm point2_ynorm point2_znorm point2_u point2_v
		line4:point3_x point3_y point3_z point3_xnorm point3_ynorm point3_znorm point3_u point1_v
		line5:any text here is fine
		line6:point4_x point4_y point4_z point4_xnorm point4_ynorm point4_znorm point4_u point4_v
		...<PATTERN REPEATS>

		point number 3n+1, 3n+2, and 3n+3 are make up triangle n


	Alternatively, you can manually assign values to the vertices, normals, uvs, and numTriangles field


struct GzTextureStruct

	This class represents a texture.

	Use textureObject.Sample(float u, float v, GzColor dest) to get the color at a certain
	location in the texture (defined by u, v). The color data at that location
	will be copied into GzColor dest

	Create a GzTextureStruct object by saying
		GzTextureStruct texture = new GzTextureStruct();

	Then, you can either fill in the pixel, textureWidth and textureHeight fields via direct assignment,
	or you can say

		texture.InitFromPPMFile("MyTexturePpmFileName.ppm")

	to load that ppm file into the texture

*/
















void PrintingInit();

void Println(char* str);


void Print(char* str);

void PrintFloat(float fl);
void PrintMatrix(GzMatrix mat);

void PrintCoord(GzCoord coord);
void PrintLight(GzLight* light);
void PrintInt(int i);


void SetVector3(float* vector, float a, float b, float c);
void SetToZero(GzMatrix mat);
typedef struct Model
{
	int numTriangles;
	float* vertices;
	float* normals;
	float* uvs;
	Model* Load(char* fileName);
};

Model* LoadModel(char* fileName);
void PrintModel(Model* m);

#endif




#ifndef GZTEXTURESTRUCT
#define GZTEXTURESTRUCT





typedef struct GzTextureStruct
{
	GzColor* pixels;
	int textureWidth;
	int textureHeight;

	int floor(float f) { return (int)f; }
	int ceil(float f) { if (f == ((int)f))return (int)f; return ((int)f) + 1; }



	/*
	Constructs an empty texture
	*/
	GzTextureStruct()
	{
		pixels = NULL;
		textureWidth = 0;
		textureHeight = 0;
	}
	

	void FreeMe()
	{
		if (pixels != NULL)
		{
			free(pixels);
		}
	}



	/*
	Input the name of a ppm file and then this function opens that file
	and loads the image data into this GzTextureStruct object for use later.

	Returns this object
	*/
	GzTextureStruct* InitFromPPMFile(char* textureFile)
	{
		GzColor* image = NULL;
		FILE* fd;
		unsigned char		pixel[3];
		unsigned char     dummy;
		char  		foo[8];
		int   		i, j;
		int xs, ys;


		fd = fopen(textureFile, "rb");
		if (fd == NULL) {
			Println("ERROR: Texture file not found");
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor) * (xs + 1) * (ys + 1));
		if (image == NULL) {
			Println("ERROR: malloc for texture failed");
			fprintf(stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs * ys; i++) {	/* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}
		fclose(fd);



		textureWidth = xs;
		textureHeight = ys;
		pixels = image;
		return this;

	}


	/*
	Sample color at a specific u, v

	Result gets copied into dest
	*/
	void Sample(float u, float v, GzColor dest);
	
};
GzTextureStruct* LoadTexture(char* textureFile);





#endif
