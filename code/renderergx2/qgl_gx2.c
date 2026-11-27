/*
 * ioQuake3-U — Wii U native (wut / GX2) port
 * code/renderergx2/qgl_gx2.c — defines renderergl1's qgl* function pointers.
 * Routes a few choke-point calls to GX2BE_*, rest are typed no-ops.
 */

#include "../renderergl1/tr_local.h"
#include "tr_gx2_backend.h"

/* ---- pointer definitions ---- */

#define GLE(ret, name, ...) name##proc * qgl##name;
QGL_1_1_PROCS;
QGL_1_1_FIXED_FUNCTION_PROCS;
QGL_DESKTOP_1_1_PROCS;
QGL_DESKTOP_1_1_FIXED_FUNCTION_PROCS;
QGL_3_0_PROCS;
#undef GLE

void (APIENTRYP qglActiveTextureARB) (GLenum texture);
void (APIENTRYP qglClientActiveTextureARB) (GLenum texture);
void (APIENTRYP qglMultiTexCoord2fARB) (GLenum target, GLfloat s, GLfloat t);
void (APIENTRYP qglLockArraysEXT) (GLint first, GLsizei count);
void (APIENTRYP qglUnlockArraysEXT) (void);

int qglMajorVersion = 1;
int qglMinorVersion = 1;
int qglesMajorVersion = 0;
int qglesMinorVersion = 0;

/* ---- typed no-ops ---- */

static void gx2_noop_void(void)                          {}
static void gx2_noop_u(GLenum a)                         { (void)a; }
static void gx2_noop_uu(GLenum a, GLenum b)              { (void)a;(void)b; }
static void gx2_noop_i(GLint a)                          { (void)a; }
static void gx2_noop_f(GLfloat a)                        { (void)a; }
static void gx2_noop_d(GLclampd a)                       { (void)a; }
static void gx2_noop_b(GLboolean a)                      { (void)a; }
static void gx2_noop_bbbb(GLboolean a, GLboolean b, GLboolean c, GLboolean d)
	{ (void)a;(void)b;(void)c;(void)d; }
static void gx2_noop_nu(GLsizei n, const GLuint *p)      { (void)n;(void)p; }
static void gx2_noop_ni(GLsizei n, GLuint *p)            { (void)n;(void)p; }
static GLenum gx2_noop_ret_u(void)                       { return GL_NO_ERROR; }
static const GLubyte *gx2_noop_ret_str(GLenum a)         { (void)a; return (const GLubyte *)""; }
static const GLubyte *gx2_noop_ret_stri(GLenum a, GLuint i)
	{ (void)a;(void)i; return (const GLubyte *)""; }
static void gx2_noop_getintegerv(GLenum pname, GLint *params)
	{ (void)pname; if (params) *params = 0; }
static void gx2_noop_getbooleanv(GLenum pname, GLboolean *params)
	{ (void)pname; if (params) *params = 0; }
static void gx2_noop_teximage2d(GLenum tgt, GLint lvl, GLint ifmt,
	GLsizei w, GLsizei h, GLint b, GLenum fmt, GLenum tp, const GLvoid *d)
	{ (void)tgt;(void)lvl;(void)ifmt;(void)w;(void)h;(void)b;(void)fmt;(void)tp;(void)d; }
static void gx2_noop_texsubimage2d(GLenum tgt, GLint lvl,
	GLint xo, GLint yo, GLsizei w, GLsizei h, GLenum fmt, GLenum tp, const GLvoid *d)
	{ (void)tgt;(void)lvl;(void)xo;(void)yo;(void)w;(void)h;(void)fmt;(void)tp;(void)d; }
static void gx2_noop_copytexsubimage2d(GLenum tgt, GLint lvl,
	GLint xo, GLint yo, GLint x, GLint y, GLsizei w, GLsizei h)
	{ (void)tgt;(void)lvl;(void)xo;(void)yo;(void)x;(void)y;(void)w;(void)h; }
static void gx2_noop_readpixels(GLint x, GLint y, GLsizei w, GLsizei h,
	GLenum fmt, GLenum tp, GLvoid *d)
	{ (void)x;(void)y;(void)w;(void)h;(void)fmt;(void)tp;(void)d; }
static void gx2_noop_rect(GLint x, GLint y, GLsizei w, GLsizei h)
	{ (void)x;(void)y;(void)w;(void)h; }
static void gx2_noop_stencil3(GLenum a, GLint b, GLuint c)
	{ (void)a;(void)b;(void)c; }
static void gx2_noop_stencil2(GLenum a, GLenum b, GLenum c)
	{ (void)a;(void)b;(void)c; }
static void gx2_noop_texparamf(GLenum a, GLenum b, GLfloat c)
	{ (void)a;(void)b;(void)c; }
static void gx2_noop_texparami(GLenum a, GLenum b, GLint c)
	{ (void)a;(void)b;(void)c; }
