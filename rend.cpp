/* CS580 Homework 4 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include "Utils.h"

// iostream
#include <iostream>
#include <bitset>
#pragma warning disable 1591 
void sortVerticesCW(GzCoord*, GzCoord*);
void calculatePlaneNormal(GzCoord*, float*);
void initializeGzMatrix(GzMatrix);
void initializeGzMatrixIdentity(GzMatrix);
void normalizeVector(GzCoord);
float dotProduct(GzCoord, GzCoord);
void crossProduct(GzCoord, GzCoord, GzCoord);
void matrixMultiplication(GzMatrix, GzMatrix, GzMatrix);
void extractRotationMatrix(GzMatrix, GzMatrix);
void calculateLightingColorsThreeVertices(GzCoord*, GzColor, GzColor, GzColor, GzLight, GzLight*, int, float, GzColor*);
void calculateLightingColor(GzCoord, GzColor, GzColor, GzColor, GzLight, GzLight*, int, float, GzColor);
void clampColor(GzColor);

#define PI (float) 3.14159265358979323846


// Function to print vector to a file
void printVectorToFile(const char* fileName, const char* vectorName, const float* vector, int size) {
	FILE* file = fopen(fileName, "a"); // Open the file in append mode

	if (file != NULL) {
		fprintf(file, "%s: ", vectorName);
		for (int i = 0; i < size; ++i) {
			fprintf(file, "%.6f ", vector[i]);
		}
		fprintf(file, "\n");

		fclose(file);
	}
	else {
		fprintf(stderr, "Error opening file: %s\n", fileName);
	}
}

bool Inverse(float T[][4], float Target[][4])
{
	float inv[16], det, invOut[16];
	int i, j, k;

	inv[0] = T[1][1] * T[2][2] * T[3][3] -
		T[1][1] * T[2][3] * T[3][2] -
		T[2][1] * T[1][2] * T[3][3] +
		T[2][1] * T[1][3] * T[3][2] +
		T[3][1] * T[1][2] * T[2][3] -
		T[3][1] * T[1][3] * T[2][2];

	inv[4] = -T[1][0] * T[2][2] * T[3][3] +
		T[1][0] * T[2][3] * T[3][2] +
		T[2][0] * T[1][2] * T[3][3] -
		T[2][0] * T[1][3] * T[3][2] -
		T[3][0] * T[1][2] * T[2][3] +
		T[3][0] * T[1][3] * T[2][2];

	inv[8] = T[1][0] * T[2][1] * T[3][3] -
		T[1][0] * T[2][3] * T[3][1] -
		T[2][0] * T[1][1] * T[3][3] +
		T[2][0] * T[1][3] * T[3][1] +
		T[3][0] * T[1][1] * T[2][3] -
		T[3][0] * T[1][3] * T[2][1];

	inv[12] = -T[1][0] * T[2][1] * T[3][2] +
		T[1][0] * T[2][2] * T[3][1] +
		T[2][0] * T[1][1] * T[3][2] -
		T[2][0] * T[1][2] * T[3][1] -
		T[3][0] * T[1][1] * T[2][2] +
		T[3][0] * T[1][2] * T[2][1];

	inv[1] = -T[0][1] * T[2][2] * T[3][3] +
		T[0][1] * T[2][3] * T[3][2] +
		T[2][1] * T[0][2] * T[3][3] -
		T[2][1] * T[0][3] * T[3][2] -
		T[3][1] * T[0][2] * T[2][3] +
		T[3][1] * T[0][3] * T[2][2];

	inv[5] = T[0][0] * T[2][2] * T[3][3] -
		T[0][0] * T[2][3] * T[3][2] -
		T[2][0] * T[0][2] * T[3][3] +
		T[2][0] * T[0][3] * T[3][2] +
		T[3][0] * T[0][2] * T[2][3] -
		T[3][0] * T[0][3] * T[2][2];

	inv[9] = -T[0][0] * T[2][1] * T[3][3] +
		T[0][0] * T[2][3] * T[3][1] +
		T[2][0] * T[0][1] * T[3][3] -
		T[2][0] * T[0][3] * T[3][1] -
		T[3][0] * T[0][1] * T[2][3] +
		T[3][0] * T[0][3] * T[2][1];

	inv[13] = T[0][0] * T[2][1] * T[3][2] -
		T[0][0] * T[2][2] * T[3][1] -
		T[2][0] * T[0][1] * T[3][2] +
		T[2][0] * T[0][2] * T[3][1] +
		T[3][0] * T[0][1] * T[2][2] -
		T[3][0] * T[0][2] * T[2][1];

	inv[2] = T[0][1] * T[1][2] * T[3][3] -
		T[0][1] * T[1][3] * T[3][2] -
		T[1][1] * T[0][2] * T[3][3] +
		T[1][1] * T[0][3] * T[3][2] +
		T[3][1] * T[0][2] * T[1][3] -
		T[3][1] * T[0][3] * T[1][2];

	inv[6] = -T[0][0] * T[1][2] * T[3][3] +
		T[0][0] * T[1][3] * T[3][2] +
		T[1][0] * T[0][2] * T[3][3] -
		T[1][0] * T[0][3] * T[3][2] -
		T[3][0] * T[0][2] * T[1][3] +
		T[3][0] * T[0][3] * T[1][2];

	inv[10] = T[0][0] * T[1][1] * T[3][3] -
		T[0][0] * T[1][3] * T[3][1] -
		T[1][0] * T[0][1] * T[3][3] +
		T[1][0] * T[0][3] * T[3][1] +
		T[3][0] * T[0][1] * T[1][3] -
		T[3][0] * T[0][3] * T[1][1];

	inv[14] = -T[0][0] * T[1][1] * T[3][2] +
		T[0][0] * T[1][2] * T[3][1] +
		T[1][0] * T[0][1] * T[3][2] -
		T[1][0] * T[0][2] * T[3][1] -
		T[3][0] * T[0][1] * T[1][2] +
		T[3][0] * T[0][2] * T[1][1];

	inv[3] = -T[0][1] * T[1][2] * T[2][3] +
		T[0][1] * T[1][3] * T[2][2] +
		T[1][1] * T[0][2] * T[2][3] -
		T[1][1] * T[0][3] * T[2][2] -
		T[2][1] * T[0][2] * T[1][3] +
		T[2][1] * T[0][3] * T[1][2];

	inv[7] = T[0][0] * T[1][2] * T[2][3] -
		T[0][0] * T[1][3] * T[2][2] -
		T[1][0] * T[0][2] * T[2][3] +
		T[1][0] * T[0][3] * T[2][2] +
		T[2][0] * T[0][2] * T[1][3] -
		T[2][0] * T[0][3] * T[1][2];

	inv[11] = -T[0][0] * T[1][1] * T[2][3] +
		T[0][0] * T[1][3] * T[2][1] +
		T[1][0] * T[0][1] * T[2][3] -
		T[1][0] * T[0][3] * T[2][1] -
		T[2][0] * T[0][1] * T[1][3] +
		T[2][0] * T[0][3] * T[1][1];

	inv[15] = T[0][0] * T[1][1] * T[2][2] -
		T[0][0] * T[1][2] * T[2][1] -
		T[1][0] * T[0][1] * T[2][2] +
		T[1][0] * T[0][2] * T[2][1] +
		T[2][0] * T[0][1] * T[1][2] -
		T[2][0] * T[0][2] * T[1][1];

	det = T[0][0] * inv[0] + T[0][1] * inv[4] + T[0][2] * inv[8] + T[0][3] * inv[12];

	if (det == 0)
		return false;


	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	Target[0][0] = invOut[0];
	Target[0][1] = invOut[1];
	Target[0][2] = invOut[2];
	Target[0][3] = invOut[3];
	Target[1][0] = invOut[4];
	Target[1][1] = invOut[5];
	Target[1][2] = invOut[6];
	Target[1][3] = invOut[7];
	Target[2][0] = invOut[8];
	Target[2][1] = invOut[9];
	Target[2][2] = invOut[10];
	Target[2][3] = invOut[11];
	Target[3][0] = invOut[12];
	Target[3][1] = invOut[13];
	Target[3][2] = invOut[14];
	Target[3][3] = invOut[15];

	return true;

}

/*Multiply a 4D vector with a 4*4 matrix */
void MatrixMulVector(float NewVec[4], float A[4][4], float Vec[3], float type) {
	int i, j;
	for (i = 0; i < 4; i++)
	{
		NewVec[i] = 0.0;
		for (j = 0; j < 4; j++)
		{
			if (j == 3)
				NewVec[i] += A[i][j] * type;
			else
				NewVec[i] += A[i][j] * Vec[j];
		}
	}
}

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/
	// initialize mat with all zeros
	initializeGzMatrix(mat);

	// convert degree to radian
	float angle_rad = (float)(degree * PI) / 180.0f;

	// rotation around x
	mat[0][0] = 1.0f;
	mat[1][1] = (float)cos(angle_rad);
	mat[1][2] = (float)-sin(angle_rad);
	mat[2][1] = (float)sin(angle_rad);
	mat[2][2] = (float)cos(angle_rad);
	mat[3][3] = 1.0f;

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/

	// initialize mat with all zeros
	initializeGzMatrix(mat);

	// convert degree to radian
	float angle_rad = (float)(degree * PI) / 180.0f;

	// rotation around y
	mat[1][1] = 1.0;
	mat[0][0] = (float)cos(angle_rad);
	mat[2][0] = (float)-sin(angle_rad);
	mat[0][2] = (float)sin(angle_rad);
	mat[2][2] = (float)cos(angle_rad);
	mat[3][3] = 1.0f;

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/

	// initialize mat with all zeros
	initializeGzMatrix(mat);

	// convert degree to radian
	float angle_rad = (float)(degree * PI) / 180.0f;

	// rotation around z
	mat[2][2] = 1.0;
	mat[0][0] = (float)cos(angle_rad);
	mat[0][1] = (float)-sin(angle_rad);
	mat[1][0] = (float)sin(angle_rad);
	mat[1][1] = (float)cos(angle_rad);
	mat[3][3] = 1.0f;

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/

	// initialize mat with all zeros
	initializeGzMatrix(mat);

	// diagonals are all 1
	for (int i = 0; i < 4; i++)
		mat[i][i] = 1.0f;

	// translate
	for (int i = 0; i < 3; i++)
		mat[i][3] = translate[i];

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/

	// initialize mat with all zeros
	initializeGzMatrix(mat);

	// last diagonal is 1
	mat[3][3] = 1.0f;

	// scale
	for (int i = 0; i < 3; i++)
		mat[i][i] = scale[i];

	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	 // set display resolution
	 // check boundary constants
	 // typecast int to unsigned short
	if (xRes >= MAXXRES)
	{
		xres = (unsigned short)MAXXRES;
		std::cout << "x resolution set to max" << std::endl;
	}
	else
		xres = (unsigned short)xRes;

	if (yRes >= MAXYRES)
	{
		yres = (unsigned short)MAXYRES;
		std::cout << "y resolution set to max" << std::endl;
	}
	else
		yres = (unsigned short)yRes;

	// frame buffer's size 
	int sizeFB = 3 * xres * yres * sizeof(char);
	// allocate memory for frame buffer
	framebuffer = new char[sizeFB];

	// pixel buffer's size
	int sizePB = xres * yres;
	// allocate memory for pixel buffer
	pixelbuffer = new GzPixel[sizePB];

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	// initialize Xsp with zeros
	initializeGzMatrix(Xsp);

	// fill the non-zero parts of Xsp
	Xsp[0][0] = (float)xres / 2.0f;
	Xsp[1][1] = (float)-yres / 2.0f;
	Xsp[2][2] = (float)MAXINT;
	Xsp[3][3] = (float)1.0f;
	Xsp[0][3] = (float)xres / 2.0f;
	Xsp[1][3] = (float)yres / 2.0f;

	// initialize matlevel
	matlevel = -1;

	// default camera lookat
	m_camera.lookat[X] = 0.0f;
	m_camera.lookat[Y] = 0.0f;
	m_camera.lookat[Z] = 0.0f;

	// default camera position
	m_camera.position[X] = (float)DEFAULT_IM_X;
	m_camera.position[Y] = (float)DEFAULT_IM_Y;
	m_camera.position[Z] = (float)DEFAULT_IM_Z;

	// default camera worldup
	m_camera.worldup[X] = 0.0f;
	m_camera.worldup[Y] = 1.0f;
	m_camera.worldup[Z] = 0.0f;

	// default camera FOV
	m_camera.FOV = (float)DEFAULT_FOV;

	// initialize number of lights
	numlights = 0;

}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
		// free frame buffer memory
	delete[] framebuffer;

	// free pixel buffer memory
	delete[] pixelbuffer;
}
int GzRender::GzDefault()
{
	return GzDefault(174 * 16, 230 * 16, 234 * 16);
}
int GzRender::GzDefault(int backgroundR, int backgroundG, int backgroundB)
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */

		// RGB values are for a light blue color (8-bit version: 174,230,234)
	
	GzIntensity r = backgroundR;
	GzIntensity g = backgroundG;
	GzIntensity b = backgroundB;

	GzPixel tempPixel = {
		r,		// red
		g,		// green
		b,		// blue
		1,		// alpha
		MAXINT	// z
	};

	for (int i = 0; i < xres; i++)
	{
		for (int j = 0; j < yres; j++)
		{
			pixelbuffer[ARRAY(i, j)] = tempPixel;
			framebuffer[ARRAY(i, j) * 3] = (char)b;
			framebuffer[ARRAY(i, j) * 3 + 1] = (char)g;
			framebuffer[ARRAY(i, j) * 3 + 2] = (char)r;
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/
	int status = 0;

	// ---- compute Xiw ----

	// compute Z
	GzCoord Z_temp;
	for (int i = 0; i < 3; i++)
		Z_temp[i] = m_camera.lookat[i] - m_camera.position[i];
	normalizeVector(Z_temp);

	// compute Y
	GzCoord Y_temp;
	for (int i = 0; i < 3; i++)
		Y_temp[i] = m_camera.worldup[i] - dotProduct(m_camera.worldup, Z_temp) * Z_temp[i];
	normalizeVector(Y_temp);

	// compute X
	GzCoord X_temp;
	crossProduct(Y_temp, Z_temp, X_temp);

	// construct Xiw
	initializeGzMatrix(m_camera.Xiw);
	for (int i = 0; i < 3; i++)
	{
		m_camera.Xiw[X][i] = X_temp[i];
		m_camera.Xiw[Y][i] = Y_temp[i];
		m_camera.Xiw[Z][i] = Z_temp[i];
	}
	m_camera.Xiw[X][3] = -dotProduct(X_temp, m_camera.position);
	m_camera.Xiw[Y][3] = -dotProduct(Y_temp, m_camera.position);
	m_camera.Xiw[Z][3] = -dotProduct(Z_temp, m_camera.position);
	m_camera.Xiw[3][3] = 1.0f;

	// ---- compute Xpi ----

	// initialize Xpi with all zeros
	initializeGzMatrix(m_camera.Xpi);

	// diagonals all ones at first
	for (int i = 0; i < 4; i++)
		m_camera.Xpi[i][i] = 1.0f;

	// place 1/d values inside the relevant positions
	m_camera.Xpi[2][2] = tan(m_camera.FOV * PI / 180.0f / 2.0f);
	m_camera.Xpi[3][2] = tan(m_camera.FOV * PI / 180.0f / 2.0f);

	// initialize Ximage base with Xsp
	status = status | GzPushMatrix(Xsp);

	// push other matrices to Ximage stack
	status = status | GzPushMatrix(m_camera.Xpi);
	status = status | GzPushMatrix(m_camera.Xiw);

	// failure if push matrix gave error (overflow)
	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/

	// overwrite FOV
	m_camera.FOV = camera.FOV;

	// overwrite lookat, position and worldup
	for (int i = 0; i < 3; i++)
	{
		m_camera.lookat[i] = camera.lookat[i];
		m_camera.position[i] = camera.position[i];
		m_camera.worldup[i] = camera.worldup[i];
	}

	// overwrite Xiw and Xpi
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/

	GzMatrix temp_mat;
	initializeGzMatrixIdentity(temp_mat);

	// if push for the first time
	if (matlevel == -1)
	{
		matlevel++;

		// for Ximage
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Ximage[matlevel][i][j] = matrix[i][j];
			}
		}

		// for Xnorm: initial Xsp matrix is identity
		initializeGzMatrixIdentity(Xnorm[matlevel]);
	}
	// if stack is not full
	else if (matlevel < MATLEVELS - 1)
	{
		matlevel++;

		// for Ximage
		matrixMultiplication(Ximage[matlevel - 1], matrix, Ximage[matlevel]);

		// for Xnorm
		if (matlevel == 1) // Xpi
		{
			// Xpi matrix is identity
			initializeGzMatrixIdentity(Xnorm[matlevel]);
		}
		else if (matlevel == 2) // Xiw
		{
			// Initialize the matrix with identity matrix
			initializeGzMatrixIdentity(Xnorm[matlevel]);

			// only take upper left 3x3 part of Xiw as only rotation will be applied to the normals
			// no need to normalize the rows and colums, because they are already normalized
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					Xnorm[matlevel][i][j] = matrix[i][j];
				}
			}
		}
		else
		{
			// extract only the rotation matrix
			extractRotationMatrix(matrix, temp_mat);

			// push the matrix by multiplying with the previous matrix
			matrixMultiplication(Xnorm[matlevel - 1], temp_mat, Xnorm[matlevel]);
		}
	}
	// if stack is full
	else
	{
		return GZ_FAILURE;
	}

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/

	// failure if there is nothing in the stack
	if (matlevel < 0)
	{
		return GZ_FAILURE;
	}
	// decrease the matlevel if stack is not empty
	else
	{
		matlevel--;
		return GZ_SUCCESS;
	}

}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */

	GzPixel tempPixel = {
		r,		// red
		g,		// green
		b,		// blue
		a,		// alpha
		z		// z
	};

	// check for pixels outside of boundaries
	if (i < xres && i >= 0 && j < yres && j >= 0)
		pixelbuffer[ARRAY(i, j)] = tempPixel;

	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity* r, GzIntensity* g, GzIntensity* b, GzIntensity* a, GzDepth* z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */

		// retrieve the GzPixel at pixel (i,j)
	GzPixel tempPixel = pixelbuffer[ARRAY(i, j)];

	// pass the values inside the tempPixel to the r, g, b, a, z pointers
	*r = tempPixel.red;
	*g = tempPixel.green;
	*b = tempPixel.blue;
	*a = tempPixel.alpha;
	*z = tempPixel.z;

	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */

		// header info for ppm output
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);

	// temporary red, green, blue
	GzIntensity r;
	GzIntensity g;
	GzIntensity b;

	// maximum 12-bit value
	int MAX12B = 4095;

	// write each pixel's RGB to the ppm file
	// write row by row so the outer loop is over y values
	for (int i = 0; i < yres; i++)
	{
		for (int j = 0; j < xres; j++)
		{
			// read values into the temporary rgb
			r = pixelbuffer[ARRAY(j, i)].red;
			g = pixelbuffer[ARRAY(j, i)].green;
			b = pixelbuffer[ARRAY(j, i)].blue;

			// clamp values to 0-4095
			if (r > MAX12B)
				r = MAX12B;
			if (r < 0)
				r = 0;
			if (g > MAX12B)
				g = MAX12B;
			if (g < 0)
				g = 0;
			if (b > MAX12B)
				b = MAX12B;
			if (b < 0)
				b = 0;

			// turn 12-bit values to 8-bit chars by shifting
			char* rgb8bit = new char[3];
			rgb8bit[0] = (r >> 4);
			rgb8bit[1] = (g >> 4);
			rgb8bit[2] = (b >> 4);

			// write clamped values to the file
			fwrite(rgb8bit, sizeof(char), 3, outfile);
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer:
		- put the pixels into the frame buffer
		- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
		- NOT red, green, and blue !!!
	*/

	// temporary red, green, blue
	GzIntensity r;
	GzIntensity g;
	GzIntensity b;

	// maximum 12-bit value
	int MAX12B = 4095;

	// write each pixel's RGB to the frame buffer file
	for (int i = 0; i < xres; i++)
	{
		for (int j = 0; j < yres; j++)
		{
			// read values into the temporary rgb
			r = pixelbuffer[ARRAY(i, j)].red;
			g = pixelbuffer[ARRAY(i, j)].green;
			b = pixelbuffer[ARRAY(i, j)].blue;

			// clamp values to 0-4095
			if (r > MAX12B)
				r = MAX12B;
			if (r < 0)
				r = 0;
			if (g > MAX12B)
				g = MAX12B;
			if (g < 0)
				g = 0;
			if (b > MAX12B)
				b = MAX12B;
			if (b < 0)
				b = 0;

			// turn 12-bit values to 8-bit chars by shifting
			char r8bit = (char)(r >> 4);
			char g8bit = (char)(g >> 4);
			char b8bit = (char)(b >> 4);

			// write clamped values to the frame buffer (BGR)
			framebuffer[ARRAY(i, j) * 3] = b8bit;
			framebuffer[ARRAY(i, j) * 3 + 1] = g8bit;
			framebuffer[ARRAY(i, j) * 3 + 2] = r8bit;
		}
	}
	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken* nameList, GzPointer* valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/

	for (int i = 0; i < numAttributes; i++)
	{
		switch (nameList[i])
		{
		case GZ_RGB_COLOR:
		{
			// typecast to GzColor* pointer then access the values inside
			flatcolor[0] = (*(GzColor*)valueList[i])[0];
			flatcolor[1] = (*(GzColor*)valueList[i])[1];
			flatcolor[2] = (*(GzColor*)valueList[i])[2];

			break;
		}
		case GZ_DIRECTIONAL_LIGHT:
		{
			// typecast to GzLight* pointer then access the values inside
			lights[i] = *(GzLight*)valueList[i];

			// increase the number of lights
			numlights++;

			break;
		}
		case GZ_AMBIENT_LIGHT:
		{
			// typecast to GzLight* pointer then access the values inside
			ambientlight = *(GzLight*)valueList[i];

			break;
		}
		case GZ_INTERPOLATE:
		{
			// typecast to int* pointer then access the values inside
			interp_mode = *(int*)valueList[i];

			break;
		}
		case GZ_DIFFUSE_COEFFICIENT:
		{
			// typecast to GzColor* pointer then access the values inside
			Kd[0] = (*(GzColor*)valueList[i])[0];
			Kd[1] = (*(GzColor*)valueList[i])[1];
			Kd[2] = (*(GzColor*)valueList[i])[2];

			break;
		}
		case GZ_AMBIENT_COEFFICIENT:
		{
			// typecast to GzColor* pointer then access the values inside
			Ka[0] = (*(GzColor*)valueList[i])[0];
			Ka[1] = (*(GzColor*)valueList[i])[1];
			Ka[2] = (*(GzColor*)valueList[i])[2];

			break;
		}
		case GZ_SPECULAR_COEFFICIENT:
		{
			// typecast to GzColor* pointer then access the values inside
			Ks[0] = (*(GzColor*)valueList[i])[0];
			Ks[1] = (*(GzColor*)valueList[i])[1];
			Ks[2] = (*(GzColor*)valueList[i])[2];

			break;
		}
		case GZ_DISTRIBUTION_COEFFICIENT:
		{
			// typecast to float* pointer then access the values inside
			spec = *(float*)valueList[i];

			break;
		}
		case GZ_TEXTURE_MAP:
		{
			// typecast to GzTexture
			tex_fun = (GzTexture)valueList[i];

			break;
		}
		case GZ_TEXTURE_NORMAL_MAP
			:
		{
			// typecast to GzTexture
			tex_norm_fun = (GzTexture)valueList[i];

			break;
		}
		case GZ_TEXTURE_DISPLACEMENT_MAP
			:
		{
			// typecast to GzTexture
			tex_displacement_fun = (GzTexture)valueList[i];

			break;
		}
		case GZ_AASHIFTX:
		{
			// typecast to float* pointer then access the values inside
			Xoffset = *(float*)valueList[i];

			break;
		}
		case GZ_AASHIFTY:
		{
			// typecast to float* pointer then access the values inside
			Yoffset = *(float*)valueList[i];

			break;
		}
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int numParts, GzToken* nameList, GzPointer* valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/

	// Create a 4x4 matrix to apply transformations to the triangles
	////Println("A");
	GzMatrix vert_mat = { 0.0f };
	GzMatrix norm_mat = { 0.0f };
	GzTextureIndex uv_mat[3];

	for (int k = 0; k < 3; k++)
	{
		for (int np = 0; np < numParts; np++)
		{
			switch (nameList[np])
			{
			case GZ_NULL_TOKEN:
			{
				// do nothing
			}
			case GZ_POSITION:
			{
				vert_mat[X][k] = ((GzCoord*)valueList[np])[k][X]; // X
				vert_mat[Y][k] = ((GzCoord*)valueList[np])[k][Y]; // Y
				vert_mat[Z][k] = ((GzCoord*)valueList[np])[k][Z]; // Z
				vert_mat[3][k] = 1.0f;
			}
			case GZ_NORMAL:
			{
				norm_mat[X][k] = ((GzCoord*)valueList[np])[k][X]; // X
				norm_mat[Y][k] = ((GzCoord*)valueList[np])[k][Y]; // Y
				norm_mat[Z][k] = ((GzCoord*)valueList[np])[k][Z]; // Z
				norm_mat[3][k] = 1.0f;
			}
			case GZ_TEXTURE_INDEX:
			{
				uv_mat[k][U] = ((GzTextureIndex*)valueList[np])[k][U]; // U
				uv_mat[k][V] = ((GzTextureIndex*)valueList[np])[k][V]; // V
			}
			}
		}

	}
	////Println("B");
	//Println("AAA");
	/*Calculate Tangent vector, Bitangent vector for each triangle mesh, reference: https://learnopengl.com/Advanced-Lighting/Normal-Mapping */
	GzCoord E1, E2, Tangent, Bitangent;
	float deltaUV1[2], deltaUV2[2];

	E1[X] = vert_mat[X][1] - vert_mat[X][0];
	E1[Y] = vert_mat[Y][1] - vert_mat[Y][0];
	E1[Z] = vert_mat[Z][1] - vert_mat[Z][0];
	E2[X] = vert_mat[X][2] - vert_mat[X][0];
	E2[Y] = vert_mat[Y][2] - vert_mat[Y][0];
	E2[Z] = vert_mat[Z][2] - vert_mat[Z][0];

	deltaUV1[U] = uv_mat[1][U] - uv_mat[0][U];
	deltaUV1[V] = uv_mat[1][V] - uv_mat[0][V];
	deltaUV2[U] = uv_mat[2][U] - uv_mat[0][U];
	deltaUV2[V] = uv_mat[2][V] - uv_mat[0][V];
	float f = 1.0f / (deltaUV1[U] * deltaUV2[V] - deltaUV2[U] * deltaUV1[V]);

	Tangent[X] = f * (deltaUV2[V] * E1[X] - deltaUV1[V] * E2[X]);
	Tangent[Y] = f * (deltaUV2[V] * E1[Y] - deltaUV1[V] * E2[Y]);
	Tangent[Z] = f * (deltaUV2[V] * E1[Z] - deltaUV1[V] * E2[Z]);

	Bitangent[X] = f * (-deltaUV2[U] * E1[X] + deltaUV1[U] * E2[X]);
	Bitangent[Y] = f * (-deltaUV2[U] * E1[Y] + deltaUV1[U] * E2[Y]);
	Bitangent[Z] = f * (-deltaUV2[U] * E1[Z] + deltaUV1[U] * E2[Z]);

	//Println("BBB");
	// Define TBN matrix for each triangle mesh to transform from tangent space to model space
	GzMatrix TBN, Inv_TBN;

	TBN[0][0] = Tangent[X]; TBN[1][0] = Tangent[Y]; TBN[2][0] = Tangent[Z];
	TBN[0][1] = Bitangent[X]; TBN[1][1] = Bitangent[Y]; TBN[2][1] = Bitangent[Z];
	TBN[0][2] = norm_mat[X][0]; TBN[1][2] = norm_mat[Y][0]; TBN[2][2] = norm_mat[Z][0];
	TBN[0][3] = TBN[1][3] = TBN[2][3] = TBN[3][0] = TBN[3][1] = TBN[3][2] = 0;
	TBN[3][3] = 1;

	////Println("C");

	//Println("CCC");
	// Inv_TBN: transform from model space to tangent space
	Inverse(TBN, Inv_TBN);

	// Create 4x4 matrices to hold transformed vertex positions and normals
	GzMatrix trans_vert_mat;
	GzMatrix trans_norm_mat;

	// Transform vertices from model space to screen space
	matrixMultiplication(Ximage[matlevel], vert_mat, trans_vert_mat);
	// Transform normals from model space to image space
	matrixMultiplication(Xnorm[matlevel], norm_mat, trans_norm_mat);

	GzMatrix Image2Model;
	// Image2Model: transform from image space to model space
	Inverse(Xnorm[matlevel], Image2Model);

	////Println("D");

	// Create 3D vertices and normals arrays for the triangle
	GzCoord vert[3];
	GzCoord norms[3];

	// Convert 4x4 matrix to 3x3 matrix
	for (int k = 0; k < 3; k++)
	{
		vert[k][X] = trans_vert_mat[X][k] / trans_vert_mat[3][k]; // X
		vert[k][Y] = trans_vert_mat[Y][k] / trans_vert_mat[3][k]; // Y
		vert[k][Z] = trans_vert_mat[Z][k] / trans_vert_mat[3][k]; // Z

		norms[k][X] = trans_norm_mat[X][k] / trans_norm_mat[3][k]; // X
		norms[k][Y] = trans_norm_mat[Y][k] / trans_norm_mat[3][k]; // Y
		norms[k][Z] = trans_norm_mat[Z][k] / trans_norm_mat[3][k]; // Z

		// Apply offset shifts in X and Y dimensions
		vert[k][X] += Xoffset;
		vert[k][Y] += Yoffset;
	}
	//Println("DDD");
	////Println("E");
	// Evaluate lighting equation to calculate color at each vertex
	GzColor vert_intensity[3] = { 0.0f };
	calculateLightingColorsThreeVertices(norms, Ka, Kd, Ks, ambientlight, lights, numlights, spec, vert_intensity);

	// Evaluate lighting equation to calculate color at each vertex to be used in Gouraud shading with texture mapping
	GzColor vert_intensity_tex[3] = { 0.0f };
	GzColor K_one = { 1.0f };
	calculateLightingColorsThreeVertices(norms, K_one, K_one, K_one, ambientlight, lights, numlights, spec, vert_intensity_tex);

	// Create an array of sorted vertices
	GzCoord sort_vert[3];

	// Sort the vertices clockwise V0, V1, V2 and write them into sort_vert
	sortVerticesCW(vert, sort_vert);

	// Compute edge parameters A, B, C such that Ax+By+C=0
	float edge1_param[3]; // Edge 1: Tail:V0 Head:V1
	float edge2_param[3]; // Edge 2: Tail:V1 Head:V2
	float edge3_param[3]; // Edge 3: Tail:V2 Head:V0

	edge1_param[0] = sort_vert[1][Y] - sort_vert[0][Y]; // A = dY
	edge1_param[1] = sort_vert[0][X] - sort_vert[1][X]; // B = -dX
	edge1_param[2] = -edge1_param[1] * sort_vert[0][Y] - edge1_param[0] * sort_vert[0][X]; // C = dX*Y-dY*X

	edge2_param[0] = sort_vert[2][Y] - sort_vert[1][Y]; // A = dY
	edge2_param[1] = sort_vert[1][X] - sort_vert[2][X]; // B = -dX
	edge2_param[2] = -edge2_param[1] * sort_vert[1][Y] - edge2_param[0] * sort_vert[1][X]; // C = dX*Y-dY*X

	edge3_param[0] = sort_vert[0][Y] - sort_vert[2][Y]; // A = dY
	edge3_param[1] = sort_vert[2][X] - sort_vert[0][X]; // B = -dX
	edge3_param[2] = -edge3_param[1] * sort_vert[2][Y] - edge3_param[0] * sort_vert[2][X]; // C = dX*Y-dY*X
	//Println("EEE");
	// Find the pixels corresponding to the bounding box including the triangle
	int box_minX = (int)floor(min(min(vert[0][X], vert[1][X]), vert[2][X]));
	int box_minY = (int)floor(min(min(vert[0][Y], vert[1][Y]), vert[2][Y]));
	int box_maxX = (int)ceil(max(max(vert[0][X], vert[1][X]), vert[2][X]));
	int box_maxY = (int)ceil(max(max(vert[0][Y], vert[1][Y]), vert[2][Y]));
	////Println("F");
	// For each pixel in the bounding box
	for (int pixel_x = max(box_minX, 0); (pixel_x <= box_maxX && pixel_x < xres); pixel_x++)
	{
		for (int pixel_y = max(box_minY, 0); (pixel_y <= box_maxY && pixel_y < yres); pixel_y++)
		{
			// Evaluate the edge line equation at pixel i,j
			float edge1_eval = edge1_param[0] * (float)pixel_x + edge1_param[1] * (float)pixel_y + edge1_param[2];
			float edge2_eval = edge2_param[0] * (float)pixel_x + edge2_param[1] * (float)pixel_y + edge2_param[2];
			float edge3_eval = edge3_param[0] * (float)pixel_x + edge3_param[1] * (float)pixel_y + edge3_param[2];



			//Peter's addition to help with the edge sharing problem
			//if the pixel lies on an edge, "move" it a bit to the right and then evaluate the edge equation again at that new location
			//note this doesn't work with horizontal lines
			if (edge1_eval == 0) edge1_eval = edge1_param[0] * (float)(pixel_x + .1f) + edge1_param[1] * (float)pixel_y + edge1_param[2];
			if (edge2_eval == 0) edge2_eval = edge2_param[0] * (float)(pixel_x + .1f) + edge2_param[1] * (float)pixel_y + edge2_param[2];
			if (edge3_eval == 0) edge3_eval = edge3_param[0] * (float)(pixel_x + .1f) + edge3_param[1] * (float)pixel_y + edge3_param[2];


			// Determine whether signes of all evaluations are consistent
			bool signs_consistent = (edge1_eval > 0.0f && edge2_eval > 0.0f && edge3_eval > 0.0f) ||
				(edge1_eval < 0.0f && edge2_eval < 0.0f && edge3_eval < 0.0f);

			// If the pixel is just at the edge3 and other signs are consistent, put it inside the triangle 
			bool on_edge3 = edge3_eval == 0.0f && edge1_eval * edge2_eval > 0.0f;

			// If the pixel is just at the edge2 and other signs are consistent 
			// and if the triangle is to the right of the pixel, put it inside the right triangle
			// this includes the case where the edge2 is horizontal, in that case pixel is put inside
			// the top triangle
			bool on_edge2 = edge2_eval == 0.0f && edge1_eval * edge3_eval > 0.0f && sort_vert[1][Y] >= sort_vert[2][Y];

			// if the pixel inside the triangle
			//if (signs_consistent || on_edge3 || on_edge2)
			if (signs_consistent)
			{
				float plane_normal[3];

				// Calculate the plane normal parameters (A,B,C)
				calculatePlaneNormal(vert, plane_normal);

				// Solve for D
				float plane_D = -(plane_normal[0] * vert[0][X] + plane_normal[1] * vert[0][Y] + plane_normal[2] * vert[0][Z]);

				// Find z value via interpolation
				float z_interp = -(plane_D + plane_normal[0] * (float)pixel_x + plane_normal[1] * (float)pixel_y) / plane_normal[2];

				// Texture UV Perspective Correction
				float z_prime = 0.0f;
				GzTextureIndex uv_pers[3]; // perspective corrected uv values
				for (int k = 0; k < 3; k++)
				{
					z_prime = vert[k][Z] / ((float)MAXINT - vert[k][Z]);
					uv_pers[k][U] = uv_mat[k][U] / (z_prime + 1.0f);
					uv_pers[k][V] = uv_mat[k][V] / (z_prime + 1.0f);
				}

				// Temporary variables
				GzIntensity r;
				GzIntensity g;
				GzIntensity b;
				GzDepth z_buffer;
				GzIntensity a;

				// Get current z value on the buffer
				GzGet(pixel_x, pixel_y, &r, &g, &b, &a, &z_buffer);

				// If z value < z buffer value update the color
				if (z_interp <= (float)z_buffer && z_interp >= 0)
				{
					// Update the color
					//r = ctoi(flatcolor[0]);
					//g = ctoi(flatcolor[1]);
					//b = ctoi(flatcolor[2]);

					switch (interp_mode)
					{
						// Flat shading
					case GZ_FLAT:
					{
						r = ctoi(vert_intensity[0][0]);
						g = ctoi(vert_intensity[0][1]);
						b = ctoi(vert_intensity[0][2]);

						break;
					}
					// Gouraud shading
					case GZ_COLOR:
					{
						GzCoord vert_color[3] = { 0.0f };
						float plane_normal_color[3];
						float plane_D_color;

						// ------ interpolate for red ------
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								vert_color[i][j] = vert[i][j];
							}
							vert_color[i][2] = (float)(vert_intensity[i][0]);
						}

						// Calculate the plane normal parameters (A,B,C)
						calculatePlaneNormal(vert_color, plane_normal_color);

						// Solve for D
						plane_D_color = -(plane_normal_color[0] * vert_color[0][X] + plane_normal_color[1] * vert_color[0][Y] + plane_normal_color[2] * vert_color[0][Z]);

						// Find red color via interpolation
						float red_color_interp = -(plane_D_color + plane_normal_color[0] * (float)pixel_x + plane_normal_color[1] * (float)pixel_y) / plane_normal_color[2];

						// ------ interpolate for green ------
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								vert_color[i][j] = vert[i][j];
							}
							vert_color[i][2] = (float)vert_intensity[i][1];
						}

						// Calculate the plane normal parameters (A,B,C)
						calculatePlaneNormal(vert_color, plane_normal_color);

						// Solve for D
						plane_D_color = -(plane_normal_color[0] * vert_color[0][X] + plane_normal_color[1] * vert_color[0][Y] + plane_normal_color[2] * vert_color[0][Z]);

						// Find green color via interpolation
						float green_color_interp = -(plane_D_color + plane_normal_color[0] * (float)pixel_x + plane_normal_color[1] * (float)pixel_y) / plane_normal_color[2];

						// ------ interpolate for blue ------
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								vert_color[i][j] = vert[i][j];
							}
							vert_color[i][2] = (float)vert_intensity[i][2];
						}

						// Calculate the plane normal parameters (A,B,C)
						calculatePlaneNormal(vert_color, plane_normal_color);

						// Solve for D
						plane_D_color = -(plane_normal_color[0] * vert_color[0][X] + plane_normal_color[1] * vert_color[0][Y] + plane_normal_color[2] * vert_color[0][Z]);

						// Find blue color via interpolation
						float blue_color_interp = -(plane_D_color + plane_normal_color[0] * (float)pixel_x + plane_normal_color[1] * (float)pixel_y) / plane_normal_color[2];


						// Texture UV Mapping

						// if texture function is not a NULL pointer
						if (tex_fun != NULL)
						{
							GzColor uv_color;
							float z_interp_prime = (float)z_interp / ((float)MAXINT - (float)z_interp);

							// ------ Interpolate for U ------
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 2; j++)
								{
									vert_color[i][j] = vert[i][j];
								}
								vert_color[i][2] = (float)uv_pers[i][U];
							}

							// Calculate the plane normal parameters (A,B,C)
							calculatePlaneNormal(vert_color, plane_normal_color);

							// Solve for D
							plane_D_color = -(plane_normal_color[0] * vert_color[0][X] + plane_normal_color[1] * vert_color[0][Y] + plane_normal_color[2] * vert_color[0][Z]);

							// Find u values via interpolation - perspective space
							float u_pers_interp = -(plane_D_color + plane_normal_color[0] * (float)pixel_x + plane_normal_color[1] * (float)pixel_y) / plane_normal_color[2];
							// Transform interpolated value back to affine space
							float u_interp = u_pers_interp * (z_interp_prime + 1.0f);

							// ------ Interpolate for V ------
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 2; j++)
								{
									vert_color[i][j] = vert[i][j];
								}
								vert_color[i][2] = (float)uv_pers[i][V];
							}

							// Calculate the plane normal parameters (A,B,C)
							calculatePlaneNormal(vert_color, plane_normal_color);

							// Solve for D
							plane_D_color = -(plane_normal_color[0] * vert_color[0][X] + plane_normal_color[1] * vert_color[0][Y] + plane_normal_color[2] * vert_color[0][Z]);

							// Find v values via interpolation - perspective space
							float v_pers_interp = -(plane_D_color + plane_normal_color[0] * (float)pixel_x + plane_normal_color[1] * (float)pixel_y) / plane_normal_color[2];
							// Transform interpolated value back to affine space
							float v_interp = v_pers_interp * (z_interp_prime + 1.0f);

							// Texture look up
							tex_fun(u_interp, v_interp, uv_color);
							if (uv_color[0] == -1 && uv_color[1] == -1 && uv_color[2] == -1)
								continue;
							// Multiply calculated color with uv colors
							red_color_interp *= uv_color[RED];
							green_color_interp *= uv_color[GREEN];
							blue_color_interp *= uv_color[BLUE];
						}

						r = ctoi(red_color_interp);
						g = ctoi(green_color_interp);
						b = ctoi(blue_color_interp);

						break;
					}
					// Phong shading
					case GZ_NORMALS:
					{
						GzCoord vert_phong[3] = { 0.0f };
						float plane_normal_phong[3];
						float plane_D_phong;

						// ------ interpolate for normal X ------
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								vert_phong[i][j] = vert[i][j];
							}
							vert_phong[i][2] = (float)(norms[i][0]);
						}

						// Calculate the plane normal parameters (A,B,C)
						calculatePlaneNormal(vert_phong, plane_normal_phong);

						// Solve for D
						plane_D_phong = -(plane_normal_phong[0] * vert_phong[0][X] + plane_normal_phong[1] * vert_phong[0][Y] + plane_normal_phong[2] * vert_phong[0][Z]);

						// Find normal X via interpolation
						float norm_X_interp = -(plane_D_phong + plane_normal_phong[0] * (float)pixel_x + plane_normal_phong[1] * (float)pixel_y) / plane_normal_phong[2];

						// ------ interpolate for normal Y ------
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								vert_phong[i][j] = vert[i][j];
							}
							vert_phong[i][2] = (float)(norms[i][1]);
						}

						// Calculate the plane normal parameters (A,B,C)
						calculatePlaneNormal(vert_phong, plane_normal_phong);

						// Solve for D
						plane_D_phong = -(plane_normal_phong[0] * vert_phong[0][X] + plane_normal_phong[1] * vert_phong[0][Y] + plane_normal_phong[2] * vert_phong[0][Z]);

						// Find normal Y via interpolation
						float norm_Y_interp = -(plane_D_phong + plane_normal_phong[0] * (float)pixel_x + plane_normal_phong[1] * (float)pixel_y) / plane_normal_phong[2];

						// ------ interpolate for normal Z ------
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								vert_phong[i][j] = vert[i][j];
							}
							vert_phong[i][2] = (float)(norms[i][2]);
						}

						// Calculate the plane normal parameters (A,B,C)
						calculatePlaneNormal(vert_phong, plane_normal_phong);

						// Solve for D
						plane_D_phong = -(plane_normal_phong[0] * vert_phong[0][X] + plane_normal_phong[1] * vert_phong[0][Y] + plane_normal_phong[2] * vert_phong[0][Z]);

						// Find normal Z via interpolation
						float norm_Z_interp = -(plane_D_phong + plane_normal_phong[0] * (float)pixel_x + plane_normal_phong[1] * (float)pixel_y) / plane_normal_phong[2];

						// Texture UV Mapping
						GzCoord norm_map_ModelSpace, norm_map_ImageSpace;
						GzCoord perturbed_norm_map_image;


						////Println("W");
						// if texture function is not a NULL pointer
						if (tex_fun != NULL)
						{
							GzColor uv_color;
							float z_interp_prime = (float)z_interp / ((float)MAXINT - (float)z_interp);

							// ------ Interpolate for U ------
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 2; j++)
								{
									vert_phong[i][j] = vert[i][j];
								}
								vert_phong[i][2] = (float)uv_pers[i][U];
							}

							// Calculate the plane normal parameters (A,B,C)
							calculatePlaneNormal(vert_phong, plane_normal_phong);

							// Solve for D
							plane_D_phong = -(plane_normal_phong[0] * vert_phong[0][X] + plane_normal_phong[1] * vert_phong[0][Y] + plane_normal_phong[2] * vert_phong[0][Z]);

							// Find u values via interpolation - perspective space
							float u_pers_interp = -(plane_D_phong + plane_normal_phong[0] * (float)pixel_x + plane_normal_phong[1] * (float)pixel_y) / plane_normal_phong[2];
							// Transform interpolated value back to affine space
							float u_interp = u_pers_interp * (z_interp_prime + 1.0f);

							// ------ Interpolate for V ------
							for (int i = 0; i < 3; i++)
							{
								for (int j = 0; j < 2; j++)
								{
									vert_phong[i][j] = vert[i][j];
								}
								vert_phong[i][2] = (float)uv_pers[i][V];
							}

							// Calculate the plane normal parameters (A,B,C)
							calculatePlaneNormal(vert_phong, plane_normal_phong);

							// Solve for D
							plane_D_phong = -(plane_normal_phong[0] * vert_phong[0][X] + plane_normal_phong[1] * vert_phong[0][Y] + plane_normal_phong[2] * vert_phong[0][Z]);

							// Find v values via interpolation - perspective space
							float v_pers_interp = -(plane_D_phong + plane_normal_phong[0] * (float)pixel_x + plane_normal_phong[1] * (float)pixel_y) / plane_normal_phong[2];
							// Transform interpolated value back to affine space
							float v_interp = v_pers_interp * (z_interp_prime + 1.0f);



							if (bumpMappingType >= 0)
							{
								// E is the view direction vector in image space
								GzCoord E = { 0.0f, 0.0f, -1.0f }, Model_E, Tangent_E;
								//Println("VVV");
								// Transform E from image space to tangent space for uv coords offset calculation
								// Image Space -> Model Space -> Tangent Space
								MatrixMulVector(Model_E, Image2Model, E, 0);
								normalizeVector(Model_E);
								MatrixMulVector(Tangent_E, Inv_TBN, Model_E, 0);
								normalizeVector(Tangent_E);
								//Println("VEE");
								// import height map
								GzColor height;
								tex_displacement_fun(u_interp, v_interp, height);
								// Play around with the value of scale to check out different bump mapping effects. 
								// A higher scale makes the bumps stand out more, but go too big, and you might notice some distortion in the bump map
								float scale = 2;
								float p[2];

								// Mode 0: Pure Normal Mapping
								// Mode 1: Parallax Mapping
								// Mode 2: Steep Parallax Mapping
								// Mode 3: Bump Mapping (Blinn's paper)
								// Mode 4: Bump Mapping (Sobel filter)
								//int bumpMappingType = 2;
								//Println("WWW");
								if (bumpMappingType == 1)
								{
									// Parallax Mapping
									p[X] = (Tangent_E[X] / Tangent_E[Z]) * height[0] * scale;
									p[Y] = (Tangent_E[Y] / Tangent_E[Z]) * height[0] * scale;
									u_interp -= p[0];
									v_interp -= p[1];
								}
								else if (bumpMappingType == 2)
								{

									// Steep Parallax Mapping
									float deltaTexCoords[2];
									// number of depth layers
									float numLayers = 10;
									// calculate the size of each layer
									float layerDepth = 1.0 / numLayers;
									// depth of current layer
									float currentLayerDepth = 0.0;
									// the amount to shift the texture coordinates per layer (from vector P)
									p[0] = Tangent_E[X] * scale;
									p[1] = Tangent_E[Y] * scale;
									deltaTexCoords[U] = p[0] / numLayers;
									deltaTexCoords[V] = p[1] / numLayers;

									// get initial values
									float currentDepthMapValue = height[0];
									while (currentLayerDepth < currentDepthMapValue)
									{
										// shift texture coordinates along direction of P
										u_interp -= deltaTexCoords[0];
										v_interp -= deltaTexCoords[1];
										tex_displacement_fun(u_interp, v_interp, height);
										// get depthmap value at current texture coordinates
										currentDepthMapValue = height[0];
										// get depth of next layer
										currentLayerDepth += layerDepth;
									}
								}
								else if (bumpMappingType == 3)
								{
									// -- Bump mapping perturbation calculation -- 
									// Reference paper: SIMULATION OF WRINKLED SURFACES, James F. Blinn
									// https://dl.acm.org/doi/pdf/10.1145/965139.507101

									// original interpolated normal vector in image space
									GzCoord norm_interp_image = { norm_X_interp, norm_Y_interp, norm_Z_interp };

									// Transform normal from image space to tangent space for perturbation map calculation
									// Image Space -> Model Space -> Tangent Space
									GzCoord norm_interp_model, norm_interp_tangent;
									MatrixMulVector(norm_interp_model, Image2Model, norm_interp_image, 0);
									normalizeVector(norm_interp_model);
									MatrixMulVector(norm_interp_tangent, Inv_TBN, norm_interp_model, 0);
									normalizeVector(norm_interp_tangent);

									GzColor bump_upluse_v; // tex_bump_fun(u+e,v)
									GzColor bump_uminuse_v; // tex_bump_fun(u-e,v)
									GzColor bump_u_vpluse; // tex_bump_fun(u,v+e)
									GzColor bump_u_vminuse; // tex_bump_fun(u,v-e)
									GzCoord perturb_map; // perturbation map

									float e = 0.0156f; // ~1/64 

									// Read neigboring textures +e, -e amount
									tex_displacement_fun(u_interp + e, v_interp, bump_upluse_v);
									tex_displacement_fun(u_interp - e, v_interp, bump_uminuse_v);
									tex_displacement_fun(u_interp, v_interp + e, bump_u_vpluse);
									tex_displacement_fun(u_interp, v_interp - e, bump_u_vminuse);

									// Bring the range to -1 to 1
									bump_upluse_v[0] = 2.0 * bump_upluse_v[0] - 1.0;
									bump_upluse_v[1] = 2.0 * bump_upluse_v[1] - 1.0;
									bump_upluse_v[2] = 2.0 * bump_upluse_v[2] - 1.0;

									bump_uminuse_v[0] = 2.0 * bump_uminuse_v[0] - 1.0;
									bump_uminuse_v[1] = 2.0 * bump_uminuse_v[1] - 1.0;
									bump_uminuse_v[2] = 2.0 * bump_uminuse_v[2] - 1.0;

									bump_u_vpluse[0] = 2.0 * bump_u_vpluse[0] - 1.0;
									bump_u_vpluse[1] = 2.0 * bump_u_vpluse[1] - 1.0;
									bump_u_vpluse[2] = 2.0 * bump_u_vpluse[2] - 1.0;

									bump_u_vminuse[0] = 2.0 * bump_u_vminuse[0] - 1.0;
									bump_u_vminuse[1] = 2.0 * bump_u_vminuse[1] - 1.0;
									bump_u_vminuse[2] = 2.0 * bump_u_vminuse[2] - 1.0;

									GzColor Fu;
									GzColor Fv;
									Fu[0] = (bump_upluse_v[0] - bump_uminuse_v[0]) / (2 * e);
									Fu[1] = (bump_upluse_v[1] - bump_uminuse_v[1]) / (2 * e);
									Fu[2] = (bump_upluse_v[2] - bump_uminuse_v[2]) / (2 * e);
									Fv[0] = (bump_u_vpluse[0] - bump_u_vminuse[0]) / (2 * e);
									Fv[1] = (bump_u_vpluse[1] - bump_u_vminuse[1]) / (2 * e);
									Fv[2] = (bump_u_vpluse[2] - bump_u_vminuse[2]) / (2 * e);

									GzCoord NcrossPv;
									GzCoord NcrossPu;
									crossProduct(norm_interp_tangent, Bitangent, NcrossPv);
									crossProduct(norm_interp_tangent, Tangent, NcrossPu);

									perturb_map[0] = Fu[0] * NcrossPv[0] - Fv[0] * NcrossPu[0];
									perturb_map[1] = Fu[1] * NcrossPv[1] - Fv[1] * NcrossPu[1];
									perturb_map[2] = Fu[2] * NcrossPv[2] - Fv[2] * NcrossPu[2];

									// this is to change the effect of perturbation on the normal map
									float scalep = 0.1f;

									GzCoord perturbed_norm_map_tangent;
									perturbed_norm_map_tangent[0] = norm_interp_tangent[0] + scalep * perturb_map[0];
									perturbed_norm_map_tangent[1] = norm_interp_tangent[1] + scalep * perturb_map[1];
									perturbed_norm_map_tangent[2] = norm_interp_tangent[2] + scalep * perturb_map[2];
									normalizeVector(perturbed_norm_map_tangent);

									GzCoord perturbed_norm_map_model;
									//Transform new normals from tangent space to model space
									MatrixMulVector(perturbed_norm_map_model, TBN, perturbed_norm_map_tangent, 0);
									normalizeVector(perturbed_norm_map_model);
									//Transform new normals from model space to image space
									MatrixMulVector(perturbed_norm_map_image, Xnorm[matlevel], perturbed_norm_map_model, 0);
									normalizeVector(perturbed_norm_map_image);
								}
								else if(bumpMappingType == 4)
								{
									// creating a normal map from the height map
									// similar to the demo at https://cpetry.github.io/NormalMap-Online/
									
									// you can play with these variables to change the normal map's effect
									// functionalities are similar to the ones in the demo link above
									float level = 7.0f;
									float strength = 2.5f;

									float dz = 1 / strength * pow(2.0f,level) / 255.0f;

									GzColor tlv; // tex_bump_fun(u-e,v+e) 
									GzColor lv; // tex_bump_fun(u-e,v)
									GzColor blv; // tex_bump_fun(u-e,v-e)
									GzColor tv; // tex_bump_fun(u,v+e)
									GzColor bv; // tex_bump_fun(u,v-e) 
									GzColor trv; // tex_bump_fun(u+e,v+e)
									GzColor rv; // tex_bump_fun(u+e,v)
									GzColor brv; // tex_bump_fun(u+e,v-e)
									
									// this value depends on the texture dimensions
									float e = -1.0f / 4096.0f; // ~0.0039 = 1/256

									// Read neigboring textures +e, -e amount
									tex_displacement_fun(u_interp - e, v_interp + e, tlv);
									tex_displacement_fun(u_interp - e, v_interp, lv);
									tex_displacement_fun(u_interp - e, v_interp - e, blv);
									tex_displacement_fun(u_interp, v_interp + e, tv);
									tex_displacement_fun(u_interp, v_interp - e, bv);
									tex_displacement_fun(u_interp + e, v_interp + e, trv);
									tex_displacement_fun(u_interp + e, v_interp, rv);
									tex_displacement_fun(u_interp + e, v_interp - e, brv);

									// Calculate dx and dy with Sobel filter
									float dx = tlv[0] + 2.0f * lv[0] + blv[0] - trv[0] - 2.0f * rv[0] - brv[0];
									float dy = tlv[0] + 2.0f * tv[0] + trv[0] - blv[0] - 2.0f * bv[0] - brv[0];

									// Perturbed normal map
									GzCoord perturbed_norm_map_tangent;
									SetVector3(perturbed_norm_map_tangent, dx, -dy, dz);
									normalizeVector(perturbed_norm_map_tangent);

									//Transform new normals from tangent space to model space
									GzCoord perturbed_norm_map_model;
									MatrixMulVector(perturbed_norm_map_model, TBN, perturbed_norm_map_tangent, 0);
									normalizeVector(perturbed_norm_map_model);
									//Transform new normals from model space to image space
									MatrixMulVector(perturbed_norm_map_image, Xnorm[matlevel], perturbed_norm_map_model, 0);
									normalizeVector(perturbed_norm_map_image);

								}
							}
							//Println("XXX");
							if (!(u_interp > 1.0 || v_interp > 1.0 || u_interp < 0.0 || v_interp < 0.0))
								tex_fun(u_interp, v_interp, uv_color);

							//Println("YYY");
							if (uv_color[0] == -1 && uv_color[1] == -1 && uv_color[2] == -1)
								continue;


							// Ka and Kd equal to texture color
							Ka[RED] = uv_color[RED];
							Ka[GREEN] = uv_color[GREEN];
							Ka[BLUE] = uv_color[BLUE];

							Kd[RED] = uv_color[RED];
							Kd[GREEN] = uv_color[GREEN];
							Kd[BLUE] = uv_color[BLUE];



							if (bumpMappingType >= 0 && bumpMappingType < 3)
							{
								/*Load new normals from texture normal map*/
								GzColor norm_temp, norm_map;
								tex_norm_fun(u_interp, v_interp, norm_temp);


								////Println("X");

								norm_map[0] = 2.0 * norm_temp[0] - 1.0;
								norm_map[1] = 2.0 * norm_temp[1] - 1.0;
								norm_map[2] = 2.0 * norm_temp[2] - 1.0;
								normalizeVector(norm_map);

								////Println("Y");
								/*Transform new normals from tangent space to model space*/
								MatrixMulVector(norm_map_ModelSpace, TBN, norm_map, 0);
								////Println("Z");
								normalizeVector(norm_map_ModelSpace);
								////Println("ZZ");
								/*Transform new normals from model space to image space*/
								MatrixMulVector(norm_map_ImageSpace, Xnorm[matlevel], norm_map_ModelSpace, 0);
								////Println("ZZZ");
								normalizeVector(norm_map_ImageSpace);
							}
							else if (bumpMappingType == 3 || bumpMappingType == 4)
							{
								norm_map_ImageSpace[0] = perturbed_norm_map_image[0];
								norm_map_ImageSpace[1] = perturbed_norm_map_image[1];
								norm_map_ImageSpace[2] = perturbed_norm_map_image[2];
							}
						}

						// calculate color based on interpolated normals
						GzColor norm_intensity = { 0.0f };

						GzCoord norm_interp;

						if(bumpMappingType == -1)
							//norm_interp = { norm_X_interp, norm_Y_interp, norm_Z_interp };
							SetVector3(norm_interp, norm_X_interp, norm_Y_interp, norm_Z_interp);
						else
							//norm_interp = { norm_map_ImageSpace[0], norm_map_ImageSpace[1], norm_map_ImageSpace[2] };
							SetVector3(norm_interp, norm_map_ImageSpace[0], norm_map_ImageSpace[1], norm_map_ImageSpace[2]);

						normalizeVector(norm_interp);
						calculateLightingColor(norm_interp, Ka, Kd, Ks, ambientlight, lights, numlights, spec, norm_intensity);

						r = ctoi(norm_intensity[0]);
						g = ctoi(norm_intensity[1]);
						b = ctoi(norm_intensity[2]);

						break;
					}
					}
					// Pass the values to the buffer
					GzPut(pixel_x, pixel_y, r, g, b, a, (int)z_interp);
				}
			}

		}
	}


	return GZ_SUCCESS;
}

