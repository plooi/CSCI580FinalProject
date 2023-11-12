/*
 * Gz.h - include file for the cs580 rendering library
 */

/*
 * universal constants
 */
#define GZ_SUCCESS      0
#define GZ_FAILURE      1

/*
 * name list tokens
 */
#define GZ_NULL_TOKEN			0	/* triangle vert attributes */
#define GZ_POSITION				1
#define GZ_NORMAL				2
#define GZ_TEXTURE_INDEX		3


#define	GZ_AASHIFTX				44	/* antialiasing screen offset */
#define	GZ_AASHIFTY				45	/* antialiasing screen offset */

/* renderer-state default pixel color */
#define GZ_RGB_COLOR            99	

#define GZ_INTERPOLATE			95	/* define interpolation mode */

#define GZ_DIRECTIONAL_LIGHT	79	/* directional light */
#define GZ_AMBIENT_LIGHT		78	/* ambient light type */

#define GZ_AMBIENT_COEFFICIENT		1001	/* Ka material property */
#define GZ_DIFFUSE_COEFFICIENT		1002	/* Kd material property */
#define GZ_SPECULAR_COEFFICIENT		1003	/* Ks material property */
#define GZ_DISTRIBUTION_COEFFICIENT	1004	/* specular power of material */

#define	GZ_TEXTURE_MAP			1010	/* pointer to texture routine */

/*
 * flags fields for value list attributes
 */

/* select interpolation mode of the shader (only one) */
#define GZ_FLAT			0	/* flat shading with GZ_RBG_COLOR */
#define	GZ_COLOR		1	/* interpolate vertex color */
#define	GZ_NORMALS		2	/* interpolate normals */

typedef int     GzToken;
typedef void    *GzPointer;
typedef float   GzColor[3];
typedef short   GzIntensity;	/* 0 - 4095 in lower 12-bits */
typedef float   GzCoord[3];
typedef float   GzTextureIndex[2];
typedef float   GzMatrix[4][4];
typedef int	GzDepth;		/* z is signed for clipping */

typedef	int	(*GzTexture)(float u, float v, GzColor color);	/* pointer to texture sampling method */
/* u,v parameters [0,1] are defined tex_fun(float u, float v, GzColor color) */





/*
 * Gz camera definition
 */
#ifndef GZCAMERA
#define GZCAMERA
typedef struct  GzCamera
{
  GzMatrix			Xiw;  		/* xform from world to image space */
  GzMatrix			Xpi;  		/* perspective projection xform */
  GzCoord			position;  	/* position of image plane origin */
  GzCoord			lookat;         /* position of look-at-point */
  GzCoord			worldup;  /* world up-vector (almost screen up) */
  float				FOV;            /* horizontal field of view */
} GzCamera;
#endif

#ifndef GZLIGHT
#define GZLIGHT
typedef struct  GzLight
{
   GzCoord		direction; 	/* vector from surface to light */
   GzColor		color;		/* light color intensity */
} GzLight;
#endif

#ifndef GZINPUT
#define GZINPUT
typedef struct  GzInput
{
   GzCoord          rotation;       /* object rotation */
   GzCoord			translation;	/* object translation */
   GzCoord			scale;			/* object scaling */
   GzCamera			camera;			/* camera */
} GzInput;
#endif

#define RED     0        /* array indicies for color vector */
#define GREEN   1
#define BLUE    2

#define X       0        /* array indicies for position vector*/
#define Y       1
#define Z       2

#define U       0        /* array indicies for texture coords */
#define V       1


#ifndef GZ_PIXEL
typedef	struct {
  GzIntensity    red;	
  GzIntensity    green;
  GzIntensity    blue;
  GzIntensity    alpha;
  GzDepth	 z;
} GzPixel;
#define GZ_PIXEL
#endif;

#define	MAXXRES	1024	/* put some bounds on size in case of error */
#define	MAXYRES	1024







#ifndef GZTEXTURESTRUCT
#define GZTEXTURESTRUCT

typedef struct GzTextureStruct
{
	GzColor* pixels;
	int textureWidth;
	int textureHeight;

	int floor(float f){return (int)f;}
	int ceil(float f){if (f == ((int)f))return f; return ((int)f) + 1;}

	void Sample(float u, float v, GzColor dest)
	{
		if (u > 1) u = 1;if (u < 0) u = 0;if (v > 1) v = 1;if (v < 0) v = 0;

		float xPixel = u * textureWidth;
		float yPixel = v * textureHeight;
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
};

#endif