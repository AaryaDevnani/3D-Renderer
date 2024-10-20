/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <cmath>
#include <algorithm> 
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define PI (float) 3.141592

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/
	// create an instance of Matrix4
	Matrix4 matrix;
	matrix = Matrix4::convertFromGzMat(mat);

	// convert degree to radians
	float radian = degree * PI / 180;

	// create rotation matrix
	Matrix4 rotationMatrix = Matrix4(
		Vector4(1, 0, 0, 0),
		Vector4(0, cos(radian), -sin(radian), 0),
		Vector4(0, sin(radian), cos(radian), 0),
		Vector4(0, 0, 0, 1)
	);

	Matrix4 rotated = rotationMatrix * matrix;
	GzMatrix result;
	Matrix4::convertToGzMat(rotated, result);
	memcpy(mat, result, sizeof(GzMatrix));


	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	Matrix4 matrix;
	matrix = Matrix4::convertFromGzMat(mat);

	// convert degree to radians
	float radian = degree * PI / 180;

	// create rotation matrix
	Matrix4 rotationMatrix = Matrix4(
		Vector4(cos(radian), 0, sin(radian), 0),
		Vector4(0, 1, 0, 0),
		Vector4(-sin(radian), 0, cos(radian), 0),
		Vector4(0, 0, 0, 1)
	);

	Matrix4 rotated = rotationMatrix * matrix;
	GzMatrix result;
	Matrix4::convertToGzMat(rotated, result);
	memcpy(mat, result, sizeof(GzMatrix));


	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	Matrix4 matrix;
	matrix = Matrix4::convertFromGzMat(mat);

	// convert degree to radians
	float radian = degree * PI / 180;

	// create rotation matrix
	Matrix4 rotationMatrix = Matrix4(
		Vector4(cos(radian), -sin(radian), 0, 0),
		Vector4(sin(radian), cos(radian), 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(0, 0, 0, 1)
	);

	Matrix4 rotated = rotationMatrix * matrix;
	GzMatrix result;
	Matrix4::convertToGzMat(rotated, result);
	memcpy(mat, result, sizeof(GzMatrix));

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/
	Matrix4 matrix;
	matrix = Matrix4::convertFromGzMat(mat);
	Matrix4 translationMatrix = Matrix4(
		Vector4(1, 0, 0, translate[0]),
		Vector4(0, 1, 0, translate[1]),
		Vector4(0, 0, 1, translate[2]),
		Vector4(0, 0, 0, 1)
	);

	Matrix4 translated = translationMatrix * matrix;
	GzMatrix result;
	Matrix4::convertToGzMat(translated, result);
	memcpy(mat, result, sizeof(GzMatrix));

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/
	Matrix4 matrix;
	matrix = Matrix4::convertFromGzMat(mat);
	Matrix4 scalingMatrix = Matrix4(
		Vector4(scale[0], 0, 0, 0),
		Vector4(0, scale[1], 0, 0),
		Vector4(0, 0, scale[2], 0),
		Vector4(0, 0, 0, 1)
	);
	Matrix4 scaled = scalingMatrix * matrix;
	GzMatrix result;
	Matrix4::convertToGzMat(scaled, result);
	memcpy(mat, result, sizeof(GzMatrix));

	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	xres = xRes;
	yres = yRes;
	pixelbuffer = new GzPixel[xRes * yRes];
	framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes);
	numlights = 0;
	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	// setup Xsp
	Matrix4 XspMat = Matrix4(
		Vector4(xres / 2, 0, 0, xres / 2),
		Vector4(0, -yres / 2, 0, yres / 2),
		Vector4(0, 0, INT_MAX, 0),
		Vector4(0, 0, 0, 1)
	);

	GzMatrix XspGzMatrix;
	Matrix4::convertToGzMat(XspMat, XspGzMatrix);
	memcpy(Xsp, XspGzMatrix, sizeof(GzMatrix));



	// init default camera
	m_camera = GzCamera();

	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	m_camera.lookat[0] = 0;
	m_camera.lookat[1] = 0;
	m_camera.lookat[2] = 0;
	m_camera.worldup[0] = 0;
	m_camera.worldup[1] = 1;
	m_camera.worldup[2] = 0;
	m_camera.FOV = DEFAULT_FOV;


}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */

	delete[] pixelbuffer;
	delete[] framebuffer;
}