void sortVerticesCW(GzCoord* vert, GzCoord* sort_vert)
{
	// -- Vertex sorting --
	int vert_TBsort_index[3]; // top to bottom sort list according to Y values
	int vert_sort_index[3]; // sort list according to CW rotation starting from the top (1) then the right (2) and then the left (2)

	// sort Y positions first
	if (vert[0][Y] > vert[1][Y])
	{
		if (vert[1][Y] > vert[2][Y])
		{
			vert_TBsort_index[0] = 2;
			vert_TBsort_index[1] = 1;
			vert_TBsort_index[2] = 0;
		}
		else if (vert[2][Y] > vert[1][Y])
		{
			vert_TBsort_index[0] = 1;

			if (vert[0][Y] > vert[2][Y])
			{
				vert_TBsort_index[1] = 2;
				vert_TBsort_index[2] = 0;
			}
			else if (vert[2][Y] > vert[0][Y])
			{
				vert_TBsort_index[1] = 0;
				vert_TBsort_index[2] = 2;
			}
			else //(vert[2][Y] == vert[0][Y])
			{
				// if equal put the one on the right (higher X value) first in the array
				if (vert[2][X] > vert[0][X])
				{
					vert_TBsort_index[1] = 2;
					vert_TBsort_index[2] = 0;
				}
				else
				{
					vert_TBsort_index[1] = 0;
					vert_TBsort_index[2] = 2;
				}
			}
		}
		else //(vert[2][Y] == vert[1][Y])
		{
			// if equal put the one on the right (higher X value) first in the array
			if (vert[2][X] > vert[1][X])
			{
				vert_TBsort_index[0] = 2;
				vert_TBsort_index[1] = 1;
				vert_TBsort_index[2] = 0;
			}
			else
			{
				vert_TBsort_index[0] = 1;
				vert_TBsort_index[1] = 2;
				vert_TBsort_index[2] = 0;
			}
		}
	}
	else if (vert[1][Y] > vert[0][Y])
	{
		if (vert[0][Y] > vert[2][Y])
		{
			vert_TBsort_index[0] = 2;
			vert_TBsort_index[1] = 0;
			vert_TBsort_index[2] = 1;
		}
		else if (vert[2][Y] > vert[0][Y])
		{
			vert_TBsort_index[0] = 0;

			if (vert[1][Y] > vert[2][Y])
			{
				vert_TBsort_index[1] = 2;
				vert_TBsort_index[2] = 1;
			}
			else if (vert[2][Y] > vert[1][Y])
			{
				vert_TBsort_index[1] = 1;
				vert_TBsort_index[2] = 2;
			}
			else //(vert[2][Y] == vert[1][Y])
			{
				// if equal put the one on the right (higher X value) first in the array
				if (vert[2][X] > vert[1][X])
				{
					vert_TBsort_index[1] = 2;
					vert_TBsort_index[2] = 1;
				}
				else
				{
					vert_TBsort_index[1] = 1;
					vert_TBsort_index[2] = 2;
				}
			}
		}
		else //(vert[2][Y] == vert[0][Y])
		{
			// if equal put the one on the right (higher X value) first in the array
			if (vert[2][X] > vert[0][X])
			{
				vert_TBsort_index[0] = 2;
				vert_TBsort_index[1] = 0;
				vert_TBsort_index[2] = 1;
			}
			else
			{
				vert_TBsort_index[0] = 0;
				vert_TBsort_index[1] = 2;
				vert_TBsort_index[2] = 1;
			}
		}
	}
	else //(vert[0][Y] == vert[1][Y])
	{
		if (vert[2][Y] > vert[0][Y])
		{
			vert_TBsort_index[2] = 2;

			if (vert[0][X] > vert[1][X])
			{
				vert_TBsort_index[0] = 0;
				vert_TBsort_index[1] = 1;
			}
			else
			{
				vert_TBsort_index[0] = 1;
				vert_TBsort_index[1] = 0;
			}
		}
		else
		{
			vert_TBsort_index[0] = 2;

			if (vert[0][X] > vert[1][X])
			{
				vert_TBsort_index[1] = 0;
				vert_TBsort_index[2] = 1;
			}
			else
			{
				vert_TBsort_index[1] = 1;
				vert_TBsort_index[2] = 0;
			}
		}
	}

	// Look at slopes to sort L/R

	// Long and short edge slopes
	float long_edge_slope = (vert[vert_TBsort_index[0]][Y] - vert[vert_TBsort_index[2]][Y]) / (vert[vert_TBsort_index[0]][X] - vert[vert_TBsort_index[2]][X]);
	float short_edge_slope = (vert[vert_TBsort_index[0]][Y] - vert[vert_TBsort_index[1]][Y]) / (vert[vert_TBsort_index[0]][X] - vert[vert_TBsort_index[1]][X]);

	// The bigger the slope the righter it is
	if (long_edge_slope > short_edge_slope)
	{
		vert_sort_index[0] = vert_TBsort_index[0];
		vert_sort_index[1] = vert_TBsort_index[2];
		vert_sort_index[2] = vert_TBsort_index[1];
	}
	else //(short_edge_slope >= long_edge_slope)
	{
		vert_sort_index[0] = vert_TBsort_index[0];
		vert_sort_index[1] = vert_TBsort_index[1];
		vert_sort_index[2] = vert_TBsort_index[2];
	}

	for (int i = 0; i < 3; i++)
	{
		sort_vert[i][X] = vert[vert_sort_index[i]][X];
		sort_vert[i][Y] = vert[vert_sort_index[i]][Y];
		sort_vert[i][Z] = vert[vert_sort_index[i]][Z];
	}

}

