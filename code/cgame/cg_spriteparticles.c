/*
####################### ####################### ####################### ####################### #######################
	sprite particles coded by #@(aka Raute)

	test coding a own particle-system
####################### ####################### ####################### ####################### #######################
*/

#include "cg_local.h"

// vv vars vv
typedef struct changeshader_s {
	struct changeshader_s *next;
	qhandle_t shader;
	int time;
} changeshader_t;

typedef struct changecolor_s {
	struct changecolor_s *next;
	vec4_t color;
	vec4_t oldcolor;
	int time;
	int fadetime;
} changecolor_t;

typedef struct changesize_s {
	struct changesize_s *next;
	float changepersecond;
	int time;
} changesize_t;

typedef struct sparticle_s {
	struct sparticle_s *next, *prev; // freep only with next

	vec3_t origin;
	vec3_t velocity;
	vec3_t random;	// crandom@origin
	vec3_t vrandom; // crandom@velocity
	vec3_t acceleration;
	float bounce;
	int starttime;
	int endtime;
	float radius;
	qhandle_t currentshader;
	vec4_t currentcolor;
	changeshader_t *csh;
	changecolor_t *cc;
	changesize_t *csi;
} sparticle_t;

/*(old defs)
#define MAX_PARTICLES		512
#define MAX_SHADERCHANGES	512
#define MAX_COLORCHANGES	1024//oder noch mehr?
#define MAX_SIZECHANGES		1024//oder noch mehr?
*/
#define MAX_PARTICLES 2048
#define MAX_SHADERCHANGES 2048
#define MAX_COLORCHANGES 4096 // oder noch mehr?
#define MAX_SIZECHANGES 4096  // oder noch mehr?

static changeshader_t cshmem[MAX_SHADERCHANGES];
static changecolor_t ccmem[MAX_COLORCHANGES];
static changesize_t csimem[MAX_SIZECHANGES];
static changeshader_t *freecsh;
static changecolor_t *freecc;
static changesize_t *freecsi;
static sparticle_t pmem[MAX_PARTICLES];
static sparticle_t *firstp, *lastp;
static sparticle_t *freep;

typedef struct {
	qhandle_t starts[3];
} spritehandles_t;

static spritehandles_t spritehandles;

/*
====================
Init_SpriteParticles
====================
*/
void Init_SpriteParticles(void) {
	int i;

	memset(&pmem, 0, sizeof(pmem));

	firstp = lastp = NULL;

	freep = &pmem[0];
	for (i = 0; i < (MAX_PARTICLES - 1); i++) {
		pmem[i].next = &pmem[i + 1];
	}

	memset(&cshmem, 0, sizeof(cshmem));
	memset(&ccmem, 0, sizeof(ccmem));
	memset(&csimem, 0, sizeof(csimem));

	freecsh = &cshmem[0];
	for (i = 0; i < (MAX_SHADERCHANGES - 1); i++) {
		cshmem[i].next = &cshmem[i + 1];
	}
	freecc = &ccmem[0];
	for (i = 0; i < (MAX_COLORCHANGES - 1); i++) {
		ccmem[i].next = &ccmem[i + 1];
	}
	freecsi = &csimem[0];
	for (i = 0; i < (MAX_SIZECHANGES - 1); i++) {
		csimem[i].next = &csimem[i + 1];
	}

	spritehandles.starts[0] = trap_R_RegisterShader("sprites/star01");
	spritehandles.starts[1] = trap_R_RegisterShader("sprites/star02");
	spritehandles.starts[2] = trap_R_RegisterShader("sprites/star03");
}

static void Free_SpriteParticle(sparticle_t *p) {
	changeshader_t *tmpcsh;
	changecolor_t *tmpcc;
	changesize_t *tmpcsi;

	if (p < &pmem[0] || p > &pmem[MAX_PARTICLES - 1]) {
		Com_Error(ERR_FATAL, "free spriteparticle got a wrong pointer (%p)\n", (const void*)p);
		return;
	}

	while (p->csh) {
		tmpcsh = p->csh->next;
		memset(p->csh, 0, sizeof(*p->csh));
		p->csh->next = freecsh;
		freecsh = p->csh;
		p->csh = tmpcsh;
	}
	while (p->cc) {
		tmpcc = p->cc->next;
		memset(p->cc, 0, sizeof(*p->cc));
		p->cc->next = freecc;
		freecc = p->cc;
		p->cc = tmpcc;
	}
	while (p->csi) {
		tmpcsi = p->csi->next;
		memset(p->csi, 0, sizeof(*p->csi));
		p->csi->next = freecsi;
		freecsi = p->csi;
		p->csi = tmpcsi;
	}

	if (p == firstp)
		firstp = p->next;
	if (p == lastp)
		lastp = p->prev;

	if (p->prev)
		p->prev->next = p->next;
	if (p->next)
		p->next->prev = p->prev;

	memset(p, 0, sizeof(*p));

	p->next = freep;
	freep = p;
}

