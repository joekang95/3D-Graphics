/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

class Edge {
	/*Start, End, Current, Slope X, Slope Z*/
public:
	GzCoord start;
	GzCoord end;
	GzCoord current;
	GzColor color_start;
	GzColor color_end;
	GzColor color_current;
	float slopeX() { return (end[0] - start[0]) / (end[1] - start[1]); }
	float slopeZ() { return (end[2] - start[2]) / (end[1] - start[1]); }
	float stepColor(int i) { return (color_end[i] - color_start[i]) / (end[1] - start[1]); }
};

class Span {
	/*Start(X, Z), End(X, Z), Current(X, Z), Slope Z*/
public:
	float start[2];
	float end[2];
	float current[2];
	GzColor color_start;
	GzColor color_end;
	GzColor color_current;
	float slopeZ() { return (end[1] - start[1]) / (end[0] - start[0]); }
	float stepColor(int i) { return (color_end[i] - color_start[i]) / (end[0] - start[0]); }
};

#define PI (float) 3.14159265358979323846

int GzRender::GzRotXMat(float degree, GzMatrix mat) {
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/
	float radian = (float)degree * (PI / 180);
	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = cos(radian);
	mat[1][2] = -sin(radian);
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = sin(radian);
	mat[2][2] = cos(radian);
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat) {
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	float radian = (float)degree * (PI / 180);
	mat[0][0] = cos(radian);
	mat[0][1] = 0;
	mat[0][2] = sin(radian);
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = -sin(radian);
	mat[2][1] = 0;
	mat[2][2] = cos(radian);
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat) {
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	float radian = (float)degree * (PI / 180);
	mat[0][0] = cos(radian);
	mat[0][1] = -sin(radian);
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = sin(radian);
	mat[1][1] = cos(radian);
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat) {
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/
	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = translate[0];

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = translate[1];

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = translate[2];

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat) {
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/
	mat[0][0] = scale[0];
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = scale[1];
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = scale[2];
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}

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

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	matlevel = 0;
	m_camera.FOV = DEFAULT_FOV;
	m_camera.lookat[0] = 0.0;
	m_camera.lookat[1] = 0.0;
	m_camera.lookat[2] = 0.0;
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	m_camera.worldup[0] = 0.0;
	m_camera.worldup[1] = 1.0;
	m_camera.worldup[2] = 0.0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xiw[i][j] = 0;
			m_camera.Xpi[i][j] = 0;
			Xsp[i][j] = 0;
		}
	}

	interp_mode = GZ_RGB_COLOR;
	numlights = 0;
	GzColor ambient = DEFAULT_AMBIENT;
	Ka[0] = ambient[0];
	Ka[1] = ambient[1];
	Ka[2] = ambient[2];

	GzColor diffuse = DEFAULT_DIFFUSE;
	Kd[0] = diffuse[0];
	Kd[1] = diffuse[1];
	Kd[2] = diffuse[2];

	GzColor specular = DEFAULT_SPECULAR;
	Ks[0] = specular[0];
	Ks[1] = specular[1];
	Ks[2] = specular[2];
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