int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			int x = ARRAY(i, j);
			pixelbuffer[x].red = GzIntensity(2055);
			pixelbuffer[x].green = GzIntensity(1798);
			pixelbuffer[x].blue = GzIntensity(1541);
			pixelbuffer[x].alpha = GzIntensity(1);
			pixelbuffer[x].z = GzDepth(INT_MAX);
		}
	}
	for (int i = 0; i < 3 * xres * yres; i++) {
		framebuffer[i] = 0;
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/
	GzDefault();

	Vector3 up = Vector3(m_camera.worldup[0], m_camera.worldup[1], m_camera.worldup[2]);

	Vector3 c = Vector3(m_camera.position[0], m_camera.position[1], m_camera.position[2]);

	Vector3 lookat = Vector3(m_camera.lookat[0], m_camera.lookat[1], m_camera.lookat[2]);

	// compute Xiw and projection xform Xpi from camera definition

	Vector3 z = lookat - c;
	z = z.normalize();
	Vector3 y = up - z * up.dotProduct(z);
	y = y.normalize();
	Vector3 x = y.crossProduct(z);


	Matrix4 XiwMatrix = Matrix4(
		Vector4(x.m_x, x.m_y, x.m_z, -x.dotProduct(c)),
		Vector4(y.m_x, y.m_y, y.m_z, -y.dotProduct(c)),
		Vector4(z.m_x, z.m_y, z.m_z, -z.dotProduct(c)),
		Vector4(0, 0, 0, 1)
	);

	// Xpi (default fov is in degrees)
	float rad = m_camera.FOV * PI / 180;
	float d = tan((rad / 2));
	Matrix4 XpiMatrix = Matrix4(
		Vector4(1, 0, 0, 0),
		Vector4(0, 1, 0, 0),
		Vector4(0, 0, d, 0),
		Vector4(0, 0, d, 1)
	);

	// init Ximage - put Xsp at base of stack, push on Xpi and Xiw

	// Xsp
	matlevel = 0;
	GzPushMatrix(Xsp);


	// push Xpi on stack
	GzMatrix XpiGzMatrix;
	Matrix4::convertToGzMat(XpiMatrix, XpiGzMatrix);
	GzPushMatrix(XpiGzMatrix);

	// push Xiw on stack
	GzMatrix XiwGzMatrix;
	Matrix4::convertToGzMat(XiwMatrix, XiwGzMatrix);
	GzPushMatrix(XiwGzMatrix);

	memcpy(m_camera.Xiw, XiwGzMatrix, sizeof(GzMatrix));
	memcpy(m_camera.Xpi, XpiGzMatrix, sizeof(GzMatrix));


	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
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

int GzRender::GzPushMatrix(GzMatrix matrix)
{
	/*
	When you push a matrix to XNorm:
	Remove any translation (set last column to (0, 0, 0, 1))
	Remove any scaling (normalize columns)
	Manage it separately from Ximage in GzPushMatrix
	Matrices that must be pushed to XNorm are: Xiw, Xwm(s)
	*/
	// Check for stack overflow
	if (matlevel >= MATLEVELS) {
		return GZ_FAILURE;
	}

	if (matlevel == 0) {
		// Copy the given matrix to the first level
		memcpy(Ximage[0], matrix, sizeof(GzMatrix));


		//push identity matrix to Xnorm
		Matrix4 result = Matrix4();
		GzMatrix resultGz;
		Matrix4::convertToGzMat(result, resultGz);

		memcpy(Xnorm[0], resultGz, sizeof(GzMatrix));
		

	}
	else {
		// Ensure the previous matrix level exists before copying
		if (matlevel > 0 && matlevel < MATLEVELS) {

			// Multiply the given matrix with the previous matrix level
			//first convert to matrix4
			Matrix4 m1 = Matrix4::convertFromGzMat(Ximage[matlevel - 1]);
			Matrix4 m2 = Matrix4::convertFromGzMat(matrix);
			Matrix4 result = m1 * m2;
			GzMatrix resultGz;
			Matrix4::convertToGzMat(result, resultGz);

			memcpy(Ximage[matlevel], resultGz, sizeof(GzMatrix));

			if (matlevel == 1) {
				// push identity mat onto norm stack
				Matrix4 result = Matrix4();
				GzMatrix resultGz;
				Matrix4::convertToGzMat(result, resultGz);

				memcpy(Xnorm[matlevel], resultGz, sizeof(GzMatrix));
			}
			else {
				//remove all translation and scaling
				Matrix4 norm = Matrix4::convertFromGzMat(Xnorm[matlevel - 1]);
				Matrix4 result = norm * m2;
				result.setColumn(3, Vector4(0, 0, 0, 1));
				result.setColumn(0, result.getColumn(0).normalize());
				result.setColumn(1, result.getColumn(1).normalize());
				result.setColumn(2, result.getColumn(2).normalize());
				GzMatrix resultGz;
				Matrix4::convertToGzMat(result, resultGz);
				memcpy(Xnorm[matlevel], resultGz, sizeof(GzMatrix));
			}
		}
		else {
			// Handle invalid matlevel (safety check)
			return GZ_FAILURE;
		}
	}

	// Successfully pushed the matrix
	matlevel++; // Increment matlevel after pushing
	return GZ_SUCCESS;
}


int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	if (matlevel > 0) {
		matlevel--;
	}


	return GZ_SUCCESS;
}