typedef enum {
	CE_NOTHING,

	CE_MEMFULL,
	CE_WRONGVALUES

} changeerror_t;

static changeerror_t AddCCToParticle(sparticle_t *p, int fadetime, int time, float r, float g, float b, float a) {
	changecolor_t *tmpcc;

	if (freecc == NULL)
		return CE_MEMFULL;

	if ((r > 1.0f || r < 0.0f) || (g > 1.0f || g < 0.0f) || (b > 1.0f || b < 0.0f) || (a > 1.0f || a < 0.0f))
		return CE_WRONGVALUES;

	freecc->time = time;
	freecc->fadetime = fadetime;
	freecc->color[0] = r;
	freecc->color[1] = g;
	freecc->color[2] = b;
	freecc->color[3] = a;

	if (!p->cc) {
		freecc->oldcolor[0] = p->currentcolor[0];
		freecc->oldcolor[1] = p->currentcolor[1];
		freecc->oldcolor[2] = p->currentcolor[2];
		freecc->oldcolor[3] = p->currentcolor[3];

		p->cc = freecc;
	} else {
		for (tmpcc = p->cc; tmpcc->next != NULL; tmpcc = tmpcc->next) {
			/* das machen wir besser beim cc-wechsel
						freecc->oldcolor[0]=tmpcc->color[0];
						freecc->oldcolor[1]=tmpcc->color[1];
						freecc->oldcolor[2]=tmpcc->color[2];
						freecc->oldcolor[3]=tmpcc->color[3];
			*/
		}
		tmpcc->next = freecc;
	}

	tmpcc = freecc->next;
	freecc->next = NULL;
	freecc = tmpcc;

	return CE_NOTHING;
}

static sparticle_t *Alloc_SpriteParticle(void) {
	sparticle_t *tmpp;

	if (!freep) // we must free something
	{
		Free_SpriteParticle(lastp);
	}

	tmpp = freep;
	freep = tmpp->next;

	if (firstp)
		firstp->prev = tmpp;
	tmpp->next = firstp;
	firstp = tmpp;
	if (!lastp)
		lastp = tmpp;

	return tmpp;
}

// puffs when running around with speedy
void LaunchSpeedyPuffTrail(vec3_t origin) {
	sparticle_t *p;
	changeerror_t tmpce;

	p = Alloc_SpriteParticle();

	p->starttime = cg.time;

	p->currentshader = cgs.media.smokePuffShader;

	p->endtime = cg.time + 4000;

	p->origin[0] = origin[0];
	p->origin[1] = origin[1];
	p->origin[2] = origin[2];

	p->radius = 8.0f;

	p->velocity[0] = 0.0f;
	p->velocity[1] = 0.0f;
	p->velocity[2] = 1.0f;

	p->vrandom[0] = 0.0f;
	p->vrandom[1] = 0.2f;
	p->vrandom[2] = 0.2f;

	p->currentcolor[0] = 1.0f;
	p->currentcolor[1] = 0.0f;
	p->currentcolor[2] = 0.8f;
	p->currentcolor[3] = 0.7f;

	// FIXME: Magical constants
	if ((tmpce = AddCCToParticle(p, 0, 2100, 1.0f, 0.8f, 1.0f, 0.80f)))
		Com_Printf("changeerror=%i\n", tmpce);
	if ((tmpce = AddCCToParticle(p, 2100, 3000, 1.0f, 1.0f, 1.0f, 0.8f)))
		Com_Printf("changeerror=%i\n", tmpce);
	if ((tmpce = AddCCToParticle(p, 3100, 4000, 1.0f, 1.0f, 1.0f, 0.0f)))
		Com_Printf("changeerror=%i\n", tmpce);
}

