#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"


GzRender::GzRender(int xRes, int yRes) {
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	xres = ((xRes < 0) ? 0 : ((xRes > MAXXRES) ? MAXXRES : xRes));
	yres = ((yRes < 0) ? 0 : ((yRes > MAXYRES) ? MAXYRES : yRes));
	framebuffer = new char[3 * xres * yres];
	pixelbuffer = new GzPixel[xres * yres];
}

GzRender::~GzRender() {
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;
}

int GzRender::GzDefault() {
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			pixelbuffer[ARRAY(i, j)].red = 3000;
			pixelbuffer[ARRAY(i, j)].green = 3000;
			pixelbuffer[ARRAY(i, j)].blue = 2000;
			pixelbuffer[ARRAY(i, j)].alpha = 1;
			pixelbuffer[ARRAY(i, j)].z = 1;
		}
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z) {
	/* HW1.4 write pixel values into the buffer */
	r = ((r > 4095) ? 4095 : ((r < 0) ? 0 : r));
	g = ((g > 4095) ? 4095 : ((g < 0) ? 0 : g));
	b = ((b > 4095) ? 4095 : ((b < 0) ? 0 : b));
	if ((i >= 0 && i < xres) && (j >= 0 && j < yres)) {
		pixelbuffer[ARRAY(i, j)].red = r;
		pixelbuffer[ARRAY(i, j)].green = g;
		pixelbuffer[ARRAY(i, j)].blue = b;
		pixelbuffer[ARRAY(i, j)].alpha = a;
		pixelbuffer[ARRAY(i, j)].z = z;
	}
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z) {
	/* HW1.5 retrieve a pixel information from the pixel buffer */
	if ((i >= 0 && i < xres) && (j >= 0 && j < yres)) {
		*r = pixelbuffer[ARRAY(i, j)].red;
		*g = pixelbuffer[ARRAY(i, j)].green;
		*b = pixelbuffer[ARRAY(i, j)].blue;
		*a = pixelbuffer[ARRAY(i, j)].alpha;
		*z = pixelbuffer[ARRAY(i, j)].z;
	}
	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile) {
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	char r, g, b;
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			r = pixelbuffer[ARRAY(i, j)].red >> 4;
			g = pixelbuffer[ARRAY(i, j)].green >> 4;
			b = pixelbuffer[ARRAY(i, j)].blue >> 4;
			fprintf(outfile, "%c%c%c", r, g, b);
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer() {
	/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
	*/
	char r, g, b;
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			r = pixelbuffer[ARRAY(i, j)].red >> 4;
			g = pixelbuffer[ARRAY(i, j)].green >> 4;
			b = pixelbuffer[ARRAY(i, j)].blue >> 4;
			framebuffer[ARRAY(i, j) * 3] = b;
			framebuffer[ARRAY(i, j) * 3 + 1] = g;
			framebuffer[ARRAY(i, j) * 3 + 2] = r;
		}
	}
	return GZ_SUCCESS;
}