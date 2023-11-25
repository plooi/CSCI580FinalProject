#pragma once
#include "Utils.h"
#include "Billboard.h"


class BumpBillboard: public Billboard
{
public:
	GzTextureStruct* normalMap;
	GzTextureStruct* displacementMap;



	virtual BumpBillboard* CreateFromModel(
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
	);

	virtual void BillboardDraw(GzRender* renderer);
	
	
};