void calculatePlaneNormal(GzCoord* vert, float* plane_normal)
{
	float u[3];
	float v[3];

	for (int i = 0; i < 3; i++)
	{
		u[i] = vert[1][i] - vert[0][i];
		v[i] = vert[2][i] - vert[0][i];
	}

	plane_normal[0] = u[1] * v[2] - u[2] * v[1];
	plane_normal[1] = u[2] * v[0] - u[0] * v[2];
	plane_normal[2] = u[0] * v[1] - u[1] * v[0];
}

void initializeGzMatrix(GzMatrix Xmat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Xmat[i][j] = 0.0f;
		}
	}
}

void initializeGzMatrixIdentity(GzMatrix Xmat)
{
	initializeGzMatrix(Xmat);
	for (int i = 0; i < 4; i++)
	{
		Xmat[i][i] = 1.0f;
	}
}

void normalizeVector(GzCoord vec)
{
	float vec_norm = sqrt(dotProduct(vec, vec));
	for (int i = 0; i < 3; i++)
		vec[i] = vec[i] / vec_norm;
}

float dotProduct(GzCoord vec1, GzCoord vec2)
{
	float dot_prod = 0.0f;
	for (int i = 0; i < 3; i++)
		dot_prod += vec1[i] * vec2[i];

	return dot_prod;
}

