/*
####################### ####################### #######################

	advanced 2D functions by #@
	based on "first UI-only Version"

####################### ####################### #######################
*/

#ifdef UI

#include "../q3_ui/ui_local.h"

#define FONTSHADER		uis.charset
#define WHITESHADER		uis.whiteShader
#define ADJUSTFROM640	UI_AdjustFrom640
#define INT_MSECTIME	uis.realtime

#else

#include "cg_local.h"

#define FONTSHADER		cgs.media.charsetShader
#define WHITESHADER		cgs.media.whiteShader
#define ADJUSTFROM640	CG_AdjustFrom640
#define INT_MSECTIME	cg.time

#endif

//#include "wop_advanced2d.h" // will be loaded with .._local.h

#define	DEG2RAD_FLOAT	0.017453292f // PI/180

/*
note:
typedef struct {
	vec3_t		xyz;
	float		st[2];
	byte		modulate[4];
} polyVert_t;

typedef struct poly_s {
	qhandle_t			hShader;
	int					numVerts;
	polyVert_t			*verts;
} poly_t;

RDF_NOWORLDMODEL

typedef struct {
	int			x, y, width, height;
	float		fov_x, fov_y;
	vec3_t		vieworg;
	vec3_t		viewaxis[3];		// transformation matrix

	// time in milliseconds for shader effects and other time dependent rendering issues
	int			time;

	int			rdflags;			// RDF_NOWORLDMODEL, etc

	// 1 bits will prevent the associated area from rendering at all
	byte		areamask[MAX_MAP_AREA_BYTES];

	// text messages for deform text shaders
	char		text[MAX_RENDER_STRINGS][MAX_RENDER_STRING_LENGTH];
} refdef_t;
*/

refdef_t	refdef2D;
int			initrefdef = 0;

/*
#######################
Initrefdef2D
#######################
*/
void Initrefdef2D(void)
{
	float		x,y,w,h;

	memset( &refdef2D, 0, sizeof( refdef2D ) );
	refdef2D.rdflags = RDF_NOWORLDMODEL;
	//orig (0/0/0)
	refdef2D.vieworg[0]=320.0f;
	refdef2D.vieworg[1]=240.0f;
//	refdef2D.vieworg[2]=23.0f;// ;) ... fov!
	refdef2D.vieworg[2]=1000.0f;
	refdef2D.viewaxis[0][0] = 0;
	refdef2D.viewaxis[0][1] = 0;
	refdef2D.viewaxis[0][2] = -1;
	refdef2D.viewaxis[1][0] = -1;
	refdef2D.viewaxis[1][1] = 0;
	refdef2D.viewaxis[1][2] = 0;
	refdef2D.viewaxis[2][0] = 0;
	refdef2D.viewaxis[2][1] = -1;
	refdef2D.viewaxis[2][2] = 0;
	x = 0;
	y = 0;
	w = 640;
	h = 480;
	ADJUSTFROM640( &x, &y, &w, &h );
	refdef2D.x = x;
	refdef2D.y = y;
	refdef2D.width = w;
	refdef2D.height = h;
//	refdef2D.fov_x = 171.77787f;//2*atan(320/23)
//	refdef2D.fov_y = 169.05174f;//2*atan(240/23)
	refdef2D.fov_x =  35.48934f;//2*atan(320/1000)
	refdef2D.fov_y =  26.99147f;//2*atan(240/1000)
	refdef2D.time = INT_MSECTIME;

	initrefdef=1;
}

/*
#######################
DrawPoly
#######################
*/
void DrawPoly(poly_t *poly)
{
	if(!initrefdef) Initrefdef2D();

	trap_R_ClearScene();

	trap_R_AddPolyToScene(poly->hShader,poly->numVerts,poly->verts);

	trap_R_RenderScene( &refdef2D );
}

