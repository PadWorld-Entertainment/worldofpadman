/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "qbsp.h"
#include "l_mem.h"
#include "botlib/aasfile.h" //aas_bbox_t
#include "aas_store.h"		//AAS_MAX_BBOXES
#include "aas_cfg.h"
#include "aas_map.h" //AAS_CreateMapBrushes
#include "l_bsp_q3.h"
#include "cm_patch.h"
#include "qcommon/surfaceflags.h"

#define NODESTACKSIZE 1024

static int Q3_BrushContents(mapbrush_t *b) {
	int contents, i, mixed, hint;
	side_t *s;

	s = &b->original_sides[0];
	contents = s->contents;
	//
	mixed = qfalse;
	hint = qfalse;
	for (i = 1; i < b->numsides; i++) {
		s = &b->original_sides[i];
		if (s->contents != contents)
			mixed = qtrue;
		if (s->surf & (SURF_HINT | SURF_SKIP))
			hint = qtrue;
		contents |= s->contents;
	}
	//
	if (hint) {
		if (contents) {
			Log_Write("WARNING: hint brush with contents: ");
			PrintContents(contents);
			Log_Write("\r\n");
			//
			Log_Write("brush contents is: ");
			PrintContents(b->contents);
			Log_Write("\r\n");
		}
		return 0;
	}
	// Log_Write("brush %d contents ", nummapbrushes);
	// PrintContents(contents);
	// Log_Write("\r\n");
	// remove ladder and fog contents
	contents &= ~(CONTENTS_LADDER | CONTENTS_FOG);
	//
	if (mixed) {
		Log_Write("Entity %i, Brush %i: mixed face contents ", b->entitynum, b->brushnum);
		PrintContents(contents);
		Log_Write("\r\n");
		//
		Log_Write("brush contents is: ");
		PrintContents(b->contents);
		Log_Write("\r\n");
		//
		if (contents & CONTENTS_DONOTENTER)
			return CONTENTS_DONOTENTER; // Log_Print("mixed contents with donotenter\n");
		/*
		Log_Print("contents:"); PrintContents(contents);
		Log_Print("\ncontents:"); PrintContents(s->contents);
		Log_Print("\n");
		Log_Print("texture name = %s\n", texinfo[s->texinfo].texture);
		*/
		// if liquid brush
		if (contents & (CONTENTS_LAVA | CONTENTS_SLIME | CONTENTS_WATER)) {
			return (contents & (CONTENTS_LAVA | CONTENTS_SLIME | CONTENTS_WATER));
		}
		if (contents & CONTENTS_PLAYERCLIP)
			return (contents & CONTENTS_PLAYERCLIP);
		return (contents & CONTENTS_SOLID);
	}
	/*
	if (contents & CONTENTS_AREAPORTAL)
	{
		static int num;
		Log_Write("Entity %i, Brush %i: area portal %d\r\n", b->entitynum, b->brushnum, num++);
	} //end if*/
	if (contents == (contents & CONTENTS_STRUCTURAL)) {
		// Log_Print("brush %i is only structural\n", b->brushnum);
		contents = 0;
	}
	if (contents & CONTENTS_DONOTENTER) {
		Log_Print("brush %i is a donotenter brush, c = %X\n", b->brushnum, contents);
	}
	return contents;
}
#define BBOX_NORMAL_EPSILON 0.0001

static void Q3_DPlanes2MapPlanes(void) {
	int i;

	for (i = 0; i < q3_numplanes; i++) {
		dplanes2mapplanes[i] = FindFloatPlane(q3_dplanes[i].normal, q3_dplanes[i].dist);
	}
}