static void gx2_noop_alpharef(GLenum a, GLclampf b)      { (void)a;(void)b; }
static void gx2_noop_color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{ (void)r;(void)g;(void)b;(void)a; }
static void gx2_noop_color3f(GLfloat r, GLfloat g, GLfloat b)
	{ (void)r;(void)g;(void)b; }
static void gx2_noop_color4ubv(const GLubyte *v)         { (void)v; }
static void gx2_noop_loadmatrix(const GLfloat *m)        { (void)m; }
static void gx2_noop_translatef(GLfloat x, GLfloat y, GLfloat z)
	{ (void)x;(void)y;(void)z; }
static void gx2_noop_frustum(GLdouble l, GLdouble r, GLdouble b, GLdouble t,
	GLdouble n, GLdouble f)
	{ (void)l;(void)r;(void)b;(void)t;(void)n;(void)f; }
static void gx2_noop_ortho(GLdouble l, GLdouble r, GLdouble b, GLdouble t,
	GLdouble n, GLdouble f)
	{ (void)l;(void)r;(void)b;(void)t;(void)n;(void)f; }
static void gx2_noop_clipplane(GLenum p, const GLdouble *eq)
	{ (void)p;(void)eq; }
static void gx2_noop_texcoord2f(GLfloat s, GLfloat t)    { (void)s;(void)t; }
static void gx2_noop_texcoord2fv(const GLfloat *v)       { (void)v; }
static void gx2_noop_vertex2f(GLfloat x, GLfloat y)      { (void)x;(void)y; }
static void gx2_noop_vertex3f(GLfloat x, GLfloat y, GLfloat z)
	{ (void)x;(void)y;(void)z; }
static void gx2_noop_vertex3fv(const GLfloat *v)         { (void)v; }
static void gx2_noop_arrayelement(GLint i)               { (void)i; }
static void gx2_noop_begin(GLenum m)                     { (void)m; }
static void gx2_noop_drawelements(GLenum m, GLsizei c, GLenum t, const GLvoid *i)
	{ (void)m;(void)c;(void)t;(void)i; }
static void gx2_noop_drawarrays(GLenum m, GLint f, GLsizei c)
	{ (void)m;(void)f;(void)c; }
static void gx2_noop_texenvf(GLenum t, GLenum p, GLfloat v)
	{ (void)t;(void)p;(void)v; }
static void gx2_noop_shademodel(GLenum m)                { (void)m; }
static void gx2_noop_multitex(GLenum t)                  { (void)t; }
static void gx2_noop_multitexcoord(GLenum t, GLfloat s, GLfloat v)
	{ (void)t;(void)s;(void)v; }
static void gx2_noop_lockarrays(GLint f, GLsizei c)      { (void)f;(void)c; }
static void gx2_noop_unlockarrays(void)                  {}
static void gx2_noop_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *p)
	{ (void)size;(void)type;(void)stride;(void)p; }

/* ---- routed wrappers ---- */

static void gx2_route_depthrange(GLclampd n, GLclampd f)
{
	GX2BE_DepthRange((float)n, (float)f);
}

/* GL stride 0 means tightly packed: size * sizeof(type) */
static int gx2_eff_stride(GLint size, GLenum type, GLsizei stride)
{
	if (stride != 0)
		return (int)stride;
	return (int)size * ((type == GL_UNSIGNED_BYTE) ? 1 : 4);
}
static void gx2_route_vertexptr(GLint size, GLenum type, GLsizei stride, const GLvoid *p)
{
	GX2BE_SetVertexPtr(p, gx2_eff_stride(size, type, stride));
}
static void gx2_route_colorptr(GLint size, GLenum type, GLsizei stride, const GLvoid *p)
{
	GX2BE_SetColorPtr(p, gx2_eff_stride(size, type, stride));
}
static void gx2_route_texcoordptr(GLint size, GLenum type, GLsizei stride, const GLvoid *p)
{
	GX2BE_SetTexCoordPtr(p, gx2_eff_stride(size, type, stride));
}
static void gx2_route_enable(GLenum cap)
{
	if (cap == GL_TEXTURE_2D)
		GX2BE_SetTexture2DEnabled(1);
	else if (cap == GL_POLYGON_OFFSET_FILL)
		GX2BE_SetPolygonOffsetEnabled(1);
	/* other caps handled at choke points or unsupported — ignore */
}
static void gx2_route_disable(GLenum cap)
{
	if (cap == GL_TEXTURE_2D)
		GX2BE_SetTexture2DEnabled(0);
	else if (cap == GL_POLYGON_OFFSET_FILL)
		GX2BE_SetPolygonOffsetEnabled(0);
}
static void gx2_route_polygonoffset(GLfloat factor, GLfloat units)
{
	GX2BE_PolygonOffset((float)factor, (float)units);
}
static void gx2_route_finish(void)
{
	GX2BE_Finish();
}