/*
#######################
AdvancedDrawPicA
#######################
*/
void AdvancedDrawPicA( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader, vec4_t color, float angle, int turnorigin)
{
	poly_t		poly;
	polyVert_t	verts[4];
	vec2_t		vec_w,vec_h;

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = hShader;

	verts[0].modulate[0] =
		verts[1].modulate[0] =
		verts[2].modulate[0] =
		verts[3].modulate[0] = 255*color[0];

	verts[0].modulate[1] =
		verts[1].modulate[1] =
		verts[2].modulate[1] =
		verts[3].modulate[1] = 255*color[1];

	verts[0].modulate[2] =
		verts[1].modulate[2] =
		verts[2].modulate[2] =
		verts[3].modulate[2] = 255*color[2];

	verts[0].modulate[3] =
		verts[1].modulate[3] =
		verts[2].modulate[3] =
		verts[3].modulate[3] = 255*color[3];

//#define	DEG2RAD_FLOAT 0.017453292f // PI/180

	angle*=DEG2RAD_FLOAT;

	vec_w[0] = cos(angle);
	vec_w[1] = -sin(angle);
	vec_h[0] = sin(angle);
	vec_h[1] = cos(angle);

	switch(turnorigin)
	{
	default:
	case TURNORIGIN_UPPERLEFT:
		verts[0].xyz[0]=x;
		verts[0].xyz[1]=y;

		verts[1].xyz[0]=x+vec_w[0]*w;
		verts[1].xyz[1]=y+vec_w[1]*w;

		verts[2].xyz[0]=x+vec_w[0]*w+vec_h[0]*h;
		verts[2].xyz[1]=y+vec_w[1]*w+vec_h[1]*h;

		verts[3].xyz[0]=x+vec_h[0]*h;
		verts[3].xyz[1]=y+vec_h[1]*h;
		break;
	case TURNORIGIN_MIDDLELEFT:
		verts[0].xyz[0]=x-vec_h[0]*h*0.5f;
		verts[0].xyz[1]=y-vec_h[1]*h*0.5f;

		verts[1].xyz[0]=x+vec_w[0]*w-vec_h[0]*h*0.5f;
		verts[1].xyz[1]=y+vec_w[1]*w-vec_h[1]*h*0.5f;

		verts[2].xyz[0]=x+vec_w[0]*w+vec_h[0]*h*0.5f;
		verts[2].xyz[1]=y+vec_w[1]*w+vec_h[1]*h*0.5f;

		verts[3].xyz[0]=x+vec_h[0]*h*0.5f;
		verts[3].xyz[1]=y+vec_h[1]*h*0.5f;
		break;
	case TURNORIGIN_MIDDLECENTER:
		verts[0].xyz[0]=x-vec_w[0]*w*0.5f-vec_h[0]*h*0.5f;
		verts[0].xyz[1]=y-vec_w[1]*w*0.5f-vec_h[1]*h*0.5f;

		verts[1].xyz[0]=x+vec_w[0]*w*0.5f-vec_h[0]*h*0.5f;
		verts[1].xyz[1]=y+vec_w[1]*w*0.5f-vec_h[1]*h*0.5f;

		verts[2].xyz[0]=x+vec_w[0]*w*0.5f+vec_h[0]*h*0.5f;
		verts[2].xyz[1]=y+vec_w[1]*w*0.5f+vec_h[1]*h*0.5f;

		verts[3].xyz[0]=x-vec_w[0]*w*0.5f+vec_h[0]*h*0.5f;
		verts[3].xyz[1]=y-vec_w[1]*w*0.5f+vec_h[1]*h*0.5f;
		break;
	}

	verts[0].st[0]=s1;
	verts[0].st[1]=t1;

	verts[1].st[0]=s2;
	verts[1].st[1]=t1;

	verts[2].st[0]=s2;
	verts[2].st[1]=t2;

	verts[3].st[0]=s1;
	verts[3].st[1]=t2;

//	Com_Printf("(%2.2f|%2.2f)(%2.2f|%2.2f)(%2.2f|%2.2f)(%2.2f|%2.2f)\n",verts[0].xyz[0],verts[0].xyz[1],verts[1].xyz[0],verts[1].xyz[1],verts[2].xyz[0],verts[2].xyz[1],verts[3].xyz[0],verts[3].xyz[1]);

	DrawPoly(&poly);
}