static void Q3_BSPBrushToMapBrush(q3_dbrush_t *bspbrush, entity_t *mapent) {
	mapbrush_t *b;
	int i, k, n;
	side_t *side, *s2;
	int planenum;
	q3_dbrushside_t *bspbrushside;
	q3_dplane_t *bspplane;

	if (nummapbrushes >= MAX_MAPFILE_BRUSHES)
		Error("nummapbrushes >= MAX_MAPFILE_BRUSHES");

	b = &mapbrushes[nummapbrushes];
	b->original_sides = &brushsides[nummapbrushsides];
	b->entitynum = mapent - entities;
	b->brushnum = nummapbrushes - mapent->firstbrush;
	b->leafnum = dbrushleafnums[bspbrush - q3_dbrushes];

	for (n = 0; n < bspbrush->numSides; n++) {
		// pointer to the bsp brush side
		bspbrushside = &q3_dbrushsides[bspbrush->firstSide + n];

		if (nummapbrushsides >= MAX_MAPFILE_BRUSHSIDES) {
			Error("MAX_MAPFILE_BRUSHSIDES");
		}
		// pointer to the map brush side
		side = &brushsides[nummapbrushsides];
		// if the BSP brush side is textured
		if (q3_dbrushsidetextured[bspbrush->firstSide + n])
			side->flags |= SFL_TEXTURED | SFL_VISIBLE;
		else
			side->flags &= ~SFL_TEXTURED;
		// NOTE: all Quake3 sides are assumed textured
		// side->flags |= SFL_TEXTURED|SFL_VISIBLE;
		//
		if (bspbrushside->shaderNum < 0) {
			side->contents = 0;
			side->surf = 0;
		} else {
			side->contents = q3_dshaders[bspbrushside->shaderNum].contentFlags;
			side->surf = q3_dshaders[bspbrushside->shaderNum].surfaceFlags;
			if (strstr(q3_dshaders[bspbrushside->shaderNum].shader, "common/hint")) {
				side->surf |= SURF_HINT;
			}
		}

		if (side->surf & SURF_NODRAW) {
			side->flags |= SFL_TEXTURED | SFL_VISIBLE;
		}

		// hints and skips are never detail, and have no content
		if (side->surf & (SURF_HINT | SURF_SKIP)) {
			side->contents = 0;
		}

		// ME: get a plane for this side
		bspplane = &q3_dplanes[bspbrushside->planeNum];
		planenum = FindFloatPlane(bspplane->normal, bspplane->dist);
		//
		// see if the plane has been used already
		//
		// ME: this really shouldn't happen!!!
		// ME: otherwise the bsp file is corrupted??
		// ME: still it seems to happen, maybe Johny Boy's
		// ME: brush bevel adding is crappy ?
		for (k = 0; k < b->numsides; k++) {
			s2 = b->original_sides + k;

			if (s2->planenum == planenum) {
				Log_Print("Entity %i, Brush %i: duplicate plane\n", b->entitynum, b->brushnum);
				break;
			}
			if (s2->planenum == (planenum ^ 1)) {
				Log_Print("Entity %i, Brush %i: mirrored plane\n", b->entitynum, b->brushnum);
				break;
			}
		}
		if (k != b->numsides)
			continue; // duplicated

		//
		// keep this side
		//
		// ME: reset pointer to side, why? hell I dunno (pointer is set above already)
		side = b->original_sides + b->numsides;
		// ME: store the plane number
		side->planenum = planenum;
		// ME: texinfo is already stored when bsp is loaded
		// NOTE: check for TEXINFO_NODE, otherwise crash in Q3_BrushContents
		// if (bspbrushside->texinfo < 0) side->texinfo = 0;
		// else side->texinfo = bspbrushside->texinfo;

		// save the td off in case there is an origin brush and we
		// have to recalculate the texinfo
		// ME: don't need to recalculate because it's already done
		//     (for non-world entities) in the BSP file
		//		side_brushtextures[nummapbrushsides] = td;

		nummapbrushsides++;
		b->numsides++;
	}

	// get the content for the entire brush
	b->contents = q3_dshaders[bspbrush->shaderNum].contentFlags;
	b->contents &= ~(CONTENTS_LADDER | CONTENTS_FOG | CONTENTS_STRUCTURAL);

	if (BrushExists(b)) {
		c_squattbrushes++;
		b->numsides = 0;
		return;
	}

	// if we're creating AAS
	if (create_aas) {
		// create the AAS brushes from this brush, don't add brush bevels
		AAS_CreateMapBrushes(b, mapent, qfalse);
		return;
	}

	// allow detail brushes to be removed
	if (nodetail && (b->contents & CONTENTS_DETAIL)) {
		b->numsides = 0;
		return;
	}

	// allow water brushes to be removed
	if (nowater && (b->contents & (CONTENTS_LAVA | CONTENTS_SLIME | CONTENTS_WATER))) {
		b->numsides = 0;
		return;
	}

	// create windings for sides and bounds for brush
	MakeBrushWindings(b);

	// mark brushes without winding or with a tiny window as bevels
	MarkBrushBevels(b);

	// brushes that will not be visible at all will never be
	// used as bsp splitters
	if (b->contents & (CONTENTS_PLAYERCLIP | CONTENTS_MONSTERCLIP)) {
		c_clipbrushes++;
		for (i = 0; i < b->numsides; i++)
			b->original_sides[i].texinfo = TEXINFO_NODE;
	}

	nummapbrushes++;
	mapent->numbrushes++;
}