int GzRender::GzBeginRender() {
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/

	//framebuffer = new char[3 * xres * yres];
	GzDefault();

	float radian = (float)(m_camera.FOV * (PI / 180));
	float d = 1 / (tan(radian / 2));
	/*Xsp*/
	Xsp[0][0] = xres / 2.0;
	Xsp[0][1] = 0;
	Xsp[0][2] = 0;
	Xsp[0][3] = xres / 2.0;

	Xsp[1][0] = 0;
	Xsp[1][1] = -(yres / 2.0);
	Xsp[1][2] = 0;
	Xsp[1][3] = yres / 2.0;

	Xsp[2][0] = 0;
	Xsp[2][1] = 0;
	Xsp[2][2] = INT_MAX;
	Xsp[2][3] = 0;

	Xsp[3][0] = 0;
	Xsp[3][1] = 0;
	Xsp[3][2] = 0;
	Xsp[3][3] = 1;

	/*Xpi*/
	m_camera.Xpi[0][0] = 1;
	m_camera.Xpi[1][1] = 1;
	m_camera.Xpi[2][2] = 1 / d;
	m_camera.Xpi[3][3] = 1;
	m_camera.Xpi[3][2] = 1 / d;

	/*Xiw*/
	GzCoord cl, up;

	cl[0] = m_camera.lookat[0] - m_camera.position[0];
	cl[1] = m_camera.lookat[1] - m_camera.position[1];
	cl[2] = m_camera.lookat[2] - m_camera.position[2];
	float len_cl = sqrt((cl[0] * cl[0]) + (cl[1] * cl[1]) + (cl[2] * cl[2]));
	GzCoord z_axis = { (cl[0] / len_cl), (cl[1] / len_cl) ,(cl[2] / len_cl) };

	float up_dot_z = (m_camera.worldup[0] * z_axis[0]) + (m_camera.worldup[1] * z_axis[1]) + (m_camera.worldup[2] * z_axis[2]);
	up[0] = m_camera.worldup[0] - (up_dot_z * z_axis[0]);
	up[1] = m_camera.worldup[1] - (up_dot_z * z_axis[1]);
	up[2] = m_camera.worldup[2] - (up_dot_z * z_axis[2]);
	float len_up = sqrt((up[0] * up[0]) + (up[1] * up[1]) + (up[2] * up[2]));
	GzCoord y_axis = { (up[0] / len_up), (up[1] / len_up) ,(up[2] / len_up) };

	GzCoord x_axis = {
		((y_axis[1] * z_axis[2]) - (y_axis[2] * z_axis[1])),
		((y_axis[2] * z_axis[0]) - (y_axis[0] * z_axis[2])),
		((y_axis[0] * z_axis[1]) - (y_axis[1] * z_axis[0]))
	};

	GzCoord cs = {
		((x_axis[0] * m_camera.position[0]) + (x_axis[1] * m_camera.position[1]) + (x_axis[2] * m_camera.position[2])),
		((y_axis[0] * m_camera.position[0]) + (y_axis[1] * m_camera.position[1]) + (y_axis[2] * m_camera.position[2])),
		((z_axis[0] * m_camera.position[0]) + (z_axis[1] * m_camera.position[1]) + (z_axis[2] * m_camera.position[2]))
	};

	m_camera.Xiw[0][0] = x_axis[0];
	m_camera.Xiw[0][1] = x_axis[1];
	m_camera.Xiw[0][2] = x_axis[2];
	m_camera.Xiw[0][3] = -cs[0];

	m_camera.Xiw[1][0] = y_axis[0];
	m_camera.Xiw[1][1] = y_axis[1];
	m_camera.Xiw[1][2] = y_axis[2];
	m_camera.Xiw[1][3] = -cs[1];

	m_camera.Xiw[2][0] = z_axis[0];
	m_camera.Xiw[2][1] = z_axis[1];
	m_camera.Xiw[2][2] = z_axis[2];
	m_camera.Xiw[2][3] = -cs[2];

	m_camera.Xiw[3][0] = 0;
	m_camera.Xiw[3][1] = 0;
	m_camera.Xiw[3][2] = 0;
	m_camera.Xiw[3][3] = 1;

	GzPushMatrix(Xsp);
	GzPushMatrix(m_camera.Xpi);
	GzPushMatrix(m_camera.Xiw);
	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera) {
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/
	m_camera.FOV = camera.FOV;

	m_camera.lookat[0] = camera.lookat[0];
	m_camera.lookat[1] = camera.lookat[1];
	m_camera.lookat[2] = camera.lookat[2];

	m_camera.position[0] = camera.position[0];
	m_camera.position[1] = camera.position[1];
	m_camera.position[2] = camera.position[2];

	m_camera.worldup[0] = camera.worldup[0];
	m_camera.worldup[1] = camera.worldup[1];
	m_camera.worldup[2] = camera.worldup[2];

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix) {
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow

	*/
	if (matlevel == MATLEVELS) { return GZ_FAILURE; }
	if (matlevel == 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel][i][j] = matrix[i][j];
				Xnorm[matlevel][i][j] = (i == j) ? 1 : 0;
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel][i][j] = 0;
				for (int k = 0; k < 4; k++) {
					Ximage[matlevel][i][j] += Ximage[matlevel - 1][i][k] * matrix[k][j];
				}
			}
		}
		if (matlevel == 1) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Xnorm[matlevel][i][j] = (i == j) ? 1 : 0;
				}
			}
		}
		else {
			GzMatrix R;
			float k = 1 / sqrt(matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0] + matrix[2][0] * matrix[2][0]);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					R[i][j] = matrix[i][j] * k;
				}
				R[i][3] = 0;
				R[3][i] = 0;
			}
			R[3][3] = 1;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Xnorm[matlevel][i][j] = 0;
					for (int k = 0; k < 4; k++) {
						Xnorm[matlevel][i][j] += Xnorm[matlevel - 1][i][k] * R[k][j];
					}
				}
			}
		}
	}
	matlevel++;
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix() {
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	if (matlevel < 0) { return GZ_FAILURE; }
	matlevel--;
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
	for (int i = 0; i < numAttributes; i++) {
		GzColor *color;
		int *mode;
		GzLight *light;
		float *alpha, r, g, b, x, y, z, length;

		switch (nameList[i]) {
		case GZ_RGB_COLOR:
			color = (GzColor*)valueList[i];
			r = (((*color)[0] > 1) ? 1 : (((*color)[0] < 0) ? 0 : (*color)[0]));
			g = (((*color)[1] > 1) ? 1 : (((*color)[1] < 0) ? 0 : (*color)[1]));
			b = (((*color)[2] > 1) ? 1 : (((*color)[2] < 0) ? 0 : (*color)[2]));
			flatcolor[0] = r;
			flatcolor[1] = g;
			flatcolor[2] = b;
			break;
		case GZ_INTERPOLATE:
			mode = (int*)valueList[i];
			interp_mode = *mode;
			break;
		case GZ_DIRECTIONAL_LIGHT:
			light = (GzLight*)valueList[i];
			if (numlights >= MAX_LIGHTS) { return GZ_FAILURE; }
			lights[numlights].color[0] = light->color[0];
			lights[numlights].color[1] = light->color[1];
			lights[numlights].color[2] = light->color[2];

			x = light->direction[0];
			y = light->direction[1];
			z = light->direction[2];

			length = sqrt((x * x) + (y * y) + (z * z)); 
			//length = 1;
			lights[numlights].direction[0] = x / length;
			lights[numlights].direction[1] = y / length;
			lights[numlights].direction[2] = z / length;

			numlights++;
			break;
		case GZ_AMBIENT_LIGHT:
			light = (GzLight*)valueList[i];
			ambientlight.color[0] = light->color[0];
			ambientlight.color[1] = light->color[1];
			ambientlight.color[2] = light->color[2];
			break;
		case GZ_AMBIENT_COEFFICIENT:
			color = (GzColor*)valueList[i];
			Ka[0] = (*color)[0];
			Ka[1] = (*color)[1];
			Ka[2] = (*color)[2];
			break;
		case GZ_DIFFUSE_COEFFICIENT:
			color = (GzColor*)valueList[i];
			Kd[0] = (*color)[0];
			Kd[1] = (*color)[1];
			Kd[2] = (*color)[2];
			break;
		case GZ_SPECULAR_COEFFICIENT:
			color = (GzColor*)valueList[i];
			Ks[0] = (*color)[0];
			Ks[1] = (*color)[1];
			Ks[2] = (*color)[2];
			break;
		case GZ_DISTRIBUTION_COEFFICIENT:
			alpha = (float*)valueList[i];
			spec = (*alpha);
			break;
		default: break;
		}
	}
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
	bool outscreen;
	GzCoord normals[3], vertex[3];
	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_NORMAL) {
			GzCoord* normal = (GzCoord*)valueList[i];

			for (int j = 0; j < 3; j++) {
				normals[j][0] = normal[j][0];
				normals[j][1] = normal[j][1];
				normals[j][2] = normal[j][2];
			}

			/*Step 0. Transform Vertex*/
			float transformed[4][3];
			float original[4][3];

			for (int j = 0; j < 3; j++) {
				original[0][j] = normals[j][0];
				original[1][j] = normals[j][1];
				original[2][j] = normals[j][2];
				original[3][j] = 1;
			}

			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 3; k++) {
					transformed[j][k] = 0;
					for (int l = 0; l < 4; l++) {
						transformed[j][k] += Xnorm[matlevel - 1][j][l] * original[l][k];
					}
				}
			}

			float x, y, z, length;
			for (int j = 0; j < 3; j++) {
				x = transformed[0][j] / transformed[3][j];
				y = transformed[1][j] / transformed[3][j];
				z = transformed[2][j] / transformed[3][j];
				length = sqrt((x * x) + (y * y) + (z * z));
				normals[j][0] = x / length;
				normals[j][1] = y / length;
				normals[j][2] = z / length;
			}

		}
		else if (nameList[i] == GZ_POSITION) {
			GzCoord *positions = (GzCoord*)valueList[i];

			for (int j = 0; j < 3; j++) {
				vertex[j][0] = positions[j][0];
				vertex[j][1] = positions[j][1];
				vertex[j][2] = positions[j][2];
			}

			/*Step 0. Transform Vertex*/
			float transformed[4][3];
			float original[4][3];
			float coord_x[4], coord_y[4], coord_z[4];

			for (int j = 0; j < 3; j++) {
				original[0][j] = vertex[j][0];
				original[1][j] = vertex[j][1];
				original[2][j] = vertex[j][2];
				original[3][j] = 1;
			}

			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 3; k++) {
					transformed[j][k] = 0;
					for (int l = 0; l < 4; l++) {
						transformed[j][k] += Ximage[matlevel - 1][j][l] * original[l][k];
					}
				}
			}
			outscreen = false;

			for (int j = 0; j < 3; j++) {
				vertex[j][0] = transformed[0][j] / transformed[3][j];
				vertex[j][1] = transformed[1][j] / transformed[3][j];
				vertex[j][2] = transformed[2][j] / transformed[3][j];
				if (vertex[j][2] < 0) {
					outscreen = true;
				}
			}
		}
	}
	if (!outscreen) {
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
					GzCoord temp_n = { normals[k][0], normals[k][1], normals[k][2] };
					normals[k][0] = normals[k + 1][0];
					normals[k][1] = normals[k + 1][1];
					normals[k][2] = normals[k + 1][2];
					normals[k + 1][0] = temp_n[0];
					normals[k + 1][1] = temp_n[1];
					normals[k + 1][2] = temp_n[2];
				}
			}
		}
		GzColor outcolor[3];
		if (interp_mode == GZ_COLOR) {
			for (int i = 0; i < 3; i++) {
				calculateColor(normals[i], outcolor[i]);
			}
		}
		else {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					outcolor[i][j] = normals[i][j];
				}
			}
		}

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
			DDA[j].color_start[0] = outcolor[k][0];
			DDA[j].color_start[1] = outcolor[k][1];
			DDA[j].color_start[2] = outcolor[k][2];
			DDA[j].color_current[0] = outcolor[k][0];
			DDA[j].color_current[1] = outcolor[k][1];
			DDA[j].color_current[2] = outcolor[k][2];

			k = order[++index];
			DDA[j].end[0] = vertex[k][0];
			DDA[j].end[1] = vertex[k][1];
			DDA[j].end[2] = vertex[k][2];
			DDA[j].color_end[0] = outcolor[k][0];
			DDA[j].color_end[1] = outcolor[k][1];
			DDA[j].color_end[2] = outcolor[k][2];
		}

		/*Step 3. Sort edges by L or R*/
		enum triangle { LEFT, RIGHT };
		triangle type = LEFT;
		if (DDA[0].slopeX() < DDA[2].slopeX()) { type = LEFT; }
		else if (DDA[0].slopeX() > DDA[2].slopeX()) { type = RIGHT; }
		/*       1                1
		//LEFT    / |       RIGHT   | \
		//	   /  |				  |  \
		//	  /   |				  |   \
		//	  2   3               3    2
		//*/

		/*Step 4. Advance (1-2) and (1-3) DDA current positions to top y-scan line (ceiling)*/
		float deltaY = ceil(vertex[0][1]) - vertex[0][1];
		DDA[0].current[0] += (DDA[0].slopeX() * deltaY);
		DDA[0].current[1] += deltaY;
		DDA[0].current[2] += (DDA[0].slopeZ() * deltaY);
		DDA[2].current[0] += (DDA[2].slopeX() * deltaY);
		DDA[2].current[1] += deltaY;
		DDA[2].current[2] += (DDA[2].slopeZ() * deltaY);

		DDA[0].color_current[0] += (DDA[0].stepColor(0) * deltaY);
		DDA[0].color_current[1] += (DDA[0].stepColor(1) * deltaY);
		DDA[0].color_current[2] += (DDA[0].stepColor(2) * deltaY);
		DDA[2].color_current[0] += (DDA[2].stepColor(0) * deltaY);
		DDA[2].color_current[1] += (DDA[2].stepColor(1) * deltaY);
		DDA[2].color_current[2] += (DDA[2].stepColor(2) * deltaY);

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

			span.color_start[0] = (type == LEFT) ? DDA[0].color_current[0] : DDA[2].color_current[0];
			span.color_start[1] = (type == LEFT) ? DDA[0].color_current[1] : DDA[2].color_current[1];
			span.color_start[2] = (type == LEFT) ? DDA[0].color_current[2] : DDA[2].color_current[2];
			span.color_end[0] = (type == LEFT) ? DDA[2].color_current[0] : DDA[0].color_current[0];
			span.color_end[1] = (type == LEFT) ? DDA[2].color_current[1] : DDA[0].color_current[1];
			span.color_end[2] = (type == LEFT) ? DDA[2].color_current[2] : DDA[0].color_current[2];
			span.color_current[0] = span.color_start[0];
			span.color_current[1] = span.color_start[1];
			span.color_current[2] = span.color_start[2];


			/*Step 6. Advance span current position to left-most covered pixel (ceiling)*/
			float deltaX = ceil(span.start[0]) - span.start[0];
			span.current[0] += deltaX;
			span.current[1] += (span.slopeZ() * deltaX);
			span.color_current[0] += (span.stepColor(0) * deltaX);
			span.color_current[1] += (span.stepColor(1) * deltaX);
			span.color_current[2] += (span.stepColor(2) * deltaX);

			/*Step 7. Interpolate span position and parameters (Z) until current position > end*/
			while ((type == LEFT && span.current[0] < DDA[2].current[0]) || (type == RIGHT && span.current[0] < DDA[0].current[0])) {
				int currentX = (int)span.current[0];
				int currentY = (int)DDA[0].current[1];
				GzGet(currentX, currentY, &r, &g, &b, &a, &z);

				/*Step 8. Test interpolated-Z against FB-Z for each pixel - lowest Z wins. Write color value into FB pixel  (default or computed color)*/
				if (span.current[1] < z) {
					if (interp_mode == GZ_COLOR) {
						GzPut(currentX, currentY, ctoi(span.color_current[0]), ctoi(span.color_current[1]), ctoi(span.color_current[2]), 1, span.current[1]);
					}
					else if (interp_mode == GZ_NORMALS) {
						float x = span.color_current[0];
						float y = span.color_current[1];
						float z = span.color_current[2];
						float length = sqrt((x * x) + (y * y) + (z * z));
						GzColor phong;
						GzCoord new_normal = { x / length, y / length, z / length };
						calculateColor(new_normal, phong);
						GzPut(currentX, currentY, ctoi(phong[0]), ctoi(phong[1]), ctoi(phong[2]), 1, span.current[1]);
					}
					else {
						r = ctoi(flatcolor[0]);
						g = ctoi(flatcolor[1]);
						b = ctoi(flatcolor[2]);
						GzPut(currentX, currentY, r, g, b, 1, span.current[1]);
					}
				}

				/*Move one pixel over*/
				span.current[0]++;
				span.current[1] += span.slopeZ();
				span.color_current[0] += span.stepColor(0);
				span.color_current[1] += span.stepColor(1);
				span.color_current[2] += span.stepColor(2);
			}

			/*Move down on both DDAs*/
			DDA[0].current[0] += DDA[0].slopeX();
			DDA[0].current[1] += 1;
			DDA[0].current[2] += DDA[0].slopeZ();
			DDA[2].current[0] += DDA[2].slopeX();
			DDA[2].current[1] += 1;
			DDA[2].current[2] += DDA[2].slopeZ();

			DDA[0].color_current[0] += DDA[0].stepColor(0);
			DDA[0].color_current[1] += DDA[0].stepColor(1);
			DDA[0].color_current[2] += DDA[0].stepColor(2);
			DDA[2].color_current[0] += DDA[2].stepColor(0);
			DDA[2].color_current[1] += DDA[2].stepColor(1);
			DDA[2].color_current[2] += DDA[2].stepColor(2);
		}

		/*Step 9. Switch from 1-2 edge to 2-3 edge when current 1-2 edge position > Y(2)*/
		/*Step 10. Continue spans until vert 3 is passed: current 1-3 edge position > Y(3)*/
		deltaY = ceil(vertex[1][1]) - vertex[1][1];
		DDA[1].current[0] += (DDA[1].slopeX() * deltaY);
		DDA[1].current[1] += deltaY;
		DDA[1].current[2] += (DDA[1].slopeZ() * deltaY);

		DDA[1].color_current[0] += (DDA[1].stepColor(0) * deltaY);
		DDA[1].color_current[1] += (DDA[1].stepColor(1) * deltaY);
		DDA[1].color_current[2] += (DDA[1].stepColor(2) * deltaY);


		while (DDA[1].current[1] < DDA[1].end[1]) {

			/*Step 5. Setup span DDA on successive lines based on edge DDA position values.
			Set span DDA current and end positions to right and left edge values*/
			span.start[0] = (type == LEFT) ? DDA[1].current[0] : DDA[2].current[0];
			span.start[1] = (type == LEFT) ? DDA[1].current[2] : DDA[2].current[2];
			span.end[0] = (type == LEFT) ? DDA[2].current[0] : DDA[1].current[0];
			span.end[1] = (type == LEFT) ? DDA[2].current[2] : DDA[1].current[2];
			span.current[0] = span.start[0];
			span.current[1] = span.start[1];

			span.color_start[0] = (type == LEFT) ? DDA[1].color_current[0] : DDA[2].color_current[0];
			span.color_start[1] = (type == LEFT) ? DDA[1].color_current[1] : DDA[2].color_current[1];
			span.color_start[2] = (type == LEFT) ? DDA[1].color_current[2] : DDA[2].color_current[2];
			span.color_end[0] = (type == LEFT) ? DDA[2].color_current[0] : DDA[1].color_current[0];
			span.color_end[1] = (type == LEFT) ? DDA[2].color_current[1] : DDA[1].color_current[1];
			span.color_end[2] = (type == LEFT) ? DDA[2].color_current[2] : DDA[1].color_current[2];
			span.color_current[0] = span.color_start[0];
			span.color_current[1] = span.color_start[1];
			span.color_current[2] = span.color_start[2];

			/*Step 6. Advance span current position to left-most covered pixel (ceiling)*/
			float deltaX = ceil(span.start[0]) - span.start[0];
			span.current[0] += deltaX;
			span.current[1] += (span.slopeZ() * deltaX);
			span.color_current[0] += (span.stepColor(0) * deltaX);
			span.color_current[1] += (span.stepColor(1) * deltaX);
			span.color_current[2] += (span.stepColor(2) * deltaX);

			/*Step 7. Interpolate span position and parameters (Z) until current position > end*/
			while ((type == LEFT && span.current[0] < DDA[2].current[0]) || (type == RIGHT && span.current[0] < DDA[1].current[0])) {
				int currentX = (int)span.current[0];
				int currentY = (int)DDA[1].current[1];
				GzGet(currentX, currentY, &r, &g, &b, &a, &z);

				/*Step 8. Test interpolated-Z against FB-Z for each pixel - lowest Z wins. Write color value into FB pixel  (default or computed color)*/
				if (span.current[1] < z) {
					if (interp_mode == GZ_COLOR) {
						GzPut(currentX, currentY, ctoi(span.color_current[0]), ctoi(span.color_current[1]), ctoi(span.color_current[2]), 1, span.current[1]);
					}
					else if (interp_mode == GZ_NORMALS) {
						float x = span.color_current[0];
						float y = span.color_current[1];
						float z = span.color_current[2];
						float length = sqrt((x * x) + (y * y) + (z * z));
						GzColor phong;
						GzCoord new_normal = { x / length, y / length, z / length };
						calculateColor(new_normal, phong);
						GzPut(currentX, currentY, ctoi(phong[0]), ctoi(phong[1]), ctoi(phong[2]), 1, span.current[1]);

					}
					else {
						r = ctoi(flatcolor[0]);
						g = ctoi(flatcolor[1]);
						b = ctoi(flatcolor[2]);
						GzPut(currentX, currentY, r, g, b, 1, span.current[1]);
					}
				}

				/*Move one pixel over*/
				span.current[0] += 1;
				span.current[1] += span.slopeZ();
				span.color_current[0] += span.stepColor(0);
				span.color_current[1] += span.stepColor(1);
				span.color_current[2] += span.stepColor(2);
			}

			/*Move down on both DDAs*/
			DDA[1].current[0] += DDA[1].slopeX();
			DDA[1].current[1] += 1;
			DDA[1].current[2] += DDA[1].slopeZ();
			DDA[2].current[0] += DDA[2].slopeX();
			DDA[2].current[1] += 1;
			DDA[2].current[2] += DDA[2].slopeZ();

			DDA[1].color_current[0] += DDA[1].stepColor(0);
			DDA[1].color_current[1] += DDA[1].stepColor(1);
			DDA[1].color_current[2] += DDA[1].stepColor(2);
			DDA[2].color_current[0] += DDA[2].stepColor(0);
			DDA[2].color_current[1] += DDA[2].stepColor(1);
			DDA[2].color_current[2] += DDA[2].stepColor(2);
		}
	}

	return GZ_SUCCESS;
}