/*
#######################
DrawLine
#######################
*/
void DrawLine( float x1, float y1, float x2, float y2, float size, vec4_t color)
{
	poly_t		poly;
	polyVert_t	verts[4];
	vec2_t		vec_w;
	float		length_1;

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = WHITESHADER;

	verts[0].modulate[0] =
		verts[1].modulate[0] =
		verts[2].modulate[0] =
		verts[3].modulate[0] = 255*color[0];

	verts[0].modulate[1] =
		verts[1].modulate[1] =
		verts[2].modulate[1] =
		verts[3].modulate[1] = 255*color[1];

	verts[0].modulate[2] =
		verts[1].modulate[2] =
		verts[2].modulate[2] =
		verts[3].modulate[2] = 255*color[2];

	verts[0].modulate[3] =
		verts[1].modulate[3] =
		verts[2].modulate[3] =
		verts[3].modulate[3] = 255*color[3];

	vec_w[0] = y1-y2;
	vec_w[1] = x2-x1;

	length_1 = 1.0f/sqrt(vec_w[0]*vec_w[0]+vec_w[1]*vec_w[1]);

	vec_w[0] *= length_1;
	vec_w[1] *= length_1;

	verts[0].xyz[0]=x1-vec_w[0]*size*0.5f;
	verts[0].xyz[1]=y1-vec_w[1]*size*0.5f;

	verts[1].xyz[0]=x2-vec_w[0]*size*0.5f;
	verts[1].xyz[1]=y2-vec_w[1]*size*0.5f;

	verts[2].xyz[0]=x2+vec_w[0]*size*0.5f;
	verts[2].xyz[1]=y2+vec_w[1]*size*0.5f;

	verts[3].xyz[0]=x1+vec_w[0]*size*0.5f;
	verts[3].xyz[1]=y1+vec_w[1]*size*0.5f;

	verts[0].st[0]=0;
	verts[0].st[1]=0;

	verts[1].st[0]=1;
	verts[1].st[1]=0;

	verts[2].st[0]=1;
	verts[2].st[1]=1;

	verts[3].st[0]=0;
	verts[3].st[1]=1;

	DrawPoly(&poly);
}

/*
#######################
DrawPic2Color
#######################
*/
void DrawPic2Color( float x, float y, float w, float h, float s1, float t1, float s2, float t2, vec4_t color1, vec4_t color2, qhandle_t shader )
{
	poly_t		poly;
	polyVert_t	verts[4];

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = shader;

	verts[0].modulate[0] =
		verts[3].modulate[0] = 255*color1[0];
	verts[1].modulate[0] =
		verts[2].modulate[0] = 255*color2[0];

	verts[0].modulate[1] =
		verts[3].modulate[1] = 255*color1[1];
	verts[1].modulate[1] =
		verts[2].modulate[1] = 255*color2[1];

	verts[0].modulate[2] =
		verts[3].modulate[2] = 255*color1[2];
	verts[1].modulate[2] =
		verts[2].modulate[2] = 255*color2[2];

	verts[0].modulate[3] =
		verts[3].modulate[3] = 255*color1[3];
	verts[1].modulate[3] =
		verts[2].modulate[3] = 255*color2[3];

	verts[0].xyz[0]=x;
	verts[0].xyz[1]=y;

	verts[1].xyz[0]=x+w;
	verts[1].xyz[1]=y;

	verts[2].xyz[0]=x+w;
	verts[2].xyz[1]=y+h;

	verts[3].xyz[0]=x;
	verts[3].xyz[1]=y+h;

	verts[0].st[0]=s1;
	verts[0].st[1]=t1;

	verts[1].st[0]=s2;
	verts[1].st[1]=t1;

	verts[2].st[0]=s2;
	verts[2].st[1]=t2;

	verts[3].st[0]=s1;
	verts[3].st[1]=t2;

	DrawPoly(&poly);
}