void QGL_Init(void)
{
	qglBindTexture          = (void(*)(GLenum,GLuint))gx2_noop_uu;
	qglBlendFunc            = (void(*)(GLenum,GLenum))gx2_noop_uu;
	qglClear                = (void(*)(GLbitfield))gx2_noop_u;
	qglClearColor           = (void(*)(GLclampf,GLclampf,GLclampf,GLclampf))gx2_noop_color4f;
	qglClearStencil         = (void(*)(GLint))gx2_noop_i;
	qglColorMask            = (void(*)(GLboolean,GLboolean,GLboolean,GLboolean))gx2_noop_bbbb;
	qglCopyTexSubImage2D    = gx2_noop_copytexsubimage2d;
	qglCullFace             = (void(*)(GLenum))gx2_noop_u;
	qglDeleteTextures       = (void(*)(GLsizei,const GLuint*))gx2_noop_nu;
	qglDepthFunc            = (void(*)(GLenum))gx2_noop_u;
	qglDepthMask            = (void(*)(GLboolean))gx2_noop_b;
	qglDisable              = gx2_route_disable;
	qglDrawArrays           = gx2_noop_drawarrays;
	qglDrawElements         = gx2_noop_drawelements;
	qglEnable               = gx2_route_enable;
	qglFinish               = gx2_route_finish;
	qglFlush                = gx2_noop_void;
	qglGenTextures          = (void(*)(GLsizei,GLuint*))gx2_noop_ni;
	qglGetBooleanv          = gx2_noop_getbooleanv;
	qglGetError             = gx2_noop_ret_u;
	qglGetIntegerv          = gx2_noop_getintegerv;
	qglGetString            = gx2_noop_ret_str;
	qglLineWidth            = (void(*)(GLfloat))gx2_noop_f;
	qglPolygonOffset        = gx2_route_polygonoffset;
	qglReadPixels           = gx2_noop_readpixels;
	qglScissor              = gx2_noop_rect;
	qglStencilFunc          = gx2_noop_stencil3;
	qglStencilMask          = (void(*)(GLuint))gx2_noop_u;
	qglStencilOp            = gx2_noop_stencil2;
	qglTexImage2D           = gx2_noop_teximage2d;
	qglTexParameterf        = gx2_noop_texparamf;
	qglTexParameteri        = gx2_noop_texparami;
	qglTexSubImage2D        = gx2_noop_texsubimage2d;
	qglViewport             = gx2_noop_rect;

	qglAlphaFunc            = gx2_noop_alpharef;
	qglColor4f              = gx2_noop_color4f;
	qglColorPointer         = gx2_route_colorptr;
	qglDisableClientState   = (void(*)(GLenum))gx2_noop_u;
	qglEnableClientState    = (void(*)(GLenum))gx2_noop_u;
	qglLoadIdentity         = gx2_noop_void;
	qglLoadMatrixf          = gx2_noop_loadmatrix;
	qglMatrixMode           = (void(*)(GLenum))gx2_noop_u;
	qglPopMatrix            = gx2_noop_void;
	qglPushMatrix           = gx2_noop_void;
	qglShadeModel           = gx2_noop_shademodel;
	qglTexCoordPointer      = gx2_route_texcoordptr;
	qglTexEnvf              = gx2_noop_texenvf;
	qglTranslatef           = gx2_noop_translatef;
	qglVertexPointer        = gx2_route_vertexptr;

	qglClearDepth           = gx2_noop_d;
	qglDepthRange           = gx2_route_depthrange;
	qglDrawBuffer           = (void(*)(GLenum))gx2_noop_u;
	qglPolygonMode          = (void(*)(GLenum,GLenum))gx2_noop_uu;

	qglArrayElement         = gx2_noop_arrayelement;
	qglBegin                = gx2_noop_begin;
	qglClipPlane            = gx2_noop_clipplane;
	qglColor3f              = gx2_noop_color3f;
	qglColor4ubv            = gx2_noop_color4ubv;
	qglEnd                  = gx2_noop_void;
	qglFrustum              = gx2_noop_frustum;
	qglOrtho                = gx2_noop_ortho;
	qglTexCoord2f           = gx2_noop_texcoord2f;
	qglTexCoord2fv          = gx2_noop_texcoord2fv;
	qglVertex2f             = gx2_noop_vertex2f;
	qglVertex3f             = gx2_noop_vertex3f;
	qglVertex3fv            = gx2_noop_vertex3fv;

	qglGetStringi           = gx2_noop_ret_stri;

	qglActiveTextureARB       = gx2_noop_multitex;
	qglClientActiveTextureARB = gx2_noop_multitex;
	qglMultiTexCoord2fARB     = gx2_noop_multitexcoord;

	/* CVA no-ops force primitives=2 (indexed path) in tr_shade.c */
	qglLockArraysEXT   = gx2_noop_lockarrays;
	qglUnlockArraysEXT = gx2_noop_unlockarrays;

	(void)gx2_noop_pointer;
}

void QGL_Shutdown(void)
{
}
