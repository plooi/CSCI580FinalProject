#pragma once


#include "Billboard.h"




class BillboardModel
{
public:
	Billboard* billboards;
	int numBillboards;

	void CreateBillboardModelFromRegularModel(float* vertices, float* uvs, float* normals);




};