/*
#######################
FillRect2Color
#######################
*/
void FillRect2Color( float x, float y, float w, float h, vec4_t color1, vec4_t color2 )
{
	poly_t		poly;
	polyVert_t	verts[4];

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = WHITESHADER;

	verts[0].modulate[0] =
		verts[3].modulate[0] = 255*color1[0];
	verts[1].modulate[0] =
		verts[2].modulate[0] = 255*color2[0];

	verts[0].modulate[1] =
		verts[3].modulate[1] = 255*color1[1];
	verts[1].modulate[1] =
		verts[2].modulate[1] = 255*color2[1];

	verts[0].modulate[2] =
		verts[3].modulate[2] = 255*color1[2];
	verts[1].modulate[2] =
		verts[2].modulate[2] = 255*color2[2];

	verts[0].modulate[3] =
		verts[3].modulate[3] = 255*color1[3];
	verts[1].modulate[3] =
		verts[2].modulate[3] = 255*color2[3];

	verts[0].xyz[0]=x;
	verts[0].xyz[1]=y;

	verts[1].xyz[0]=x+w;
	verts[1].xyz[1]=y;

	verts[2].xyz[0]=x+w;
	verts[2].xyz[1]=y+h;

	verts[3].xyz[0]=x;
	verts[3].xyz[1]=y+h;

	verts[0].st[0]=0;
	verts[0].st[1]=0;

	verts[1].st[0]=1;
	verts[1].st[1]=0;

	verts[2].st[0]=1;
	verts[2].st[1]=1;

	verts[3].st[0]=0;
	verts[3].st[1]=1;

	DrawPoly(&poly);
}

/*
#######################
FillTriangle
#######################
*/
void FillTriangle( float x1, float y1, float x2, float y2, float x3, float y3, vec4_t color )
{
	poly_t		poly;
	polyVert_t	verts[3];
	int			itmp;

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 3;
	poly.hShader = WHITESHADER;

//	(x2-x1)   (x3-x1)
//	(y2-y1) x (y3-y1)   = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1)
//		  0   0

	if( ((x2-x1)*(y3-y1)-(y2-y1)*(x3-x1))<0 )
	{
		itmp=x2;
		x2=x3;
		x3=itmp;

		itmp=y2;
		y2=y3;
		y3=itmp;
	}

	verts[0].modulate[0] =
		verts[1].modulate[0] =
		verts[2].modulate[0] = 255*color[0];

	verts[0].modulate[1] =
		verts[1].modulate[1] =
		verts[2].modulate[1] = 255*color[1];

	verts[0].modulate[2] =
		verts[1].modulate[2] =
		verts[2].modulate[2] = 255*color[2];

	verts[0].modulate[3] =
		verts[1].modulate[3] =
		verts[2].modulate[3] = 255*color[3];

	verts[0].xyz[0]=x1;
	verts[0].xyz[1]=y1;

	verts[1].xyz[0]=x2;
	verts[1].xyz[1]=y2;

	verts[2].xyz[0]=x3;
	verts[2].xyz[1]=y3;

	verts[0].st[0]=0;
	verts[0].st[1]=0;

	verts[1].st[0]=1;
	verts[1].st[1]=0;

	verts[2].st[0]=1;
	verts[2].st[1]=1;

	DrawPoly(&poly);
}

/*
#######################
DrawTriangle
#######################
*/
void DrawTriangle( float x1, float y1, float x2, float y2, float x3, float y3, vec4_t color )
{
	DrawLine(x1,y1,x2,y2,1,color);
	DrawLine(x2,y2,x3,y3,1,color);
	DrawLine(x3,y3,x1,y1,1,color);
}