static void Q3_ParseBSPBrushes(entity_t *mapent) {
	int i;

	for (i = 0; i < q3_dmodels[mapent->modelnum].numBrushes; i++) {
		Q3_BSPBrushToMapBrush(&q3_dbrushes[q3_dmodels[mapent->modelnum].firstBrush + i], mapent);
	}
}

static qboolean Q3_ParseBSPEntity(int entnum) {
	entity_t *mapent;
	const char *model;

	mapent = &entities[entnum]; // num_entities];
	mapent->firstbrush = nummapbrushes;
	mapent->numbrushes = 0;
	mapent->modelnum = -1; //-1 = no BSP model

	model = ValueForKey(mapent, "model");
	if (model && strlen(model)) {
		if (*model == '*') {
			// get the model number of this entity (skip the leading *)
			mapent->modelnum = atoi(&model[1]);
		}
	}

	GetVectorForKey(mapent, "origin", mapent->origin);

	// if this is the world entity it has model number zero
	// the world entity has no model key
	if (!strcmp("worldspawn", ValueForKey(mapent, "classname"))) {
		mapent->modelnum = 0;
	}
	// if the map entity has a BSP model (a modelnum of -1 is used for
	// entities that aren't using a BSP model)
	if (mapent->modelnum >= 0) {
		// parse the bsp brushes
		Q3_ParseBSPBrushes(mapent);
	}

	// the origin of the entity is already taken into account
	//
	// func_group entities can't be in the bsp file
	//
	// check out the func_areaportal entities
	if (!strcmp("func_areaportal", ValueForKey(mapent, "classname"))) {
		c_areaportals++;
		mapent->areaportalnum = c_areaportals;
		return qtrue;
	}
	return qtrue;
}

#define MAX_PATCH_VERTS 1024

