/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image = NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color) {
	unsigned char pixel[3];
	unsigned char dummy;
	char foo[8];
	int i, j;
	FILE *fd;

	if (reset) {          /* open and load texture file */
		fd = fopen("texture", "rb");
		if (fd == NULL) {
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs + 1)*(ys + 1));
		if (image == NULL) {
			fprintf(stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	/* bounds-test u,v to make sure nothing will overflow image array bounds */
	u = (u < 0) ? 0 : ((u > 1) ? 1 : u);
	v = (v < 0) ? 0 : ((v > 1) ? 1 : v);

	/* determine texture cell corner values and perform bilinear interpolation */
	float px, py, s, t;
	px = u * (xs - 1);
	py = v * (ys - 1);
	s = px - floor(px);
	t = py - floor(py);

	/* set color to interpolated GzColor value and return */
	GzColor outcolor[4];
	for (int i = 0; i < 3; i++) {
		outcolor[0][i] = image[xs * (int)floor(py) + (int)floor(px)][i];
		outcolor[1][i] = image[xs * (int)floor(py) + (int)ceil(px)][i];
		outcolor[2][i] = image[xs * (int)ceil(py) + (int)ceil(px)][i];
		outcolor[3][i] = image[xs * (int)ceil(py) + (int)floor(px)][i];
	}
	for (int i = 0; i < 3; i++) {
		color[i] = (s * t * outcolor[2][i]) + ((1 - s) * t * outcolor[3][i]) + (s * (1 - t) * outcolor[1][i]) + ((1 - s) * (1 - t) * outcolor[0][i]);
	}

	return GZ_SUCCESS;
}


/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color) {

	/*Checkerboard*/
	/*GzColor outcolor1 = { 1.0, 1.0, 1.0 };
	GzColor outcolor2 = { 0.3, 0.3, 0.3 };

	bool x = (u < 0.25 || (u >= 0.5 && u < 0.75)) ? true : false;
	bool y = (v < 0.25 || (v >= 0.5 && v < 0.75)) ? true : false;

	for (int i = 0; i < 3; i++) {
		color[i] = (x && y || !x && !y) ? outcolor1[i] : outcolor2[i];
	}*/

	/*Julia Set*/
	float xr = 3 * (u - 0.5), xi = 2 * (v - 0.5);
	float cr = -0.70176, ci = -0.3842;
	//float cr = -0.835, ci = -0.2321;
	//float cr = -0.8, ci = -0.156;
	int iteration = 0, N = 150;
	while (xr * xr + xi * xi <= 4.0 && iteration < N) {
		float r_temp = xr * xr - xi * xi;
		xi = 2 * xr * xi + ci;
		xr = r_temp + cr;
		iteration++;
	}
	float lengthX = sqrt(xr * xr + xi * xi);
	GzColor outcolor1 = { 1.0, 0.6, 0.2 };
	GzColor outcolor2 = { 0.5, 0.7, 0.5 };
	if (iteration == N) {
		for (int i = 0; i < 3; i++) {
			color[i] = outcolor1[i];
		}
	}
	else {
		float range = float(iteration) / lengthX;
		color[0] = range * outcolor2[0] + (1 - range) * outcolor1[0];
		color[1] = range * outcolor2[1] + (1 - range) * outcolor1[1];
		color[2] = range * outcolor2[2] + (1 - range) * outcolor1[2];
	}

	/*Mandelbrot*/
	/*float scale = 2.0;
	float xcenter = 0.6, ycenter = 0.0;
	float cr = 1.3333 * (u - 0.5) * scale - xcenter;
	float ci = (v - 0.5) * scale - ycenter;
	float xr = cr, xi = ci;
	int iteration = 0, N = 250;

	while (xr * xr + xi * xi <= 4.0 && iteration < N) {
		float r_temp = xr * xr - xi * xi;
		xi = 2 * xr * xi + ci;
		xr = r_temp + cr;
		iteration++;
	}

	float output = float(iteration) / 100;
	color[0] = output;
	color[1] = output;
	color[2] = output;*/


	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture() {
	if (image != NULL)
		free(image);
	return GZ_SUCCESS;
}