/*
#######################
Draw4VertsPic
#######################
*/
void Draw4VertsPic( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, qhandle_t shader,vec4_t color )
{
	poly_t		poly;
	polyVert_t	verts[4];

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = shader;

//	Com_Printf("x1=%3.2f, y1=%3.2f, x2=%3.2f, y2=%3.2f\nx3=%3.2f, y3=%3.2f, x4=%3.2f, y4=%3.2f\n", x1, y1, x2, y2, x3, y3, x4, y4);

	verts[0].modulate[0] =
		verts[1].modulate[0] =
		verts[2].modulate[0] =
		verts[3].modulate[0] = 255*color[0];

	verts[0].modulate[1] =
		verts[1].modulate[1] =
		verts[2].modulate[1] =
		verts[3].modulate[1] = 255*color[1];

	verts[0].modulate[2] =
		verts[1].modulate[2] =
		verts[2].modulate[2] =
		verts[3].modulate[2] = 255*color[2];

	verts[0].modulate[3] =
		verts[1].modulate[3] =
		verts[2].modulate[3] =
		verts[3].modulate[3] = 255*color[3];

	verts[0].xyz[0]=x1;
	verts[0].xyz[1]=y1;

	verts[1].xyz[0]=x2;
	verts[1].xyz[1]=y2;

	verts[2].xyz[0]=x3;
	verts[2].xyz[1]=y3;

	verts[3].xyz[0]=x4;
	verts[3].xyz[1]=y4;

	verts[0].st[0]=0;
	verts[0].st[1]=0;

	verts[1].st[0]=1;
	verts[1].st[1]=0;

	verts[2].st[0]=1;
	verts[2].st[1]=1;

	verts[3].st[0]=1;
	verts[3].st[1]=0;

	DrawPoly(&poly);
}



/*
####################### ####################### #######################

	Turnable String

####################### ####################### #######################
*/

/*
#######################
AddCharToScene
#######################
*/
void AddCharToScene( float x, float y, int ch, vec4_t color, vec2_t vec_w, vec2_t vec_h)
{
	poly_t		poly;
	polyVert_t	verts[4];
//	vec2_t		vec_w,vec_h;
	int			row, col;

	if(ch==' ') return;

	memset(&poly,0,sizeof(poly));
	memset(&verts,0,sizeof(verts));
	poly.verts = verts;
	poly.numVerts = 4;
	poly.hShader = FONTSHADER;

	verts[0].modulate[0] =
		verts[1].modulate[0] =
		verts[2].modulate[0] =
		verts[3].modulate[0] = 255*color[0];

	verts[0].modulate[1] =
		verts[1].modulate[1] =
		verts[2].modulate[1] =
		verts[3].modulate[1] = 255*color[1];

	verts[0].modulate[2] =
		verts[1].modulate[2] =
		verts[2].modulate[2] =
		verts[3].modulate[2] = 255*color[2];

	verts[0].modulate[3] =
		verts[1].modulate[3] =
		verts[2].modulate[3] =
		verts[3].modulate[3] = 255*color[3];

//	angle*=DEG2RAD_FLOAT;

//	vec_w[0] = cos(angle);
//	vec_w[1] = -sin(angle);
//	vec_h[0] = sin(angle);
//	vec_h[1] = cos(angle);

//	case TURNORIGIN_UPPERLEFT:
//		verts[0].xyz[0]=x;
//		verts[0].xyz[1]=y;
//
//		verts[1].xyz[0]=x+vec_w[0]*w;
//		verts[1].xyz[1]=y+vec_w[1]*w;
//
//		verts[2].xyz[0]=x+vec_w[0]*w+vec_h[0]*h;
//		verts[2].xyz[1]=y+vec_w[1]*w+vec_h[1]*h;
//
//		verts[3].xyz[0]=x+vec_h[0]*h;
//		verts[3].xyz[1]=y+vec_h[1]*h;

		verts[0].xyz[0]=x;
		verts[0].xyz[1]=y;

		verts[1].xyz[0]=x+vec_w[0];
		verts[1].xyz[1]=y+vec_w[1];

		verts[2].xyz[0]=x+vec_w[0]+vec_h[0];
		verts[2].xyz[1]=y+vec_w[1]+vec_h[1];

		verts[3].xyz[0]=x+vec_h[0];
		verts[3].xyz[1]=y+vec_h[1];


/* from CG_DrawChar:
	row = ch>>4;
	col = ch&15;

	frow = row*0.0625;
	fcol = col*0.0625;
	size = 0.0625;
*/
	row = ch>>4;
	col = ch&15;

	verts[0].st[0]=col*0.0625f;
	verts[0].st[1]=row*0.0625f;

	verts[1].st[0]=(col+1)*0.0625f;
	verts[1].st[1]=row*0.0625f;

	verts[2].st[0]=(col+1)*0.0625f;
	verts[2].st[1]=(row+1)*0.0625f;

	verts[3].st[0]=col*0.0625f;
	verts[3].st[1]=(row+1)*0.0625f;

//	Com_Printf("(%2.2f|%2.2f)(%2.2f|%2.2f)(%2.2f|%2.2f)(%2.2f|%2.2f)\n",verts[0].xyz[0],verts[0].xyz[1],verts[1].xyz[0],verts[1].xyz[1],verts[2].xyz[0],verts[2].xyz[1],verts[3].xyz[0],verts[3].xyz[1]);

	trap_R_AddPolyToScene(poly.hShader,poly.numVerts,poly.verts);
}

