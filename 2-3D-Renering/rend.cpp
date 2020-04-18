#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
//#include <fstream>

class Edge {
	/*Start, End, Current, Slope X, Slope Z*/
public:
	GzCoord start;
	GzCoord end;
	GzCoord current;
	float slopeX() { return (end[0] - start[0]) / (end[1] - start[1]); }
	float slopeZ() { return (end[2] - start[2]) / (end[1] - start[1]); }
};

class Span {
	/*Start(X, Z), End(X, Z), Current(X, Z), Slope Z*/
public:
	float start[2];
	float end[2];
	float current[2];
	float slopeZ() { return (end[1] - start[1]) / (end[0] - start[0]); }
};

/***********************************************/
/* HW1 methods: copy here the methods from HW1 */

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
			pixelbuffer[ARRAY(i, j)].z = INT_MAX;
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


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) {
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/

	//std::ofstream log;
	//log.open("log.txt");
	for (int i = 0; i < numAttributes; i++) {
		if (nameList[i] == GZ_RGB_COLOR) {
			GzColor *color = (GzColor*)valueList[i];
			float r, g, b;

			/*The square brackets operator takes precedence over the pointer dereference operator.*/
			//log << (*color)[0] << " " << (*color)[1] << " " << (*color)[2] << "\n";
			//log << *color[0] << " " << *color[1] << " " << *color[2] << "\n";
			r = (((*color)[0] > 1) ? 1 : (((*color)[0] < 0) ? 0 : (*color)[0]));
			g = (((*color)[1] > 1) ? 1 : (((*color)[1] < 0) ? 0 : (*color)[1]));
			b = (((*color)[2] > 1) ? 1 : (((*color)[2] < 0) ? 0 : (*color)[2]));
			flatcolor[0] = r;
			flatcolor[1] = g;
			flatcolor[2] = b;
		}
	}
	//log.close();
	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList) 
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_POSITION) {
			GzCoord *positions = (GzCoord*)valueList[i];
			GzCoord vertex[3];

			/*std::ofstream log;
			log.open("log.txt");*/

			for (int j = 0; j < 3; j++) {
				vertex[j][0] = positions[j][0];
				vertex[j][1] = positions[j][1];
				vertex[j][2] = positions[j][2];
			}

			/*Step 1. Sort vertices by Y*/
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2 - j; k++) {
					if (vertex[k][1] > vertex[k + 1][1]) {
						GzCoord temp = { vertex[k][0], vertex[k][1], vertex[k][2] };
						vertex[k][0] = vertex[k + 1][0];
						vertex[k][1] = vertex[k + 1][1];
						vertex[k][2] = vertex[k + 1][2];
						vertex[k + 1][0] = temp[0];
						vertex[k + 1][1] = temp[1];
						vertex[k + 1][2] = temp[2];
					}
				}
			}
			/*for (int j = 0; j < 3; j++) {
				log << vertex[j][0] << " " << vertex[j][1] << " " << vertex[j][2] << "\n";
			}
			log << "\n";*/

			int order[6] = { 0, 1, 1, 2, 0, 2 };
			int index = -1;
			/*Step 2. Setup edges DDAs*/
			Edge DDA[3];
			for (int j = 0; j < 3; j++) {
				int k = order[++index];
				DDA[j].start[0] = vertex[k][0];
				DDA[j].start[1] = vertex[k][1];
				DDA[j].start[2] = vertex[k][2];
				DDA[j].current[0] = vertex[k][0];
				DDA[j].current[1] = vertex[k][1];
				DDA[j].current[2] = vertex[k][2];

				k = order[++index];
				DDA[j].end[0] = vertex[k][0];
				DDA[j].end[1] = vertex[k][1];
				DDA[j].end[2] = vertex[k][2];

				/*log << "DDA" << j << "\n";
				log << "start: " << DDA[j].start[0] << " " << DDA[j].start[1] << " " << DDA[j].start[2] << "\n";
				log << "end: " << DDA[j].end[0] << " " << DDA[j].end[1] << " " << DDA[j].end[2] << "\n";
				log << "current: " << DDA[j].current[0] << " " << DDA[j].current[1] << " " << DDA[j].current[2] << "\n\n";*/
			}


			///*Step 3. Sort edges by L or R*/
			enum triangle { LEFT, RIGHT };
			triangle type = LEFT;
			if (DDA[0].slopeX() < DDA[2].slopeX()) { type = LEFT; }
			if (DDA[0].slopeX() > DDA[2].slopeX()) { type = RIGHT; }
			/*log << "slopeX: " << DDA[0].slopeX() << " " << DDA[2].slopeX() << "\n";
			log << "type: " << type << "\n\n";*/
			/*       1                1
			//LEFT    / |       RIGHT   | \
			//	   /  |				  |  \
			//	  /   |				  |   \
			//	  2   3               3    2
			//*/

			///*Step 4. Advance (1-2) and (1-3) DDA current positions to top y-scan line (ceiling)*/
			float deltaY = ceil(vertex[0][1]) - vertex[0][1];
			DDA[0].current[0] += (DDA[0].slopeX() * deltaY);
			DDA[0].current[1] += deltaY;
			DDA[0].current[2] += (DDA[0].slopeZ() * deltaY);
			DDA[2].current[0] += (DDA[2].slopeX() * deltaY);
			DDA[2].current[1] += deltaY;
			DDA[2].current[2] += (DDA[2].slopeZ() * deltaY);

			/*log << "deltaY: " << deltaY << "\n";
			log << "DDA0\n";
			log << "current: " << DDA[0].current[0] << " " << DDA[0].current[1] << " " << DDA[0].current[2] << "\n\n";
			log << "DDA2\n";
			log << "current: " << DDA[2].current[0] << " " << DDA[2].current[1] << " " << DDA[2].current[2] << "\n\n";*/

			GzIntensity r, g, b, a;
			GzDepth z;
			Span span;
			while (DDA[0].current[1] < DDA[0].end[1]) {

				/*Step 5. Setup span DDA on successive lines based on edge DDA position values.
				Set span DDA current and end positions to right and left edge values*/
				span.start[0] = (type == LEFT) ? DDA[0].current[0] : DDA[2].current[0];
				span.start[1] = (type == LEFT) ? DDA[0].current[2] : DDA[2].current[2];
				span.end[0] = (type == LEFT) ? DDA[2].current[0] : DDA[0].current[0];
				span.end[1] = (type == LEFT) ? DDA[2].current[2] : DDA[0].current[2];
				span.current[0] = span.start[0];
				span.current[1] = span.start[1];

				/*log << "Span\n";
				log << "start: " << span.start[0] << " " << span.start[1] << "\n";
				log << "end: " << span.end[0] << " " << span.end[1] << "\n";
				log << "current: " << span.current[0] << " " << span.current[1] << "\n\n";*/


				/*Step 6. Advance span current position to left-most covered pixel (ceiling)*/
				float deltaX = ceil(span.start[0]) - span.start[0];
				span.current[0] += deltaX;
				span.current[1] += (deltaX * span.slopeZ());

				/*log << "deltaX: " << deltaX << "\n";
				log << "Span\n";
				log << "current: " << span.current[0] << " " << span.current[1] << "\n\n";*/

				/*Step 7. Interpolate span position and parameters (Z) until current position > end*/
				while ((type == LEFT && span.current[0] < DDA[2].current[0]) || (type == RIGHT && span.current[0] < DDA[0].current[0])) {
					int currentX = (int)span.current[0];
					int currentY = (int)DDA[0].current[1];
					GzGet(currentX, currentY, &r, &g, &b, &a, &z);
					/*log << "Pixel\n";
					log << currentX << " " << currentY << "\n\n";
					log << "Color\n";
					log << r << " " << g << " " << b << " " << a << " " << z << " " << "\n\n";*/
					/*Step 8. Test interpolated-Z against FB-Z for each pixel - lowest Z wins. Write color value into FB pixel  (default or computed color)*/
					if (span.current[1] < z) {
						r = ctoi(flatcolor[0]);
						g = ctoi(flatcolor[1]);
						b = ctoi(flatcolor[2]);
						GzPut(currentX, currentY, r, g, b, 1, span.current[1]);
						/*log << "Color\n";
						log << r << " " << g << " " << b << " 1 " << span.current[1] << " " << "\n\n";*/
					}

					/*Move one pixel over*/
					span.current[0] += 1;
					span.current[1] += span.slopeZ();
					/*log << "Span\n";
					log << "current: " << span.current[0] << " " << span.current[1] << "\n\n";*/
				}

				///*Move down on both DDAs*/
				DDA[0].current[0] += DDA[0].slopeX();
				DDA[0].current[1] += 1;
				DDA[0].current[2] += DDA[0].slopeZ();
				DDA[2].current[0] += DDA[2].slopeX();
				DDA[2].current[1] += 1;
				DDA[2].current[2] += DDA[2].slopeZ();
				/*log << "DDA0\n";
				log << "current: " << DDA[0].current[0] << " " << DDA[0].current[1] << " " << DDA[0].current[2] << "\n\n";
				log << "DDA2\n";
				log << "current: " << DDA[2].current[0] << " " << DDA[2].current[1] << " " << DDA[2].current[2] << "\n\n";*/
			}

			///*Step 9. Switch from 1-2 edge to 2-3 edge when current 1-2 edge position > Y(2)*/
			///*Step 10. Continue spans until vert 3 is passed: current 1-3 edge position > Y(3)*/
			deltaY = ceil(vertex[1][1]) - vertex[1][1]; 
			/*log << "DDA2\n";
			log << "current: " << DDA[2].current[0] << " " << DDA[2].current[1] << " " << DDA[2].current[2] << "\n\n";
			log << "DDA1\n";
			log << "current: " << DDA[1].current[0] << " " << DDA[1].current[1] << " " << DDA[1].current[2] << "\n\n";*/
			DDA[1].current[0] += (DDA[1].slopeX() * deltaY);
			DDA[1].current[1] += deltaY;
			DDA[1].current[2] += (DDA[1].slopeZ() * deltaY);

			/*log << "deltaY: " << deltaY << "\n";
			log << "DDA1\n";
			log << "current: " << DDA[1].current[0] << " " << DDA[1].current[1] << " " << DDA[1].current[2] << "\n\n";*/

			while (DDA[1].current[1] < DDA[1].end[1]) {

				/*Step 5. Setup span DDA on successive lines based on edge DDA position values.
				Set span DDA current and end positions to right and left edge values*/
				span.start[0] = (type == LEFT) ? DDA[1].current[0] : DDA[2].current[0];
				span.start[1] = (type == LEFT) ? DDA[1].current[2] : DDA[2].current[2];
				span.end[0] = (type == LEFT) ? DDA[2].current[0] : DDA[1].current[0];
				span.end[1] = (type == LEFT) ? DDA[2].current[2] : DDA[1].current[2];
				span.current[0] = span.start[0];
				span.current[1] = span.start[1];
				/*log << "Span\n";
				log << "start: " << span.start[0] << " " << span.start[1] << "\n";
				log << "end: " << span.end[0] << " " << span.end[1] << "\n";
				log << "current: " << span.current[0] << " " << span.current[1] << "\n\n";*/

				/*Step 6. Advance span current position to left-most covered pixel (ceiling)*/
				float deltaX = ceil(span.start[0]) - span.start[0];
				span.current[0] += deltaX;
				span.current[1] += (deltaX * span.slopeZ());
				/*log << "deltaX: " << deltaX << "\n";
				log << "Span\n";
				log << "current: " << span.current[0] << " " << span.current[1] << "\n\n";*/

				/*Step 7. Interpolate span position and parameters (Z) until current position > end*/
				while ((type == LEFT && span.current[0] < DDA[2].current[0]) || (type == RIGHT && span.current[0] < DDA[1].current[0])) {
					int currentX = (int)span.current[0];
					int currentY = (int)DDA[1].current[1];
					GzGet(currentX, currentY, &r, &g, &b, &a, &z);
					/*log << "Pixel\n";
					log << currentX << " " << currentY << "\n\n";
					log << "Color\n";
					log << r << " " << g << " " << b << " " << a << " " << z << " " << "\n\n";*/

					/*Step 8. Test interpolated-Z against FB-Z for each pixel - lowest Z wins. Write color value into FB pixel  (default or computed color)*/
					if (span.current[1] < z) {
						r = ctoi(flatcolor[0]);
						g = ctoi(flatcolor[1]);
						b = ctoi(flatcolor[2]);
						GzPut(currentX, currentY, r, g, b, 1, span.current[1]); 
						/*log << "Color\n";
						log << r << " " << g << " " << b << " 1 " << span.current[1] << " " << "\n\n";*/
					}

					/*Move one pixel over*/
					span.current[0] += 1;
					span.current[1] += span.slopeZ();
				}

				/*Move down on both DDAs*/
				DDA[1].current[0] += DDA[1].slopeX();
				DDA[1].current[1] += 1;
				DDA[1].current[2] += DDA[1].slopeZ();
				DDA[2].current[0] += DDA[2].slopeX();
				DDA[2].current[1] += 1;
				DDA[2].current[2] += DDA[2].slopeZ();
				/*log << "DDA1\n";
				log << "current: " << DDA[1].current[0] << " " << DDA[1].current[1] << " " << DDA[1].current[2] << "\n\n";
				log << "DDA2\n";
				log << "current: " << DDA[2].current[0] << " " << DDA[2].current[1] << " " << DDA[2].current[2] << "\n\n";*/
			}

			//log.close();

		}
	}

	return GZ_SUCCESS;
}