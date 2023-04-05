/*
####################### ####################### ####################### ####################### #######################
	raute lib coded by #@(aka Raute)

	this file contains some own coded functions
####################### ####################### ####################### ####################### #######################
*/

#include "cg_local.h"

/*
####################### ####################### ####################### ####################### #######################

	3DTo2D-functions by #@

####################### ####################### ####################### ####################### #######################
*/

/*
#######################
Calculate_2DOf3D

A function for calculating the 2D-pos of a 3D-Dot ... base on my first lensflare src(for padmod)
returns qfalse if the dot is in or behind the cam ... NEW: will return SquaredDistance if visible
point is the 3d origin of the dot
refdef is the refdef for the drawn scene
x pointer on the x output var
y pointer on the y output var
#######################
*/
qboolean Calculate_2DOf3D(vec3_t point, float *x, float *y) {
	vec3_t vec;
	vec3_t axis[3];
	float width, height; // in units of a 640x480-grind

	width = 640.0f * (float)cg.refdef.width / (float)cgs.glconfig.vidWidth;
	height = 480.0f * (float)cg.refdef.height / (float)cgs.glconfig.vidHeight;

	// make a vector from camera to dot
	vec[0] = point[0] - cg.refdef.vieworg[0];
	vec[1] = point[1] - cg.refdef.vieworg[1];
	vec[2] = point[2] - cg.refdef.vieworg[2];

	// make a dotproduct to get a rough anglecheck ...
	if (((vec[0] * cg.refdef.viewaxis[0][0] + vec[1] * cg.refdef.viewaxis[0][1] + vec[2] * cg.refdef.viewaxis[0][2]) <= 0))
		return qfalse;

	// copy axis to get a short name ;)
	AxisCopy(cg.refdef.viewaxis, axis);
	if (vec[0] != 0.0f) // the normal formula doesn't work with vec[0]==0 ...
		*x = (((vec[0] * axis[0][2] - vec[2] * axis[0][0]) * (vec[1] * axis[2][0] - vec[0] * axis[2][1])) -
			  ((vec[0] * axis[0][1] - vec[1] * axis[0][0]) * (vec[2] * axis[2][0] - vec[0] * axis[2][2]))) /
			 (((vec[0] * axis[1][1] - vec[1] * axis[1][0]) * (vec[2] * axis[2][0] - vec[0] * axis[2][2])) -
			  ((vec[0] * axis[1][2] - vec[2] * axis[1][0]) * (vec[1] * axis[2][0] - vec[0] * axis[2][1])));
	else // the formula is symmetric ... so I exchange [0] with [2] and [2] with [0]
		*x = (((vec[2] * axis[0][0] - vec[0] * axis[0][2]) * (vec[1] * axis[2][2] - vec[2] * axis[2][1])) -
			  ((vec[2] * axis[0][1] - vec[1] * axis[0][2]) * (vec[0] * axis[2][2] - vec[2] * axis[2][0]))) /
			 (((vec[2] * axis[1][1] - vec[1] * axis[1][2]) * (vec[0] * axis[2][2] - vec[2] * axis[2][0])) -
			  ((vec[2] * axis[1][0] - vec[0] * axis[1][2]) * (vec[1] * axis[2][2] - vec[2] * axis[2][1])));
	if (vec[1] * axis[2][0] - vec[0] * axis[2][1]) // don't diff with 0
		*y = (vec[0] * axis[0][1] - vec[1] * axis[0][0] + (*x) * (vec[0] * axis[1][1] - vec[1] * axis[1][0])) /
			 (vec[1] * axis[2][0] - vec[0] * axis[2][1]);
	else // use a formular with an other "Definitionsluecke"(definition-gap ?)
		*y = (vec[0] * axis[0][2] - vec[2] * axis[0][0] + (*x) * (vec[0] * axis[1][2] - vec[2] * axis[1][0])) /
			 (vec[2] * axis[2][0] - vec[0] * axis[2][2]);

	*x *= -1.0f; // turn around so we get a normal 2d system

	// wow the calc is very simple (and I don't need any fixed mult value =) ... but I must use Rad instead of Deg ;)
	// scale to render-size
#define DEG2RAD_FLOAT 0.017453292f // PI/180
	*x *= ((float)(width)*0.5f) / tan(cg.refdef.fov_x * 0.5f * DEG2RAD_FLOAT);
	*y *= ((float)(height)*0.5f) / tan(cg.refdef.fov_y * 0.5f * DEG2RAD_FLOAT);

	// flip and shift to scr 2d-coordinates ...
	*x += (width * 0.5f) + (float)(cg.refdef.x);
	*y = ((height * 0.5f) + (float)(cg.refdef.y) - (*y));

	// debug line
	//	Com_Printf("{%2.2f|%2.2f|%2.2f} => Calculate_2DOf3D => x=%2.2f, y=%2.2f\n",point[0],point[1],point[2],*x,*y);

	return VectorLengthSquared(vec); // qtrue;
}