//#define TURNORIGIN_UPPERLEFT		1
//#define TURNORIGIN_MIDDLELEFT		2
//#define TURNORIGIN_MIDDLECENTER	3

int colorAndPosinc(const char *str, int *spos, float* tmpColor,qboolean forceColor) {
	int posInc = 1;
	int i=*spos;

	if ( Q_IsColorString( str+i ) ) {
		if ( !forceColor ) {
//			memcpy( tmpColor, g_color_table[ColorIndex(str[i+1])], sizeof(tmpColor) );
//			tmpColor[3] = color[3];
			memcpy( tmpColor, g_color_table[ColorIndex(str[i+1])], sizeof(vec3_t) );
		}
		++(*spos);
		posInc=0;
	}

	return posInc;
}

/*
#######################
DrawTurnableString
#######################
*/
void DrawTurnableStringFC(float x, float y, const char *s, vec4_t color, float charHeight, float angle, int turnorigin, qboolean forceColor)
{
	vec2_t	vec_w, vec_h;
	int		i,j, sLen;
	vec4_t	tmpColor;

	memcpy(tmpColor,color,sizeof(vec4_t));

	if(!initrefdef) Initrefdef2D();

	sLen=strlen(s);

	trap_R_ClearScene();

	angle*=DEG2RAD_FLOAT;

	vec_w[0] = cos(angle)*charHeight*0.5f;
	vec_w[1] = -sin(angle)*charHeight*0.5f;
	vec_h[0] = sin(angle)*charHeight;
	vec_h[1] = cos(angle)*charHeight;

	j=0;
	switch(turnorigin)
	{
	default:
	case TURNORIGIN_UPPERLEFT:
		for(i=0;i<sLen;i++) {
			if(colorAndPosinc(s,&i,tmpColor,forceColor)) {
				AddCharToScene(x+j*vec_w[0],y+j*vec_w[1],s[i],tmpColor,vec_w,vec_h);
				++j;
			}
		}
		break;
	case TURNORIGIN_MIDDLELEFT:
		for(i=0;i<sLen;i++) {
			if(colorAndPosinc(s,&i,tmpColor,forceColor)) {
				AddCharToScene(x+j*vec_w[0]-vec_h[0]*0.5f,y+j*vec_w[1]-vec_h[1]*0.5f,s[i],tmpColor,vec_w,vec_h);
				++j;
			}
		}
		break;
	case TURNORIGIN_MIDDLECENTER:
		for(i=0;i<sLen;i++){
			if(colorAndPosinc(s,&i,tmpColor,forceColor)) {
				AddCharToScene(x+((float)j-(float)sLen*0.5f)*vec_w[0]-vec_h[0]*0.5f,y+((float)j-(float)sLen*0.5f)*vec_w[1]-vec_h[1]*0.5f,s[i],tmpColor,vec_w,vec_h);
				++j;
			}
		}
		break;
	}

	trap_R_RenderScene( &refdef2D );
}

