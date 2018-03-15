/*
####################### ####################### #######################

	advanced 2D functions by #@

####################### ####################### #######################
*/

#ifndef WOP_ADVANCED_H
#define WOP_ADVANCED_H

#define TURNORIGIN_UPPERLEFT	1
#define TURNORIGIN_MIDDLELEFT	2
#define TURNORIGIN_MIDDLECENTER	3

void AdvancedDrawPicA( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader, vec4_t color, float angle, int turnorigin );

void DrawLine( float x1, float y1, float x2, float y2, float size, vec4_t color );

void DrawPic2Color( float x, float y, float w, float h, float s1, float t1, float s2, float t2, vec4_t color1, vec4_t color2, qhandle_t shader );

void FillTriangle( float x1, float y1, float x2, float y2, float x3, float y3, vec4_t color );

void DrawTriangle( float x1, float y1, float x2, float y2, float x3, float y3, vec4_t color );

void Draw4VertsPic( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, qhandle_t shader,vec4_t color );

void DrawTurnableString(float x, float y, const char *s, vec4_t color, float charHeight, float angle, int turnorigin);

void DrawStringWithCutFrame(float x,float y,const char* str,vec4_t color,float cW,float cH,float fl,float ft,float fr,float fb);

#endif
