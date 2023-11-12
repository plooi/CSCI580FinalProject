#include "stdafx.h"
#include "Billboard.h"



int floor(float f)
{
	return (int)f;
}
int ceil(float f)
{
	if (f == ((int)f))return f;
	return ((int)f) + 1;
}

void Billboard::SampleTexture(int u, int v, GzColor dest)
{
	if (u > 1) u = 1;
	if (u < 0) u = 0;
	if (v > 1) v = 1;
	if (v < 0) v = 0;

	float xPixel = u * textureWidth;
	float yPixel = v * textureHeight;

	if (xPixel > textureWidth - 1) xPixel = textureWidth - 1;
	if (yPixel > textureHeight - 1) yPixel = textureHeight - 1;

	GzColor* upperLeft;
	GzColor* upperRight;
	GzColor* lowerLeft;
	GzColor* lowerRight;

	upperLeft = texture + floor(yPixel) * textureWidth + floor(xPixel);
	upperRight = texture + floor(yPixel) * textureWidth + ceil(xPixel);
	lowerLeft = texture + ceil(yPixel) * textureWidth + floor(xPixel);
	lowerRight = texture + ceil(yPixel) * textureWidth + ceil(xPixel);


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