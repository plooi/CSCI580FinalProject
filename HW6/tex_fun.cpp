/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
    unsigned char		pixel[3];
    unsigned char     dummy;
    char  		foo[8];
    int   		i, j;
    FILE* fd;

    if (reset) {          /* open and load texture file */
        fd = fopen("texture", "rb");
        if (fd == NULL) {
            fprintf(stderr, "texture file not found\n");
            exit(-1);
        }
        fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
        image = (GzColor*)malloc(sizeof(GzColor) * (xs + 1) * (ys + 1));
        if (image == NULL) {
            fprintf(stderr, "malloc for texture image failed\n");
            exit(-1);
        }

        for (i = 0; i < xs * ys; i++) {	/* create array of GzColor values */
            fread(pixel, sizeof(pixel), 1, fd);
            image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
            image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
            image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
        }

        reset = 0;          /* init is done */
        fclose(fd);
    }

    /* bounds-test u,v to make sure nothing will overflow image array bounds */
    /* determine texture cell corner values and perform bilinear interpolation */
    /* set color to interpolated GzColor value and return */


    // boundary test
    // if value outside clamp to zero or one
    if (u > 1.0f)
        u = 1.0f;
    else if (u < 0.0f)
        u = 0.0f;
    if (v > 1.0f)
        v = 1.0f;
    else if (v < 0.0f)
        v = 0.0f;

    float u_scaled = (float)u * (xs - 1);
    float v_scaled = (float)v * (ys - 1);

    int u_up = (int)ceil(u_scaled);
    int u_low = (int)floor(u_scaled);
    int v_up = (int)ceil(v_scaled);
    int v_low = (int)floor(v_scaled);

    float s = u_scaled - u_low;
    float t = v_scaled - v_low;

    // Neigbor at top left
    GzColor A;
    int A_ind = v_low * xs + u_low;
    A[RED] = image[A_ind][RED];
    A[GREEN] = image[A_ind][GREEN];
    A[BLUE] = image[A_ind][BLUE];

    // Neigbor at top right
    GzColor B;
    int B_ind = v_low * xs + u_up;
    B[RED] = image[B_ind][RED];
    B[GREEN] = image[B_ind][GREEN];
    B[BLUE] = image[B_ind][BLUE];

    // Neigbor at bottom right
    GzColor C;
    int C_ind = v_up * xs + u_up;
    C[RED] = image[C_ind][RED];
    C[GREEN] = image[C_ind][GREEN];
    C[BLUE] = image[C_ind][BLUE];

    // Neigbor at bottom left
    GzColor D;
    int D_ind = v_up * xs + u_up;
    D[RED] = image[D_ind][RED];
    D[GREEN] = image[D_ind][GREEN];
    D[BLUE] = image[D_ind][BLUE];

    // Bilinear interpolation
    color[RED] = s*t*C[RED] + (1.0f-s)*t*D[RED] + s*(1.0f-t)*B[RED] + (1.0f -s)*(1.0f -t)*A[RED];
    color[GREEN] = s*t*C[GREEN] + (1.0f-s)*t*D[GREEN] + s*(1.0f-t)*B[GREEN] + (1.0f -s)*(1.0f -t)*A[GREEN];
    color[BLUE] = s*t*C[BLUE] + (1.0f-s)*t*D[BLUE] + s*(1.0f-t)*B[BLUE] + (1.0f -s)*(1.0f -t)*A[BLUE];

	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
    // boundary test
    // if value outside clamp to zero or one
    if (u > 1.0f)
        u = 1.0f;
    else if (u < 0.0f)
        u = 0.0f;
    if (v > 1.0f)
        v = 1.0f;
    else if (v < 0.0f)
        v = 0.0f;

    float r = sin(v*30.0f + cos(u*20.0f)) + cos(3*pow(u,2)+pow(v,2));
    float g = sin((u-0.04f)*30.0f + cos((v-0.01f)*20.0f)) + cos(pow(u,2)+2*pow(v,2));
    float b = sin((u+0.04f)*30.0f + cos((v+0.01f)*20.0f)) + cos(pow(u,2)+pow(v,2));

    if (r > 1.0f)
        r = 1.0f;
    else if (r < 0.0f)
        r = 0.0f;
    if (g > 1.0f)
        g = 1.0f;
    else if (g < 0.0f)
        g = 0.0f;
    if (b > 1.0f)
        b = 1.0f;
    else if (b < 0.0f)
        b = 0.0f;

    // Color
    color[RED] = r;
    color[GREEN] = g;
    color[BLUE] = b;
	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}