void crossProduct(GzCoord vec1, GzCoord vec2, GzCoord vec3)
{
	// vec3 = vec1 x vec2
	vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	vec3[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	vec3[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void matrixMultiplication(GzMatrix mat1, GzMatrix mat2, GzMatrix mat3)
{
	// mat3 = mat1 * mat2
	// 4-by-4 matrix multiplication
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// temporary variable to keep the sum from the vector vector multiplication
			float temp_sum = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				temp_sum += mat1[i][k] * mat2[k][j];
			}

			mat3[i][j] = temp_sum;
		}
	}
}

void extractRotationMatrix(GzMatrix mat1, GzMatrix mat2)
{
	// mat1 is the original matrix
	// mat2 is the version with only rotation on the upper left 3x3 part

	initializeGzMatrixIdentity(mat2);

	// this only works if rotation/scale/translate matrices are pushed seperately 
	if (!(mat1[0][1] == 0 && mat1[0][2] == 0 && mat1[1][0] == 0 && mat1[1][2] == 0 && mat1[2][1] == 0 && mat1[2][2] == 0))
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				mat2[i][j] = mat1[i][j];
			}
		}
	}
}

void calculateLightingColorsThreeVertices(GzCoord* norms, GzColor Ka, GzColor Kd, GzColor Ks, GzLight amblight, GzLight* lights, int numlights, float spec, GzColor* intensity)
{
	GzCoord R; // Reflection vector
	GzCoord E = { 0.0f, 0.0f, -1.0f }; // Eye vector
	GzCoord N; // Normal vector
	GzCoord L; // Light vector

	// dot products to be used in sign tests and calculations
	float NdL; // N (dot) L
	float NdE; // N (dot) E
	float RdE; // R (dot) E

	float signN = 1.0f; // to be used to flip the normal vector

	// calculate for each vertex
	for (int i = 0; i < 3; i++)
	{
		// normal for the vertex
		N[X] = norms[i][X];
		N[Y] = norms[i][Y];
		N[Z] = norms[i][Z];

		normalizeVector(N);

		NdE = dotProduct(N, E);

		// ambient light
		for (int j = 0; j < 3; j++)
		{
			intensity[i][j] += Ka[j] * amblight.color[j];
		}

		// for each directional light
		for (int k = 0; k < numlights; k++)
		{
			L[X] = lights[k].direction[X];
			L[Y] = lights[k].direction[Y];
			L[Z] = lights[k].direction[Z];

			normalizeVector(L);

			NdL = dotProduct(N, L);

			// calculate if signs of both dot products same, otherwise skip
			if (NdL * NdE > 0.0f)
			{
				if (NdL > 0.0f && NdE > 0.0f) // if both positive
					signN = 1.0f;
				else if (NdL < 0.0f && NdE < 0.0f) // if both negative
					signN = -1.0f;

				R[X] = 2 * NdL * N[X] - L[X];
				R[Y] = 2 * NdL * N[Y] - L[Y];
				R[Z] = 2 * NdL * N[Z] - L[Z];

				normalizeVector(R);

				RdE = dotProduct(R, E);

				// clamp RdE to maintain [0, 1] bounded range
				if (RdE > 1.0f)
					RdE = 1.0f;
				else if (RdE < 0.0f)
					RdE = 0.0f;

				for (int j = 0; j < 3; j++)
				{
					intensity[i][j] += Ks[j] * lights[k].color[j] * pow(RdE, spec) + Kd[j] * lights[k].color[j] * NdL * signN;
				}
			}
		}

		// clamp intensity to [0, 1]
		clampColor(intensity[i]);
	}
}