static void AAS_CreateCurveBrushes(void) {
	int i, j, n, planenum, numcurvebrushes = 0;
	q3_dsurface_t *surface;
	q3_drawVert_t *dv_p;
	vec3_t points[MAX_PATCH_VERTS];
	int width, height, c;
	patchCollide_t *pc;
	facet_t *facet;
	mapbrush_t *brush;
	side_t *side;
	entity_t *mapent;
	winding_t *winding;

	qprintf("nummapbrushsides = %d\n", nummapbrushsides);
	mapent = &entities[0];
	for (i = 0; i < q3_numDrawSurfaces; i++) {
		surface = &q3_drawSurfaces[i];
		if (!surface->patchWidth)
			continue;
		// if the curve is not solid
		if (!(q3_dshaders[surface->shaderNum].contentFlags & (CONTENTS_SOLID | CONTENTS_PLAYERCLIP))) {
			// Log_Print("skipped non-solid curve\n");
			continue;
		}
		// if this curve should not be used for AAS
		if (q3_dshaders[surface->shaderNum].contentFlags & CONTENTS_NOBOTCLIP) {
			continue;
		}
		//
		width = surface->patchWidth;
		height = surface->patchHeight;
		c = width * height;
		if (c > MAX_PATCH_VERTS) {
			Error("ParseMesh: MAX_PATCH_VERTS");
		}

		dv_p = q3_drawVerts + surface->firstVert;
		for (j = 0; j < c; j++, dv_p++) {
			points[j][0] = dv_p->xyz[0];
			points[j][1] = dv_p->xyz[1];
			points[j][2] = dv_p->xyz[2];
		}
		// create the internal facet structure
		pc = CM_GeneratePatchCollide(width, height, points);
		//
		for (j = 0; j < pc->numFacets; j++) {
			facet = &pc->facets[j];
			//
			brush = &mapbrushes[nummapbrushes];
			brush->original_sides = &brushsides[nummapbrushsides];
			brush->entitynum = 0;
			brush->brushnum = nummapbrushes - mapent->firstbrush;
			//
			brush->numsides = facet->numBorders + 2;
			nummapbrushsides += brush->numsides;
			brush->contents = CONTENTS_SOLID;
			//
			// qprintf("\r%6d curve brushes", nummapbrushsides);//++numcurvebrushes);
			qprintf("\r%6d curve brushes", ++numcurvebrushes);
			//
			planenum = FindFloatPlane(pc->planes[facet->surfacePlane].plane, pc->planes[facet->surfacePlane].plane[3]);
			//
			side = &brush->original_sides[0];
			side->planenum = planenum;
			side->contents = CONTENTS_SOLID;
			side->flags |= SFL_TEXTURED | SFL_VISIBLE | SFL_CURVE;
			side->surf = 0;
			//
			side = &brush->original_sides[1];
			if (create_aas) {
				// the plane is expanded later so it's not a problem that
				// these first two opposite sides are coplanar
				side->planenum = planenum ^ 1;
			} else {
				side->planenum = FindFloatPlane(mapplanes[planenum ^ 1].normal, mapplanes[planenum ^ 1].dist + 1);
				side->flags |= SFL_TEXTURED | SFL_VISIBLE;
			}
			side->contents = CONTENTS_SOLID;
			side->flags |= SFL_CURVE;
			side->surf = 0;
			//
			winding = BaseWindingForPlane(mapplanes[side->planenum].normal, mapplanes[side->planenum].dist);
			for (n = 0; n < facet->numBorders; n++) {
				// never use the surface plane as a border
				if (facet->borderPlanes[n] == facet->surfacePlane)
					continue;
				//
				side = &brush->original_sides[2 + n];
				side->planenum = FindFloatPlane(pc->planes[facet->borderPlanes[n]].plane,
												pc->planes[facet->borderPlanes[n]].plane[3]);
				if (facet->borderInward[n])
					side->planenum ^= 1;
				side->contents = CONTENTS_SOLID;
				side->flags |= SFL_TEXTURED | SFL_CURVE;
				side->surf = 0;
				// chop the winding in place
				if (winding)
					ChopWindingInPlace(&winding, mapplanes[side->planenum ^ 1].normal,
									   mapplanes[side->planenum ^ 1].dist, 0.1); // CLIP_EPSILON);
			}
			// VectorCopy(pc->bounds[0], brush->mins);
			// VectorCopy(pc->bounds[1], brush->maxs);
			if (!winding) {
				Log_Print("WARNING: AAS_CreateCurveBrushes: no winding\n");
				brush->numsides = 0;
				continue;
			}
			brush->original_sides[0].winding = winding;
			WindingBounds(winding, brush->mins, brush->maxs);
			for (n = 0; n < 3; n++) {
				// IDBUG: all the indexes into the mins and maxs were zero (not using i)
				if (brush->mins[n] < -MAX_MAP_BOUNDS || brush->maxs[n] > MAX_MAP_BOUNDS) {
					Log_Print("entity %i, brush %i: bounds out of range\n", brush->entitynum, brush->brushnum);
					Log_Print("brush->mins[%d] = %f, brush->maxs[%d] = %f\n", n, brush->mins[n], n, brush->maxs[n]);
					brush->numsides = 0; // remove the brush
					break;
				}
				if (brush->mins[n] > MAX_MAP_BOUNDS || brush->maxs[n] < -MAX_MAP_BOUNDS) {
					Log_Print("entity %i, brush %i: no visible sides on brush\n", brush->entitynum, brush->brushnum);
					Log_Print("brush->mins[%d] = %f, brush->maxs[%d] = %f\n", n, brush->mins[n], n, brush->maxs[n]);
					brush->numsides = 0; // remove the brush
					break;
				}
			}
			if (create_aas) {
				// NOTE: brush bevels now already added
				// AddBrushBevels(brush);
				AAS_CreateMapBrushes(brush, mapent, qfalse);
			} else {
				// create windings for sides and bounds for brush
				MakeBrushWindings(brush);
				AddBrushBevels(brush);
				nummapbrushes++;
				mapent->numbrushes++;
			}
		}
	}
	// qprintf("\r%6d curve brushes", nummapbrushsides);//++numcurvebrushes);
	qprintf("\r%6d curve brushes\n", numcurvebrushes);
}