void GzRender::calculateColor(GzCoord normal, GzColor color) {
	GzCoord reflect, light_direction, light_color;
	GzCoord eye = { 0.0, 0.0, -1.0 };
	GzColor specular_color = { 0.0, 0.0, 0.0 };
	GzColor diffuse_color = { 0.0, 0.0, 0.0 };
	float NL, NE, RE;

	for (int j = 0; j < numlights; j++)
	{
		light_direction[0] = lights[j].direction[0];
		light_direction[1] = lights[j].direction[1];
		light_direction[2] = lights[j].direction[2];
		light_color[0] = lights[j].color[0];
		light_color[1] = lights[j].color[1];
		light_color[2] = lights[j].color[2];

		NL = light_direction[0] * normal[0] + light_direction[1] * normal[1] + light_direction[2] * normal[2];
		NE = eye[0] * normal[0] + eye[1] * normal[1] + eye[2] * normal[2];

		if (NL * NE < 0) { continue; }
		else if (NL * NE > 0 && NL < 0) {
			normal[0] = -normal[0];
			normal[1] = -normal[1];
			normal[2] = -normal[2];
			NL = -NL;
		}

		//Sigma(le * (NL))
		diffuse_color[0] += light_color[0] * NL;
		diffuse_color[1] += light_color[1] * NL;
		diffuse_color[2] += light_color[2] * NL;

		//R = 2 * (NL) * N - L unit-length
		float x = 2 * NL * normal[0] - light_direction[0];
		float y = 2 * NL * normal[1] - light_direction[1];
		float z = 2 * NL * normal[2] - light_direction[2];
		float length = sqrt((x * x) + (y * y) + (z * z));
		reflect[0] = x / length;
		reflect[1] = y / length;
		reflect[2] = z / length;

		RE = reflect[0] * eye[0] + reflect[1] * eye[1] + reflect[2] * eye[2];
		RE = (RE < 0) ? 0 : ((RE > 1) ? 1 : RE);
		RE = pow(RE, spec);
		//Sigma(le * (RE)^s)
		specular_color[0] += light_color[0] * RE;
		specular_color[1] += light_color[1] * RE;
		specular_color[2] += light_color[2] * RE;
	}

	color[0] = Ks[0] * specular_color[0] + Kd[0] * diffuse_color[0] + Ka[0] * ambientlight.color[0];
	color[1] = Ks[1] * specular_color[1] + Kd[1] * diffuse_color[1] + Ka[1] * ambientlight.color[1];
	color[2] = Ks[2] * specular_color[2] + Kd[2] * diffuse_color[2] + Ka[2] * ambientlight.color[2];

	color[0] = (color[0] < 0) ? 0 : ((color[0] > 1) ? 1 : color[0]);
	color[1] = (color[1] < 0) ? 0 : ((color[1] > 1) ? 1 : color[1]);
	color[2] = (color[2] < 0) ? 0 : ((color[2] > 1) ? 1 : color[2]);
}