void calculateLightingColor(GzCoord norms, GzColor Ka, GzColor Kd, GzColor Ks, GzLight amblight, GzLight* lights, int numlights, float spec, GzColor intensity)
{
	GzCoord R; // Reflection vector
	GzCoord E = { 0.0f, 0.0f, -1.0f }; // Eye vector
	GzCoord N; // Normal vector
	GzCoord L; // Light vector

	// dot products to be used in sign tests and calculations
	float NdL; // N (dot) L
	float NdE; // N (dot) E
	float RdE; // R (dot) E

	float signN = 1.0f; // to be used to flip the normal vector

	// normal for the vertex
	N[X] = norms[X];
	N[Y] = norms[Y];
	N[Z] = norms[Z];

	normalizeVector(N);

	NdE = dotProduct(N, E);

	// ambient light
	for (int j = 0; j < 3; j++)
	{
		intensity[j] += Ka[j] * amblight.color[j];
	}

	// for each directional light
	for (int k = 0; k < numlights; k++)
	{
		L[X] = lights[k].direction[X];
		L[Y] = lights[k].direction[Y];
		L[Z] = lights[k].direction[Z];

		normalizeVector(L);

		NdL = dotProduct(N, L);

		// calculate if signs of both dot products same, otherwise skip
		if (NdL * NdE > 0.0f)
		{
			if (NdL > 0.0f && NdE > 0.0f) // if both positive
				signN = 1.0f;
			else if (NdL < 0.0f && NdE < 0.0f) // if both negative
				signN = -1.0f;

			R[X] = 2 * NdL * N[X] - L[X];
			R[Y] = 2 * NdL * N[Y] - L[Y];
			R[Z] = 2 * NdL * N[Z] - L[Z];

			normalizeVector(R);

			RdE = dotProduct(R, E);

			// clamp RdE to maintain [0, 1] bounded range
			if (RdE > 1.0f)
				RdE = 1.0f;
			else if (RdE < 0.0f)
				RdE = 0.0f;

			for (int j = 0; j < 3; j++)
			{
				intensity[j] += Ks[j] * lights[k].color[j] * pow(RdE, spec) + Kd[j] * lights[k].color[j] * NdL * signN;
			}
		}
	}

	// clamp intensity to [0, 1]
	clampColor(intensity);
}

void clampColor(GzColor col)
{
	for (int i = 0; i < 3; i++)
	{
		if (col[i] > 1.0f)
			col[i] = 1.0f;
		else if (col[i] < 0.0f)
			col[i] = 0.0f;
	}
}


void GzRender::SetBumpMappingType(int b)
{
	bumpMappingType = b;
}