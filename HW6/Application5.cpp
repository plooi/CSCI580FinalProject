// Application5.cpp: implementation of the Application5 class.
//
//////////////////////////////////////////////////////////////////////

/*
 * application test code for homework assignment #5
*/

#include "stdafx.h"
#include "CS580HW.h"
#include "Application5.h"
#include "Gz.h"
#include "rend.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INFILE  "ppot.asc"
#define OUTFILE "output.ppm"


extern int tex_fun(float u, float v, GzColor color); /* image texture function */
extern int ptex_fun(float u, float v, GzColor color); /* procedural texture function */
extern int GzFreeTexture();

void shade(GzCoord norm, GzCoord color);

float AAFilter[AAKERNEL_SIZE][3] 		/* X-shift, Y-shift, weight */
{
	-0.52, 0.38, 0.128, 		0.41, 0.56, 0.119,		0.27, 0.08, 0.294,
	-0.17, -0.29, 0.249,		0.58, -0.55, 0.104,		-0.31, -0.71, 0.106
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Application5::Application5()
{

}

Application5::~Application5()
{
	Clean();
}

int Application5::Initialize()
{
	GzCamera	camera;  
	int		    xRes, yRes;	/* display parameters */ 

	GzToken		nameListShader[9]; 	    /* shader attribute names */
	GzPointer   valueListShader[9];		/* shader attribute pointers */
	GzToken     nameListLights[10];		/* light info */
	GzPointer   valueListLights[10];
	GzToken		nameListOffset[2];		/* Offset attribute names */
	GzPointer	valueListOffset[2];		/* Offset attribute pointers */
	int			shaderType, interpStyle;
	float		specpower;
	int		status; 
 
	status = 0; 

	/* 
	 * Allocate memory for user input
	 */
	m_pUserInput = new GzInput;

	/* 
	 * initialize the display and the renderer 
	 */ 
 	m_nWidth = 256;		// frame buffer and display width
	m_nHeight = 256;    // frame buffer and display height

	// loop over each renderer for antialiasing
	for (int i = 0; i <= AAKERNEL_SIZE; i++)
	{
		m_pRender[i] = new GzRender(m_nWidth, m_nHeight);
		m_pRender[i]->GzDefault();

		/* Translation matrix */
		GzMatrix	scale =
		{
			3.25,	0.0,	0.0,	0.0,
			0.0,	3.25,	0.0,	-3.25,
			0.0,	0.0,	3.25,	3.5,
			0.0,	0.0,	0.0,	1.0
		};

		GzMatrix	rotateX =
		{
			1.0,	0.0,	0.0,	0.0,
			0.0,	.7071,	.7071,	0.0,
			0.0,	-.7071,	.7071,	0.0,
			0.0,	0.0,	0.0,	1.0
		};

		GzMatrix	rotateY =
		{
			.866,	0.0,	-0.5,	0.0,
			0.0,	1.0,	0.0,	0.0,
			0.5,	0.0,	.866,	0.0,
			0.0,	0.0,	0.0,	1.0
		};

#if 1 	/* set up app-defined camera if desired, else use camera defaults */
		camera.position[X] = -3;
		camera.position[Y] = -25;
		camera.position[Z] = -4;

		camera.lookat[X] = 7.8;
		camera.lookat[Y] = 0.7;
		camera.lookat[Z] = 6.5;

		camera.worldup[X] = -0.2;
		camera.worldup[Y] = 1.0;
		camera.worldup[Z] = 0.0;

		camera.FOV = 63.7;              /* degrees *              /* degrees */

		status |= m_pRender[i]->GzPutCamera(camera);
#endif 

		/* Start Renderer */
		status |= m_pRender[i]->GzBeginRender();

		/* Light */
		GzLight	light1 = { {-0.7071, 0.7071, 0}, {0.5, 0.5, 0.9} };
		GzLight	light2 = { {0, -0.7071, -0.7071}, {0.9, 0.2, 0.3} };
		GzLight	light3 = { {0.7071, 0.0, -0.7071}, {0.2, 0.7, 0.3} };
		GzLight	ambientlight = { {0, 0, 0}, {0.3, 0.3, 0.3} };

		/* Material property */
		GzColor specularCoefficient = { 0.3, 0.3, 0.3 };
		GzColor ambientCoefficient = { 0.1, 0.1, 0.1 };
		GzColor diffuseCoefficient = { 0.7, 0.7, 0.7 };

		/*
		  renderer is ready for frame --- define lights and shader at start of frame
		*/

		/*
		 * Tokens associated with light parameters
		 */
		nameListLights[0] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[0] = (GzPointer)&light1;
		nameListLights[1] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[1] = (GzPointer)&light2;
		nameListLights[2] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[2] = (GzPointer)&light3;
		status |= m_pRender[i]->GzPutAttribute(3, nameListLights, valueListLights);

		nameListLights[0] = GZ_AMBIENT_LIGHT;
		valueListLights[0] = (GzPointer)&ambientlight;
		status |= m_pRender[i]->GzPutAttribute(1, nameListLights, valueListLights);

		/*
		 * Tokens associated with shading
		 */
		nameListShader[0] = GZ_DIFFUSE_COEFFICIENT;
		valueListShader[0] = (GzPointer)diffuseCoefficient;

		/*
		* Select either GZ_COLOR or GZ_NORMALS as interpolation mode
		*/
		nameListShader[1] = GZ_INTERPOLATE;
		//interpStyle = GZ_COLOR;         /* Gouraud shading */
		interpStyle = GZ_NORMALS;         /* Phong shading */
		valueListShader[1] = (GzPointer)&interpStyle;

		nameListShader[2] = GZ_AMBIENT_COEFFICIENT;
		valueListShader[2] = (GzPointer)ambientCoefficient;
		nameListShader[3] = GZ_SPECULAR_COEFFICIENT;
		valueListShader[3] = (GzPointer)specularCoefficient;
		nameListShader[4] = GZ_DISTRIBUTION_COEFFICIENT;
		specpower = 32;
		valueListShader[4] = (GzPointer)&specpower;

		nameListShader[5] = GZ_TEXTURE_MAP;
		#if 0   /* set up null texture function or valid pointer */
			valueListShader[5] = (GzPointer)0;
		#else
			valueListShader[5] = (GzPointer)(tex_fun);	/* or use ptex_fun */
		#endif
		status |= m_pRender[i]->GzPutAttribute(6, nameListShader, valueListShader);

		// shift amounts
		float shiftX = 0.0f;
		float shiftY = 0.0f;

		if (i < AAKERNEL_SIZE)
		{
			shiftX = AAFilter[i][0];
			shiftY = AAFilter[i][1];
		}

		nameListOffset[0] = GZ_AASHIFTX;
		valueListOffset[0] = (GzPointer) & shiftX;
		nameListOffset[1] = GZ_AASHIFTY;
		valueListOffset[1] = (GzPointer) & shiftY;

		status |= m_pRender[i]->GzPutAttribute(2, nameListOffset, valueListOffset);

		status |= m_pRender[i]->GzPushMatrix(scale);
		status |= m_pRender[i]->GzPushMatrix(rotateY);
		status |= m_pRender[i]->GzPushMatrix(rotateX);
	}

	m_pFrameBuffer = m_pRender[AAKERNEL_SIZE]->framebuffer;

	if (status) exit(GZ_FAILURE); 

	if (status) 
		return(GZ_FAILURE); 
	else 
		return(GZ_SUCCESS); 
}

int Application5::Render() 
{
	GzToken		nameListTriangle[3]; 	/* vertex attribute names */
	GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */
	GzCoord		vertexList[3];	/* vertex position coordinates */ 
	GzCoord		normalList[3];	/* vertex normals */ 
	GzTextureIndex  	uvList[3];		/* vertex texture map indices */ 
	char		dummy[256]; 
	int			status; 


	/* Initialize Display for each rendered */
	for (int i = 0; i <= AAKERNEL_SIZE; i++)
	{
		status |= m_pRender[i]->GzDefault();  /* init for new frame */

		/*
		* Tokens associated with triangle vertex values
		*/
		nameListTriangle[0] = GZ_POSITION;
		nameListTriangle[1] = GZ_NORMAL;
		nameListTriangle[2] = GZ_TEXTURE_INDEX;

		// I/O File open
		FILE* infile;
		if ((infile = fopen(INFILE, "r")) == NULL)
		{
			AfxMessageBox("The input file was not opened\n");
			return GZ_FAILURE;
		}

		/*
		* Walk through the list of triangles, set color
		* and render each triangle
		*/
		while (fscanf(infile, "%s", dummy) == 1) { 	/* read in tri word */
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

			/*
			 * Set the value pointers to the first vertex of the
			 * triangle, then feed it to the renderer
			 * NOTE: this sequence matches the nameList token sequence
			 */
			valueListTriangle[0] = (GzPointer)vertexList;
			valueListTriangle[1] = (GzPointer)normalList;
			valueListTriangle[2] = (GzPointer)uvList;
			m_pRender[i]->GzPutTriangle(3, nameListTriangle, valueListTriangle);
		}

		/*
		* Close file
		*/

		if (fclose(infile))
			AfxMessageBox(_T("The input file was not closed\n"));
	}

	// temporary red, color, blue variables to save filter applied values
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;

	// temporary filter weight
	float w = 0.0f;

	// loop over all pixels to apply antialiasing filter
	for (int p = 0; p < m_nWidth * m_nHeight; p++)
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;

		// loop over all renderers
		for (int i = 0; i < AAKERNEL_SIZE; i++)
		{
			w = AAFilter[i][2]; // filter weight
			r += (float)m_pRender[i]->pixelbuffer[p].red * (float)w; // weighted sum
			g += (float)m_pRender[i]->pixelbuffer[p].green * (float)w; // weighted sum
			b += (float)m_pRender[i]->pixelbuffer[p].blue * (float)w; // weighted sum
		}

		// update the pixel buffer of the last renderer with filtered color values 
		m_pRender[AAKERNEL_SIZE]->pixelbuffer[p].red = (short)r;
		m_pRender[AAKERNEL_SIZE]->pixelbuffer[p].green = (short)g;
		m_pRender[AAKERNEL_SIZE]->pixelbuffer[p].blue = (short)b;

	}

	FILE* outfile;
	if ((outfile = fopen(OUTFILE, "wb")) == NULL)
	{
		AfxMessageBox("The output file was not opened\n");
		return GZ_FAILURE;
	}

	m_pRender[AAKERNEL_SIZE]->GzFlushDisplay2File(outfile); 	/* write out or update display to file*/
	m_pRender[AAKERNEL_SIZE]->GzFlushDisplay2FrameBuffer();	// write out or update display to frame buffer

	if (fclose(outfile))
		AfxMessageBox(_T("The output file was not closed\n"));

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}

int Application5::Clean()
{
	/* 
	 * Clean up and exit 
	 */ 
	int	status = 0; 

	// clear all renderers
	for (int i = 0; i <= AAKERNEL_SIZE; i++)
		delete(m_pRender[i]);

	status |= GzFreeTexture();
	
	if (status) 
		return(GZ_FAILURE); 
	else 
		return(GZ_SUCCESS);
}