GzIntensity pixelCheck(GzIntensity i) {
	if (i < 0) {
		return GzIntensity(0);
	}
	if (i > 4095) {
		return GzIntensity(4095);
	}
	return i;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */
	if (i < 0) {
		i = 0;
	}
	if (i >= xres) {
		i = xres - 1;
	}
	if (j < 0) {
		j = 0;
	}
	if (j >= yres) {
		j = yres - 1;
	}
	int coords = ARRAY(i, j);


	if (z < pixelbuffer[ARRAY(i, j)].z) {

		pixelbuffer[coords].red = pixelCheck(r);
		pixelbuffer[coords].green = pixelCheck(g);
		pixelbuffer[coords].blue = pixelCheck(b);
		pixelbuffer[coords].alpha = a;
		pixelbuffer[coords].z = z;

	}
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity* r, GzIntensity* g, GzIntensity* b, GzIntensity* a, GzDepth* z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */
	int x = ARRAY(i, j);
	*r = pixelbuffer[x].red;
	*g = pixelbuffer[x].green;
	*b = pixelbuffer[x].blue;
	*a = pixelbuffer[x].alpha;
	*z = pixelbuffer[x].z;

	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			int x = ARRAY(j, i);
			fprintf(outfile, "%c%c%c", char(pixelbuffer[x].red >> 4), char(pixelbuffer[x].green >> 4), char(pixelbuffer[x].blue >> 4));
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer:
		- put the pixels into the frame buffer
		- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
		- NOT red, green, and blue !!!
	*/
	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			int x = ARRAY(i, j);
			framebuffer[3 * x] = char(pixelbuffer[x].blue >> 4);
			framebuffer[3 * x + 1] = char(pixelbuffer[x].green >> 4);
			framebuffer[3 * x + 2] = char(pixelbuffer[x].red >> 4);
		}
	}

	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken* nameList, GzPointer* valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/
	for (int i = 0; i < numAttributes; i++) {
		if (nameList[i] == GZ_RGB_COLOR) {
			GzColor* color = (GzColor*)valueList[i];
			flatcolor[0] = color[0][0];
			flatcolor[1] = color[0][1];
			flatcolor[2] = color[0][2];
		}
		else if (nameList[i] == GZ_INTERPOLATE) {
			int* mode = (int*)valueList[i];
			interp_mode = *mode;
		}
		else if (nameList[i] == GZ_DIRECTIONAL_LIGHT) {
			GzLight* light = (GzLight*)valueList[i];
			lights[numlights] = *light;
			numlights++;
		}
		else if (nameList[i] == GZ_AMBIENT_LIGHT) {
			GzLight* light = (GzLight*)valueList[i];
			ambientlight = *light;
		}
		else if (nameList[i] == GZ_AMBIENT_COEFFICIENT) {
			GzColor* color = (GzColor*)valueList[i];
			Ka[0] = color[0][0];
			Ka[1] = color[0][1];
			Ka[2] = color[0][2];
		}
		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT) {
			GzColor* color = (GzColor*)valueList[i];
			Kd[0] = color[0][0];
			Kd[1] = color[0][1];
			Kd[2] = color[0][2];
		}
		else if (nameList[i] == GZ_SPECULAR_COEFFICIENT) {
			GzColor* color = (GzColor*)valueList[i];
			Ks[0] = color[0][0];
			Ks[1] = color[0][1];
			Ks[2] = color[0][2];
		}
		else if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT) {
			float* spec = (float*)valueList[i];
			this->spec = *spec;
		}
		
	}
	return GZ_SUCCESS;
}


