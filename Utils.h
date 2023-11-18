#pragma once
#include "gz.h"
#ifndef UTILS
#define UTILS


void PrintingInit();

void Println(char* str);


void Print(char* str);

void PrintFloat(float fl);
void PrintMatrix(GzMatrix mat);

void PrintCoord(GzCoord coord);
void PrintLight(GzLight* light);
void PrintInt(int i);


void SetVector3(float* vector, float a, float b, float c);


#endif