/*
LaunchRevivalParticle

Launches a single particle for revival powerup effect at $origin that
exists for $lifetime ticks.

*/
#define CHANGE_ERROR(x) Com_Printf("changeerror=%i\n", x)
void LaunchRevivalParticle(vec3_t origin, const int lifetime) {
	sparticle_t *p;
	changeerror_t tmpce;

	p = Alloc_SpriteParticle();
	p->starttime = cg.time;
	p->endtime = cg.time + lifetime;

	p->origin[0] = origin[0];
	p->origin[1] = origin[1];
	p->origin[2] = origin[2];

	p->currentshader = cgs.media.revivalParticleShader;
	p->radius = 3.0f;

	p->velocity[0] = 0.0f;
	p->velocity[1] = 0.0f;
	p->velocity[2] = 16.0f;

	p->vrandom[0] = 0.0f; // ?? not really sure what these do yet ^_^
	p->vrandom[1] = 0.2f;
	p->vrandom[2] = 0.2f;

	// start with 100% transparent
	p->currentcolor[0] = 0.0f;
	p->currentcolor[1] = 0.0f;
	p->currentcolor[2] = 0.0f;
	p->currentcolor[3] = 0.0f;

	// fade it in over 1/4 of its lifetime
	if ((tmpce = AddCCToParticle(p, 0, lifetime / 4, 1.0f, 1.0f, 1.0f, 0.0f)))
		CHANGE_ERROR(tmpce);

	// start fading out again after 3/4 of its lifetime
	if ((tmpce = AddCCToParticle(p, lifetime * 3 / 4, lifetime, 0.0f, 0.0f, 0.0f, 0.0f)))
		CHANGE_ERROR(tmpce);
}

static void CheckCurrentStats(sparticle_t *p) {
	float ftmp;
	vec3_t oldOrigin;
	trace_t tr;
	void *tmpP;

	if (p->csh && cg.time >= p->starttime + p->csh->time) {
		p->currentshader = p->csh->shader;
		tmpP = p->csh->next;
		memset(p->csh, 0, sizeof(*p->csh));
		p->csh->next = freecsh;
		freecsh = p->csh;
		p->csh = tmpP;
	}
	if (p->cc && cg.time >= p->starttime + p->cc->fadetime) {
		float tmpf;

		if (p->cc->time == p->cc->fadetime)
			tmpf = 1.0f;
		else {
			tmpf = (float)(cg.time - p->starttime - p->cc->fadetime) / (float)(p->cc->time - p->cc->fadetime);
			if (tmpf > 1.0f)
				tmpf = 1.0f;
		}

		p->currentcolor[0] = p->cc->color[0] * tmpf + p->cc->oldcolor[0] * (1 - tmpf);
		p->currentcolor[1] = p->cc->color[1] * tmpf + p->cc->oldcolor[1] * (1 - tmpf);
		p->currentcolor[2] = p->cc->color[2] * tmpf + p->cc->oldcolor[2] * (1 - tmpf);
		p->currentcolor[3] = p->cc->color[3] * tmpf + p->cc->oldcolor[3] * (1 - tmpf);

		if (cg.time >= p->starttime + p->cc->time) {
			tmpP = p->cc->next;
			memset(p->cc, 0, sizeof(*p->cc));
			p->cc->next = freecc;
			freecc = p->cc;
			p->cc = tmpP;
			if (tmpP) {
				p->cc->oldcolor[0] = p->currentcolor[0];
				p->cc->oldcolor[1] = p->currentcolor[1];
				p->cc->oldcolor[2] = p->currentcolor[2];
				p->cc->oldcolor[3] = p->currentcolor[3];
			}
		}
	}
	if (p->csi) {
		if (cg.time < p->starttime + p->csi->time) {
			p->radius += p->csi->changepersecond * cg.frametime * 0.001f;
		} else {
			tmpP = p->csi->next;
			memset(p->csi, 0, sizeof(changesize_t));
			p->csi->next = freecsi;
			freecsi = p->csi;
			p->csi = tmpP;
		}
	}

	oldOrigin[0] = p->origin[0];
	oldOrigin[1] = p->origin[1];
	oldOrigin[2] = p->origin[2];

	ftmp = crandom();
	p->origin[0] += (p->velocity[0] + p->random[0] * ftmp) * (float)cg.frametime * 0.001f;
	p->origin[1] += (p->velocity[1] + p->random[1] * ftmp) * (float)cg.frametime * 0.001f;
	p->origin[2] += (p->velocity[2] + p->random[2] * ftmp) * (float)cg.frametime * 0.001f;

	if (p->bounce > 0.0f) {
		// if bounce>0.0f check bounce on solid (and change velocity ... keep accel/origin ...)
		CG_Trace(&tr, oldOrigin, NULL, NULL, p->origin, -1, CONTENTS_SOLID);
		if (tr.fraction != 1.0f) {
			// noch nicht gemacht...
		}
	}

	ftmp = crandom();
	p->velocity[0] += (p->acceleration[0] + p->vrandom[0] * ftmp) * (float)cg.frametime * 0.001f;
	p->velocity[1] += (p->acceleration[1] + p->vrandom[1] * ftmp) * (float)cg.frametime * 0.001f;
	p->velocity[2] += (p->acceleration[2] + p->vrandom[2] * ftmp) * (float)cg.frametime * 0.001f;
}