static Vector3 ShadingEquation(GzRender* render, Vector3 Normal) {

	// Normalize the normal
	Normal = Normal.normalize();
	Vector3 viewDir = Vector3(0, 0, -1);
	float NdotE = Normal.dotProduct(viewDir);
	if (NdotE <= 0.0f) {
		Normal = Normal * -1;
	}

	Vector3 finalColor = Vector3();
	for (int i = 0; i < render->numlights; i++) {
		GzLight light = render->lights[i];

		// Compute N · L (dot product of surface normal and light direction)
		Vector3 lightDir(light.direction[0], light.direction[1], light.direction[2]);
		lightDir = lightDir.normalize();  // Ensure lightDir is normalized
		float NdotL = max(0.0f, Normal.dotProduct(lightDir));

		// Diffuse component (Lambertian reflection)
		Vector3 diffuse = Vector3(render->Kd[0], render->Kd[1], render->Kd[2]) * NdotL;

		// Compute the reflection vector R
		Vector3 R = (Normal * 2.0f * NdotL - lightDir).normalize();

		// Compute R · V (dot product of reflection and view direction)
		float RdotV = max(0.0f, R.dotProduct(viewDir));

		// Specular component (Blinn-Phong reflection)
		float specularPower = pow(RdotV, render->spec);
		Vector3 specular = Vector3(render->Ks[0], render->Ks[1], render->Ks[2]) * specularPower;

		// Accumulate diffuse and specular components
		finalColor = finalColor + Vector3(light.color[0], light.color[1], light.color[2]) * (diffuse + specular);
	}

	// Ambient light contribution
	Vector3 ambient = Vector3(render->Ka[0], render->Ka[1], render->Ka[2]) *
		Vector3(render->ambientlight.color[0], render->ambientlight.color[1], render->ambientlight.color[2]);

	// Add the ambient contribution to the final color
	finalColor = finalColor + ambient;

	// Clamp the color values
	finalColor.m_x = min(1.0f, max(0.0f, finalColor.m_x));
	finalColor.m_y = min(1.0f, max(0.0f, finalColor.m_y));
	finalColor.m_z = min(1.0f, max(0.0f, finalColor.m_z));

	return finalColor;
}



static void sortVerticesAndNormals(Vector3& temp1, Vector3& temp2, Vector3& temp3, Vector4& tempn1, Vector4& tempn2, Vector4& tempn3) {
	// Sort by y-coordinate first (smallest y comes first)
	if (temp1.m_y > temp2.m_y) {
		std::swap(temp1, temp2);
		std::swap(tempn1, tempn2);
	}
	if (temp1.m_y > temp3.m_y) {
		std::swap(temp1, temp3);
		std::swap(tempn1, tempn3);
	}
	if (temp2.m_y > temp3.m_y) {
		std::swap(temp2, temp3);
		std::swap(tempn2, tempn3);
	}

	// Now temp1 is the vertex at the top, and temp2 and temp3 are the lower vertices.
	// If temp2 and temp3 are at the same y level, sort them by x-coordinate
	if (temp2.m_y == temp3.m_y && temp2.m_x > temp3.m_x) {
		std::swap(temp2, temp3);
		std::swap(tempn2, tempn3);
	}

	// If temp2 and temp3 are not at the same y level, ensure temp2 is to the right of temp3
	if (temp2.m_y != temp3.m_y && temp2.m_x > temp3.m_x) {
		std::swap(temp2, temp3);
		std::swap(tempn2, tempn3);
	}
}