void DrawTurnableString(float x, float y, const char *s, vec4_t color, float charHeight, float angle, int turnorigin) {
	DrawTurnableStringFC(x, y, s, color, charHeight, angle, turnorigin, qfalse);
}


/*
####################### ####################### #######################

	"Cutable" String

####################### ####################### #######################
*/

#define CSIZE 0.0625f
static void DrawCharWithCutFrame(float x,float y,char ch,float w,float h,float fl,float ft,float fr,float fb) {
	float lcut,tcut,rcut,bcut;
	int row, col;
	float s1, t1, s2, t2;

	if(x>fr || y>fb || x+w<fl || y+h<ft)
		return; // not in the frame

	lcut = (fl>x)?((fl-x)/w):0.0f;
	rcut = (fr<x+w)?(((x+w)-fr)/w):0.0f;
	tcut = (ft>y)?((ft-y)/h):0.0f;
	bcut = (fb<y+h)?(((y+h)-fb)/h):0.0f;


/* from CG_DrawChar:
	row = ch>>4;
	col = ch&15;

	frow = row*0.0625;
	fcol = col*0.0625;
	size = 0.0625;
*/
	row = ch>>4;
	col = ch&15;
	s1 = (col*CSIZE);
	t1 = (row*CSIZE);
	s2 = ((col+1)*CSIZE);
	t2 = ((row+1)*CSIZE);

	s1 += (lcut*CSIZE);
	t1 += (tcut*CSIZE);
	s2 -= (rcut*CSIZE);
	t2 -= (bcut*CSIZE);

	ADJUSTFROM640(&x,&y,&w,&h);
	trap_R_DrawStretchPic(x+lcut*w, y+tcut*h, w*(1.0f-lcut-rcut), h*(1.0f-tcut-bcut), s1, t1, s2, t2, FONTSHADER);
}
/**
 *
 * Args:
 * x leftedge on 640x480
 * y topedge on 640x480
 * str String that should be drawn
 * cW width of one letter
 * cH height of one letter
 * fl frame leftedge
 * ft frame topedge
 * fr frame rightedge
 * fb frame bottomedge
 */
void DrawStringWithCutFrameFC(float x,float y,const char* str,vec4_t color,float cW,float cH,float fl,float ft,float fr,float fb,qboolean forceColor) {
	int i,sLen;
	float j=0.0f;
	vec4_t tmpColor;

	if(str==NULL) return;

	sLen = strlen(str);

	trap_R_SetColor(color);
	j=0;
	for(i=0;i<sLen;++i) {
		if ( Q_IsColorString( str+i ) ) {
			if ( !forceColor ) {
				memcpy( tmpColor, g_color_table[ColorIndex(str[i+1])], sizeof(tmpColor) );
				tmpColor[3] = color[3];
				trap_R_SetColor( tmpColor );
			}
			++i;
			continue;
		}

		DrawCharWithCutFrame(x+j,y,str[i],cW,cH,fl,ft,fr,fb);
		j+=cW;
	}
	trap_R_SetColor(NULL);
}

void DrawStringWithCutFrame(float x,float y,const char* str,vec4_t color,float cW,float cH,float fl,float ft,float fr,float fb) {
	DrawStringWithCutFrameFC(x,y,str,color,cW,cH,fl,ft,fr,fb,qfalse);
}
