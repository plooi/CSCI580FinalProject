#include "stdafx.h"
#include "Utils.h"
#include <stdio.h>
#include "stdlib.h"



void PrintingInit()
{
	FILE* f = fopen("./output.txt", "w");
	fputs("", f);
	fclose(f);
}


void Println(char* str)
{
	FILE* f = fopen("./output.txt", "a+");
	fputs(str, f);
	fputs("\n", f);
	fclose(f);
}



void Print(char* str)
{
	FILE* f = fopen("./output.txt", "a+");
	fputs(str, f);
	fclose(f);
}

void PrintFloat(float fl)
{
	FILE* f = fopen("./output.txt", "a+");
	char buffer[500];
	sprintf(buffer, "%f", fl);
	fputs(buffer, f);
	fclose(f);
}
void PrintMatrix(GzMatrix mat)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			PrintFloat(mat[r][c]);
			Print(" ");
		}
		Print("\n");
	}
}

void PrintCoord(GzCoord coord)
{
	Print("(");
	for (int i = 0; i < 3; i++)
	{
		PrintFloat(coord[i]);
		Print(" ");
	}
	Print(")");
}

void PrintLight(GzLight* light)
{
	Print("Light: ");
	PrintCoord(light->color);
	Print(" : ");
	PrintCoord(light->direction);
	Print("\n");
}

void PrintInt(int i)
{
	FILE* f = fopen("./output.txt", "a+");
	char buffer[500];
	sprintf(buffer, "%d", i);
	fputs(buffer, f);
	fclose(f);
}

void SetVector3(float* vector, float a, float b, float c)
{
	vector[0] = a;
	vector[1] = b;
	vector[2] = c;
}
