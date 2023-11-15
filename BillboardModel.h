#pragma once


#include "Billboard.h"




class BillboardModel
{
public:
	Billboard* billboards;
	int numBillboards;

	int CreateBillboardModelFromRegularModel(int numVertices, float* vertices, float* uvs, float* normals, GzTextureStruct* texture, int billboardWidth, int billboardHeight);

};