float interpolateColor(Vector3 v1, Vector3 v2, Vector3 v3, int j, int k, float color1, float color2, float color3) {
	Vector3 colorV1 = Vector3(v2.m_x - v1.m_x, v2.m_y - v1.m_y, color2 - color1);
	Vector3 colorV2 = Vector3(v3.m_x - v1.m_x, v3.m_y - v1.m_y, color3 - color1);

	Vector3 planeVector = colorV1.crossProduct(colorV2);
	float D = -(planeVector.m_x * v1.m_x + planeVector.m_y * v1.m_y + planeVector.m_z * color1);

	float interpolatedColor = (planeVector.m_x * j + planeVector.m_y * k + D) / -planeVector.m_z;

	return (interpolatedColor); // Return the interpolated value
}


int GzRender::GzPutTriangle(int	numParts, GzToken* nameList, GzPointer* valueList)
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
		if (nameList[i] == GZ_NULL_TOKEN) {
			continue;
		}
		if (nameList[i] == GZ_POSITION) {
			// get triangle coordinates
			GzCoord* vertexList = (GzCoord*)valueList[i];



			Vector4 temp1 = Vector4(vertexList[0][0], vertexList[0][1], vertexList[0][2], 1);
			Vector4 temp2 = Vector4(vertexList[1][0], vertexList[1][1], vertexList[1][2], 1);
			Vector4 temp3 = Vector4(vertexList[2][0], vertexList[2][1], vertexList[2][2], 1);

			/* multiply model space with Xsw*/
			Matrix4 modelMatrix = Matrix4::convertFromGzMat(Ximage[matlevel - 1]);
			temp1 = modelMatrix * temp1;
			temp2 = modelMatrix * temp2;
			temp3 = modelMatrix * temp3;

			if (temp1.m_z < 0 || temp2.m_z < 0 || temp3.m_z < 0) {
				return GZ_SUCCESS;
			}

			// divide by w
			temp1 = temp1 / temp1.m_w;
			temp2 = temp2 / temp2.m_w;
			temp3 = temp3 / temp3.m_w;

			// convert to vector3 
			Vector3 v1 = Vector3(temp1.m_x, temp1.m_y, temp1.m_z);
			Vector3 v2 = Vector3(temp2.m_x, temp2.m_y, temp2.m_z);
			Vector3 v3 = Vector3(temp3.m_x, temp3.m_y, temp3.m_z);



			GzCoord* normalList = (GzCoord*)valueList[i + 1];
			Vector4 n1 = Vector4(normalList[0][0], normalList[0][1], normalList[0][2],1);
			Vector4 n2 = Vector4(normalList[1][0], normalList[1][1], normalList[1][2],1);
			Vector4 n3 = Vector4(normalList[2][0], normalList[2][1], normalList[2][2],1);

			sortVerticesAndNormals(v1, v2, v3, n1, n2, n3);

			// get bounding box 
			int x_min = min(v1.m_x, min(v2.m_x, v3.m_x));
			int x_max = max(v1.m_x, max(v2.m_x, v3.m_x));
			int y_min = min(v1.m_y, min(v2.m_y, v3.m_y));
			int y_max = max(v1.m_y, max(v2.m_y, v3.m_y));


			float dX1 = v2.m_x - v1.m_x;
			float dY1 = v2.m_y - v1.m_y;

			float dX2 = v3.m_x - v2.m_x;
			float dY2 = v3.m_y - v2.m_y;

			float dX3 = v1.m_x - v3.m_x;
			float dY3 = v1.m_y - v3.m_y;
			

			// color all coords in bbox
			for (int j = x_min; j <= x_max; j++) {
				for (int k = y_min; k <= y_max; k++) {

					Vector3 p = Vector3(j, k, v1.m_z);

					float E1 = dY1 * (p.m_x - v1.m_x) - dX1 * (p.m_y - v1.m_y);
					float E2 = dY2 * (p.m_x - v2.m_x) - dX2 * (p.m_y - v2.m_y);
					float E3 = dY3 * (p.m_x - v3.m_x) - dX3 * (p.m_y - v3.m_y);
					if ((E1 <= 0 && E2 <= 0 && E3 <= 0) || (E1 >= 0 && E2 >= 0 && E3 >= 0)) {

						// z Interpolation
						Vector3 planeVector = (v2 - v1).crossProduct(v3 - v1);
						float D = -(planeVector.m_x * v1.m_x + planeVector.m_y * v1.m_y + planeVector.m_z * v1.m_z);
						GzDepth z = std::round((planeVector.m_x * j + planeVector.m_y * k + D) / -planeVector.m_z);

						Matrix4 XnormMatrix = Matrix4::convertFromGzMat(Xnorm[matlevel-1]);

						Vector3 finalColor = Vector3();

						if (this->interp_mode == GZ_FLAT) {
							// pass default normal
							Vector4 n1 = XnormMatrix.multVec4(Vector4(normalList[0][0], normalList[0][1], normalList[0][2], 1));
							//get color
							Vector3 shaded = ShadingEquation(this, Vector3(n1.m_x, n1.m_y, n1.m_z));
							
							finalColor = shaded;
						}
						else if (this->interp_mode == GZ_COLOR) {
							//Gouraud shading
							
							Vector4 newN1 = XnormMatrix.multVec4(n1);
							Vector4 newN2 = XnormMatrix.multVec4(n2);
							Vector4 newN3 = XnormMatrix.multVec4(n3);
							Vector3 Col1 = ShadingEquation(this, Vector3(newN1.m_x, newN1.m_y, newN1.m_z));
							Vector3 Col2 = ShadingEquation(this, Vector3(newN2.m_x, newN2.m_y, newN2.m_z));
							Vector3 Col3 = ShadingEquation(this, Vector3(newN3.m_x, newN3.m_y, newN3.m_z));

							finalColor.m_x = interpolateColor(v1, v2, v3, j, k, Col1.m_x, Col2.m_x, Col3.m_x);
							finalColor.m_y = interpolateColor(v1, v2, v3, j, k, Col1.m_y, Col2.m_y, Col3.m_y);
							finalColor.m_z = interpolateColor(v1, v2, v3, j, k, Col1.m_z, Col2.m_z, Col3.m_z);
							
							
						}
						else if (this->interp_mode == GZ_NORMALS) {
							// Phong shading
	
							Vector4 newN1 = XnormMatrix.multVec4(n1);
							Vector4 newN2 = XnormMatrix.multVec4(n2);
							Vector4 newN3 = XnormMatrix.multVec4(n3);

							// use j and k to interpolate normals 
							Vector3 n = Vector3();
							n.m_x = (interpolateColor(v1, v2, v3, j, k, newN1.m_x, newN2.m_x, newN3.m_x));
							n.m_y = (interpolateColor(v1, v2, v3, j, k, newN1.m_y, newN2.m_y, newN3.m_y));
							n.m_z = (interpolateColor(v1, v2, v3, j, k, newN1.m_z, newN2.m_z, newN3.m_z));
							n.normalize();
							finalColor = ShadingEquation(this, Vector3(n.getX(), n.getY(), n.getZ()));

						}

						GzIntensity r = GzIntensity(ctoi(finalColor.m_x));
						GzIntensity g = GzIntensity(ctoi(finalColor.m_y));
						GzIntensity b = GzIntensity(ctoi(finalColor.m_z));
						GzIntensity a = GzIntensity(1);
						GzPut(j, k, r, g, b, a, z);
							
							
					}


				}
			}
		}
	}
	return GZ_SUCCESS;
}

