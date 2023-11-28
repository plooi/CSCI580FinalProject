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
#include "Utils.h"
#include "Billboard.h"
#include "BumpBillboard.h"
#include <chrono>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


float angle = 180;



#define INFILE  "rect.asc"
#define OUTFILE "output.ppm"
#define PI 3.14159

extern int tex_fun(float u, float v, GzColor color); /* image texture function */
extern int tex_norm_fun(float u, float v, GzColor color);
extern int tex_displacement_fun(float u, float v, GzColor height);
extern int ptex_fun(float u, float v, GzColor color); /* procedural texture function */
extern int GzFreeTexture();

void shade(GzCoord norm, GzCoord color);

GzTextureStruct* theeModelTexture;
int theeBillboardTextureFunction(float u, float v, GzColor color)
{
	theeModelTexture->Sample(u, v, color);
	return GZ_SUCCESS;
}



#define NUM_BILLBOARDS 12
BumpBillboard billboards[NUM_BILLBOARDS];

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

	PrintingInit();



	//load the teapot model and store all that information in an object
	Model model;
	model.Load("justthepot.asc");
	//Println("B");

	//put the model parameters into buffers
	int numTriangles = model.numTriangles;
	float* vertices = model.vertices;
	float* normals = model.normals;
	float* uvs = model.uvs;



	//load the USC texture into this 'texture' object
	GzTextureStruct texture;
	texture.InitFromPPMFile("texture");

	//Println("C");
	//what do we want the resolution of the billboard image to be?
	int billboardWidthPixels = 315;//165;
	int billboardHeightPixels = 315;// 165;

	//here we set up a bunch of rendering parameters so that the teapot can be rendered
	//onto the billboard
	GzLight ambientLight;
	SetVector3(ambientLight.direction, 0, 0, 0);
	SetVector3(ambientLight.color, .5, .5, .5);
	GzLight lights[1];
	SetVector3(lights[0].color, 0, 0, 0);
	SetVector3(lights[0].direction, -1, 0, -1);
	int numLights = 1;
	GzColor ambientCoefficient;
	SetVector3(ambientCoefficient, 1, 1, 1);
	GzColor specularCoefficient;
	SetVector3(specularCoefficient, 1, 1, 1);
	GzColor diffuseCoefficient;
	SetVector3(diffuseCoefficient, 1, 1, 1);

	

	


	for (int i = 0; i < 12; i++)
	{
		billboards[i].CreateFromModel(numTriangles,
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
			32,
			0,//model vertical rotation before being rendered onto the billboard (in radians)
			-i * 3.14 / 6//model horizontal rotation before being rendered onto the billboard (in radians)
			
		);

		billboards[i].SetDimensions(10, 10);//the billboard will be scaled up to be this amount units in world space when rendered
		billboards[i].SetRotation(i*3.14 / 6, 0);//the billboard itself will be rotated by this amount

		//the billboard will be rendered near the origin of the scene
		GzCoord loc;
		SetVector3(loc, 0, 0, 0);
		billboards[i].SetLocation(loc);
	}
	







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
	m_nWidth = 512;//256;		// frame buffer and display width
	m_nHeight = 512;// 256;    // frame buffer and display height

	// loop over each renderer for antialiasing
	for (int i = 0; i <= AAKERNEL_SIZE; i++)
	{
		m_pRender[i] = new GzRender(m_nWidth, m_nHeight);
		m_pRender[i]->GzDefault();

		/* Translation matrix */




#if 1 	/* set up app-defined camera if desired, else use camera defaults */
		//CAMERA SETUP

		float CAMERA_DEGREE = angle;

		camera.position[X] = 10*cos(CAMERA_DEGREE / 360 * 2 * 3.14159);
		camera.position[Y] = 0;
		camera.position[Z] = 10 * sin(CAMERA_DEGREE / 360 * 2 * 3.14159);

		camera.lookat[X] = 0;
		camera.lookat[Y] = 0;
		camera.lookat[Z] = 0;

		camera.worldup[X] = 0;
		camera.worldup[Y] = 1.0;
		camera.worldup[Z] = 0.0;

		camera.FOV = 63.7;              /* degrees *              /* degrees */

		status |= m_pRender[i]->GzPutCamera(camera);
#endif 

		/* Start Renderer */
		status |= m_pRender[i]->GzBeginRender();

		/* Light */
		/*GzLight	light1 = {{-0.7071, 0.7071, 0}, {0.5, 0.5, 0.9}};
		GzLight	light2 = { {0, -0.7071, -0.7071}, {0.9, 0.2, 0.3} };
		GzLight	light3 = { {0.7071, 0.0, -0.7071}, {0.2, 0.7, 0.3} };
		GzLight	ambientlight = { {0, 0, 0}, {0.3, 0.3, 0.3} };*/
		GzLight	light1 = { {-0.7071, 0.7071, 0}, {1, 1, 1} };
		GzLight	light2 = { {0, -0.7071, -0.7071}, {1, 1, 1} };
		GzLight	light3 = { {0.7071, 0.0, -0.7071}, {1, 1, 1} };
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
		valueListShader[5] = (GzPointer)(theeBillboardTextureFunction);	/* or use ptex_fun */
#endif
		nameListShader[6] = GZ_TEXTURE_NORMAL_MAP;
#if 0   /* set up null texture function or valid pointer */
		valueListShader[6] = (GzPointer)0;
#else
		valueListShader[6] = (GzPointer)(tex_norm_fun);	/* or use ptex_fun */
#endif
		nameListShader[7] = GZ_TEXTURE_DISPLACEMENT_MAP;
#if 0   /* set up null texture function or valid pointer */
		valueListShader[7] = (GzPointer)0;
#else
		valueListShader[7] = (GzPointer)(tex_displacement_fun);	/* or use ptex_fun */
#endif
		status |= m_pRender[i]->GzPutAttribute(8, nameListShader, valueListShader);



		// shift amounts
		float shiftX = 0.0f;
		float shiftY = 0.0f;

		if (i < AAKERNEL_SIZE)
		{
			shiftX = AAFilter[i][0];
			shiftY = AAFilter[i][1];
		}

		nameListOffset[0] = GZ_AASHIFTX;
		valueListOffset[0] = (GzPointer)&shiftX;
		nameListOffset[1] = GZ_AASHIFTY;
		valueListOffset[1] = (GzPointer)&shiftY;

		status |= m_pRender[i]->GzPutAttribute(2, nameListOffset, valueListOffset);


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
	//float angle;
	
	int testOriginalRendering = 0;



	GzMatrix	scale =
	{
		3.25,	0.0,	0.0,	10.0,
		0.0,	3.25,	0.0,	10,
		0.0,	0.0,	3.25,	50,
		0.0,	0.0,	0.0,	1.0
	};

	int degree = 250;
	GzMatrix rotateX =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(degree * PI / 180), -sin(degree * PI / 180), 0.0,
		0.0, sin(degree * PI / 180), cos(degree * PI / 180), 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	degree = 60;
	GzMatrix rotateY =
	{
		cos(degree * PI / 180), 0.0, sin(degree * PI / 180), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(degree * PI / 180), 0.0, cos(degree * PI / 180), 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	degree = 30;
	GzMatrix rotateZ =
	{
		cos(degree * PI / 180), -sin(degree * PI / 180), 0.0, 0.0,
		sin(degree * PI / 180), cos(degree * PI / 180), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};








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
			

		//status |= m_pRender[i]->GzDefault();  /* init for new frame */
		//m_pRender[i]->GzPutCamera(camForThisFrame);
		//m_pRender[i]->GzBeginRender();


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




		float* camPosition = m_pRender[0]->m_camera.position;
		float angleToCenter;
		if (camPosition[0] > 0)
		{
			angleToCenter = atan(camPosition[2] / camPosition[0]);
		}
		else if (camPosition[0] < 0)

			angleToCenter = atan(camPosition[2] / camPosition[0]) + 3.14f;
		else
			if (camPosition[2] > 0)
				angleToCenter = 3.14 / 2;
			else
				angleToCenter = -3.14 / 2;
		angleToCenter += 3.14 / 2;
		int billboardNum = (angleToCenter + 3.14 / 12) / (3.14 / 6);
		while (billboardNum >= 12)
			billboardNum -= 12;



		///if (!testOriginalRendering)
		billboards[billboardNum].BillboardDraw(m_pRender[i]);


		Println("HIELLLLLOOO");


		//billboards[0].BillboardDraw(m_pRender[i]);
		Print("Billboard num");
		PrintInt(billboardNum);
		Println(" ");
		Print("angle to center");
		PrintFloat(angleToCenter / 6.28 * 360);
		Println(" ");

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


	char fileName[100];
	strcpy(fileName, "output");
	char buff[100];
	//itoa((int)a, buff, 100);
	sprintf(buff, "%d", angle);
	strcat(fileName, buff);
	strcat(fileName, ".ppm");
	Println(buff);
	Println(fileName);
	FILE* outfile;
	if ((outfile = fopen(fileName, "wb")) == NULL)
	{
		AfxMessageBox("The output file was not opened\n");
		return GZ_FAILURE;
	}

	m_pRender[AAKERNEL_SIZE]->GzFlushDisplay2File(outfile); 	/* write out or update display to file*/
	m_pRender[AAKERNEL_SIZE]->GzFlushDisplay2FrameBuffer();	// write out or update display to frame buffer

	if (fclose(outfile))
		AfxMessageBox(_T("The output file was not closed\n"));














	/*
	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
	*/
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