void AAS_ExpandMapBrush(mapbrush_t *brush, vec3_t mins, vec3_t maxs);

void Q3_LoadMapFromBSP(struct quakefile_s *qf) {
	int i;

	Log_Print("-- Q3_LoadMapFromBSP --\n");
	// loaded map type
	loadedmaptype = MAPTYPE_QUAKE3;

	Log_Print("Loading map from %s...\n", qf->filename);
	// load the bsp file
	Q3_LoadBSPFile(qf);

	// create an index from bsp planes to map planes
	// DPlanes2MapPlanes();
	// clear brush model numbers
	for (i = 0; i < MAX_MAPFILE_BRUSHES; i++)
		brushmodelnumbers[i] = -1;

	nummapbrushsides = 0;
	num_entities = 0;

	Q3_ParseEntities();
	//
	for (i = 0; i < num_entities; i++) {
		Q3_ParseBSPEntity(i);
	}

	AAS_CreateCurveBrushes();
	// get the map mins and maxs from the world model
	ClearBounds(map_mins, map_maxs);
	for (i = 0; i < entities[0].numbrushes; i++) {
		if (mapbrushes[i].numsides <= 0)
			continue;
		AddPointToBounds(mapbrushes[i].mins, map_mins, map_maxs);
		AddPointToBounds(mapbrushes[i].maxs, map_mins, map_maxs);
	}
	/*/
	for (i = 0; i < nummapbrushes; i++)
	{
		//if (!mapbrushes[i].original_sides) continue;
		//AddBrushBevels(&mapbrushes[i]);
		//AAS_ExpandMapBrush(&mapbrushes[i], mins, maxs);
	} //end for*/
	/*
	for (i = 0; i < nummapbrushsides; i++)
	{
		Log_Write("side %d flags = %d", i, brushsides[i].flags);
	}
	for (i = 0; i < nummapbrushes; i++)
	{
		Log_Write("brush contents: ");
		PrintContents(mapbrushes[i].contents);
		Log_Print("\n");
	} //end for*/
}

void Q3_ResetMapLoading(void) {
	// reset for map loading from bsp
	memset(nodestack, 0, NODESTACKSIZE * sizeof(int));
	nodestackptr = NULL;
	nodestacksize = 0;
	memset(brushmodelnumbers, 0, MAX_MAPFILE_BRUSHES * sizeof(int));
}