static void AddSpriteParticleToScene(sparticle_t *p) {
	polyVert_t verts[4];

	if (p->radius == 0)
		return;

	verts[0].st[0] = verts[0].st[1] = 0.0f;
	verts[1].st[0] = 0.0f;
	verts[1].st[1] = 1.0f;
	verts[2].st[0] = verts[2].st[1] = 1.0f;
	verts[3].st[0] = 1.0f;
	verts[3].st[1] = 0.0f;

	verts[0].modulate[0] = verts[1].modulate[0] = verts[2].modulate[0] = verts[3].modulate[0] =
		p->currentcolor[0] * 0xff;

	verts[0].modulate[1] = verts[1].modulate[1] = verts[2].modulate[1] = verts[3].modulate[1] =
		p->currentcolor[1] * 0xff;

	verts[0].modulate[2] = verts[1].modulate[2] = verts[2].modulate[2] = verts[3].modulate[2] =
		p->currentcolor[2] * 0xff;

	verts[0].modulate[3] = verts[1].modulate[3] = verts[2].modulate[3] = verts[3].modulate[3] =
		p->currentcolor[3] * 0xff;

	verts[0].xyz[0] = p->origin[0] + p->radius * (-cg.refdef.viewaxis[1][0] + cg.refdef.viewaxis[2][0]);
	verts[0].xyz[1] = p->origin[1] + p->radius * (-cg.refdef.viewaxis[1][1] + cg.refdef.viewaxis[2][1]);
	verts[0].xyz[2] = p->origin[2] + p->radius * (-cg.refdef.viewaxis[1][2] + cg.refdef.viewaxis[2][2]);

	verts[1].xyz[0] = p->origin[0] + p->radius * (-cg.refdef.viewaxis[1][0] - cg.refdef.viewaxis[2][0]);
	verts[1].xyz[1] = p->origin[1] + p->radius * (-cg.refdef.viewaxis[1][1] - cg.refdef.viewaxis[2][1]);
	verts[1].xyz[2] = p->origin[2] + p->radius * (-cg.refdef.viewaxis[1][2] - cg.refdef.viewaxis[2][2]);

	verts[2].xyz[0] = p->origin[0] + p->radius * (cg.refdef.viewaxis[1][0] - cg.refdef.viewaxis[2][0]);
	verts[2].xyz[1] = p->origin[1] + p->radius * (cg.refdef.viewaxis[1][1] - cg.refdef.viewaxis[2][1]);
	verts[2].xyz[2] = p->origin[2] + p->radius * (cg.refdef.viewaxis[1][2] - cg.refdef.viewaxis[2][2]);

	verts[3].xyz[0] = p->origin[0] + p->radius * (cg.refdef.viewaxis[1][0] + cg.refdef.viewaxis[2][0]);
	verts[3].xyz[1] = p->origin[1] + p->radius * (cg.refdef.viewaxis[1][1] + cg.refdef.viewaxis[2][1]);
	verts[3].xyz[2] = p->origin[2] + p->radius * (cg.refdef.viewaxis[1][2] + cg.refdef.viewaxis[2][2]);

	if (p->currentshader)
		trap_R_AddPolyToScene(p->currentshader, 4, verts);
	else
		trap_R_AddPolyToScene(cgs.media.whiteShader, 4, verts);
}

void Main_SpriteParticles(void) {
	sparticle_t *p, *tmp;

	for (p = firstp; p != NULL; p = p->next) {
		if (p->endtime < cg.time) {
			tmp = p;
			p = tmp->prev;
			Free_SpriteParticle(tmp);
			if (!p)
				break;
			continue;
		}
		CheckCurrentStats(p);
		AddSpriteParticleToScene(p);
	}
}
