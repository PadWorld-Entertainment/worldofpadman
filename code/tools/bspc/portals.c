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

static int c_active_portals;
static int c_peak_portals;
static int c_boundary;
static int c_boundary_sides;
static int c_portalmemory;

portal_t *AllocPortal(void) {
	portal_t *p;

	p = GetMemory(sizeof(portal_t));
	memset(p, 0, sizeof(portal_t));

	if (numthreads == 1) {
		c_active_portals++;
		if (c_active_portals > c_peak_portals) {
			c_peak_portals = c_active_portals;
		}
		c_portalmemory += MemorySize(p);
	}

	return p;
}

void FreePortal(portal_t *p) {
	if (p->winding)
		FreeWinding(p->winding);
	if (numthreads == 1) {
		c_active_portals--;
		c_portalmemory -= MemorySize(p);
	}
	FreeMemory(p);
}

static int ClusterContents(node_t *node) {
	int c1, c2, c;

	if (node->planenum == PLANENUM_LEAF)
		return node->contents;

	c1 = ClusterContents(node->children[0]);
	c2 = ClusterContents(node->children[1]);
	c = c1 | c2;

	// a cluster may include some solid detail areas, but
	// still be seen into
	if (!(c1 & CONTENTS_SOLID) || !(c2 & CONTENTS_SOLID))
		c &= ~CONTENTS_SOLID;
	return c;
}

//===========================================================================
// The entity flood determines which areas are
// "outside" on the map, which are then filled in.
// Flowing from side s to side !s
//===========================================================================
static qboolean Portal_EntityFlood(portal_t *p, int s) {
	if (p->nodes[0]->planenum != PLANENUM_LEAF || p->nodes[1]->planenum != PLANENUM_LEAF)
		Error("Portal_EntityFlood: not a leaf");

	// can never cross to a solid
	if ((p->nodes[0]->contents & CONTENTS_SOLID) || (p->nodes[1]->contents & CONTENTS_SOLID))
		return qfalse;

	// can flood through everything else
	return qtrue;
}

//=============================================================================

static int c_tinyportals;

static void AddPortalToNodes(portal_t *p, node_t *front, node_t *back) {
	if (p->nodes[0] || p->nodes[1])
		Error("AddPortalToNode: allready included");

	p->nodes[0] = front;
	p->next[0] = front->portals;
	front->portals = p;

	p->nodes[1] = back;
	p->next[1] = back->portals;
	back->portals = p;
}

void RemovePortalFromNode(portal_t *portal, node_t *l) {
	portal_t **pp, *t;

	int s, i, n;
	portal_t *p;
	portal_t *portals[4096];

	// remove reference to the current portal
	pp = &l->portals;
	while (1) {
		t = *pp;
		if (!t)
			Error("RemovePortalFromNode: portal not in leaf");

		if (t == portal)
			break;

		if (t->nodes[0] == l)
			pp = &t->next[0];
		else if (t->nodes[1] == l)
			pp = &t->next[1];
		else
			Error("RemovePortalFromNode: portal not bounding leaf");
	}

	if (portal->nodes[0] == l) {
		*pp = portal->next[0];
		portal->nodes[0] = NULL;
	} else if (portal->nodes[1] == l) {
		*pp = portal->next[1];
		portal->nodes[1] = NULL;
	} else {
		Error("RemovePortalFromNode: mislinked portal");
	}
	// #ifdef ME
	n = 0;
	for (p = l->portals; p; p = p->next[s]) {
		for (i = 0; i < n; i++) {
			if (p == portals[i])
				Error("RemovePortalFromNode: circular linked\n");
		}
		if (p->nodes[0] != l && p->nodes[1] != l) {
			Error("RemovePortalFromNodes: portal does not belong to node\n");
		}
		portals[n] = p;
		s = (p->nodes[1] == l);
		//		if (++n >= 4096) Error("RemovePortalFromNode: more than 4096 portals\n");
	}
	// #endif
}

static void PrintPortal(portal_t *p) {
	int i;
	winding_t *w;

	w = p->winding;
	for (i = 0; i < w->numpoints; i++)
		printf("(%5.0f,%5.0f,%5.0f)\n", w->p[i][0], w->p[i][1], w->p[i][2]);
}

#define SIDESPACE 8
//===========================================================================
// The created portals will face the global outside_node
//===========================================================================
static void MakeHeadnodePortals(tree_t *tree) {
	vec3_t bounds[2];
	int i, j, n;
	portal_t *p, *portals[6];
	plane_t bplanes[6], *pl;
	node_t *node;

	node = tree->headnode;

	// pad with some space so there will never be null volume leaves
	for (i = 0; i < 3; i++) {
		bounds[0][i] = tree->mins[i] - SIDESPACE;
		bounds[1][i] = tree->maxs[i] + SIDESPACE;
		if (bounds[0][i] > bounds[1][i]) {
			Error("empty BSP tree");
		}
	}

	tree->outside_node.planenum = PLANENUM_LEAF;
	tree->outside_node.brushlist = NULL;
	tree->outside_node.portals = NULL;
	tree->outside_node.contents = 0;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 2; j++) {
			n = j * 3 + i;

			p = AllocPortal();
			portals[n] = p;

			pl = &bplanes[n];
			memset(pl, 0, sizeof(*pl));
			if (j) {
				pl->normal[i] = -1;
				pl->dist = -bounds[j][i];
			} else {
				pl->normal[i] = 1;
				pl->dist = bounds[j][i];
			}
			p->plane = *pl;
			p->winding = BaseWindingForPlane(pl->normal, pl->dist);
			AddPortalToNodes(p, node, &tree->outside_node);
		}

	// clip the basewindings by all the other planes
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 6; j++) {
			if (j == i)
				continue;
			ChopWindingInPlace(&portals[i]->winding, bplanes[j].normal, bplanes[j].dist, ON_EPSILON);
		}
	}
}

#define BASE_WINDING_EPSILON 0.001
#define SPLIT_WINDING_EPSILON 0.001

static winding_t *BaseWindingForNode(node_t *node) {
	winding_t *w;
	node_t *n;
	plane_t *plane;
	vec3_t normal;
	vec_t dist;

	w = BaseWindingForPlane(mapplanes[node->planenum].normal, mapplanes[node->planenum].dist);

	// clip by all the parents
	for (n = node->parent; n && w;) {
		plane = &mapplanes[n->planenum];

		if (n->children[0] == node) { // take front
			ChopWindingInPlace(&w, plane->normal, plane->dist, BASE_WINDING_EPSILON);
		} else { // take back
			VectorSubtract(vec3_origin, plane->normal, normal);
			dist = -plane->dist;
			ChopWindingInPlace(&w, normal, dist, BASE_WINDING_EPSILON);
		}
		node = n;
		n = n->parent;
	}

	return w;
}

//===========================================================================
// create the new portal by taking the full plane winding for the cutting
// plane and clipping it by all of parents of this node
//===========================================================================
static void MakeNodePortal(node_t *node) {
	portal_t *new_portal, *p;
	winding_t *w;
	vec3_t normal;
	float dist;
	int side;

	w = BaseWindingForNode(node);

	// clip the portal by all the other portals in the node
	for (p = node->portals; p && w; p = p->next[side]) {
		if (p->nodes[0] == node) {
			side = 0;
			VectorCopy(p->plane.normal, normal);
			dist = p->plane.dist;
		} else if (p->nodes[1] == node) {
			side = 1;
			VectorSubtract(vec3_origin, p->plane.normal, normal);
			dist = -p->plane.dist;
		} else {
			Error("MakeNodePortal: mislinked portal");
		}
		ChopWindingInPlace(&w, normal, dist, 0.1);
	}

	if (!w) {
		return;
	}

	if (WindingIsTiny(w)) {
		c_tinyportals++;
		FreeWinding(w);
		return;
	}

	new_portal = AllocPortal();
	new_portal->plane = mapplanes[node->planenum];

#ifdef ME
	new_portal->planenum = node->planenum;
#endif // ME

	new_portal->onnode = node;
	new_portal->winding = w;
	AddPortalToNodes(new_portal, node->children[0], node->children[1]);
}

//===========================================================================
// Move or split the portals that bound node so that the node's
// children have portals instead of node.
//===========================================================================
static void SplitNodePortals(node_t *node) {
	portal_t *p, *next_portal, *new_portal;
	node_t *f, *b, *other_node;
	int side;
	plane_t *plane;
	winding_t *frontwinding, *backwinding;

	plane = &mapplanes[node->planenum];
	f = node->children[0];
	b = node->children[1];

	for (p = node->portals; p; p = next_portal) {
		if (p->nodes[0] == node)
			side = 0;
		else if (p->nodes[1] == node)
			side = 1;
		else
			Error("SplitNodePortals: mislinked portal");
		next_portal = p->next[side];

		other_node = p->nodes[!side];
		RemovePortalFromNode(p, p->nodes[0]);
		RemovePortalFromNode(p, p->nodes[1]);

		//
		// cut the portal into two portals, one on each side of the cut plane
		//
		ClipWindingEpsilon(p->winding, plane->normal, plane->dist, SPLIT_WINDING_EPSILON, &frontwinding, &backwinding);

		if (frontwinding && WindingIsTiny(frontwinding)) {
			FreeWinding(frontwinding);
			frontwinding = NULL;
			c_tinyportals++;
		}

		if (backwinding && WindingIsTiny(backwinding)) {
			FreeWinding(backwinding);
			backwinding = NULL;
			c_tinyportals++;
		}

		if (!frontwinding && !backwinding) { // tiny windings on both sides
			continue;
		}

		if (!frontwinding) {
			FreeWinding(backwinding);
			if (side == 0)
				AddPortalToNodes(p, b, other_node);
			else
				AddPortalToNodes(p, other_node, b);
			continue;
		}
		if (!backwinding) {
			FreeWinding(frontwinding);
			if (side == 0)
				AddPortalToNodes(p, f, other_node);
			else
				AddPortalToNodes(p, other_node, f);
			continue;
		}

		// the winding is split
		new_portal = AllocPortal();
		*new_portal = *p;
		new_portal->winding = backwinding;
		FreeWinding(p->winding);
		p->winding = frontwinding;

		if (side == 0) {
			AddPortalToNodes(p, f, other_node);
			AddPortalToNodes(new_portal, b, other_node);
		} else {
			AddPortalToNodes(p, other_node, f);
			AddPortalToNodes(new_portal, other_node, b);
		}
	}

	node->portals = NULL;
}

static void CalcNodeBounds(node_t *node) {
	portal_t *p;
	int s;
	int i;

	// calc mins/maxs for both leaves and nodes
	ClearBounds(node->mins, node->maxs);
	for (p = node->portals; p; p = p->next[s]) {
		s = (p->nodes[1] == node);
		for (i = 0; i < p->winding->numpoints; i++)
			AddPointToBounds(p->winding->p[i], node->mins, node->maxs);
	}
}

static int c_numportalizednodes;

static void MakeTreePortals_r(node_t *node) {
	int i;

#ifdef ME
	qprintf("\r%6d", ++c_numportalizednodes);
	if (cancelconversion)
		return;
#endif // ME

	CalcNodeBounds(node);
	if (node->mins[0] >= node->maxs[0]) {
		Log_Print("WARNING: node without a volume\n");
	}

	for (i = 0; i < 3; i++) {
		if (node->mins[i] < -MAX_MAP_BOUNDS || node->maxs[i] > MAX_MAP_BOUNDS) {
			Log_Print("WARNING: node with unbounded volume\n");
			break;
		}
	}
	if (node->planenum == PLANENUM_LEAF)
		return;

	MakeNodePortal(node);
	SplitNodePortals(node);

	MakeTreePortals_r(node->children[0]);
	MakeTreePortals_r(node->children[1]);
}

void MakeTreePortals(tree_t *tree) {

#ifdef ME
	Log_Print("---- Node Portalization ----\n");
	c_numportalizednodes = 0;
	c_portalmemory = 0;
	qprintf("%6d nodes portalized", c_numportalizednodes);
#endif // ME

	MakeHeadnodePortals(tree);
	MakeTreePortals_r(tree->headnode);

#ifdef ME
	qprintf("\n");
	Log_Write("%6d nodes portalized\r\n", c_numportalizednodes);
	Log_Print("%6d tiny portals\r\n", c_tinyportals);
	Log_Print("%6d KB of portal memory\r\n", c_portalmemory >> 10);
	Log_Print("%6i KB of winding memory\r\n", WindingMemory() >> 10);
#endif // ME
}

/*
=========================================================

FLOOD ENTITIES

=========================================================
*/

node_t *p_firstnode;
node_t *p_lastnode;

// add the node to the end of the node list
static void P_AddNodeToList(node_t *node) {
	node->next = NULL;
	if (p_lastnode)
		p_lastnode->next = node;
	else
		p_firstnode = node;
	p_lastnode = node;
}

//===========================================================================
// get the first node from the front of the node list
//===========================================================================
static node_t *P_NextNodeFromList(void) {
	node_t *node;

	node = p_firstnode;
	if (p_firstnode)
		p_firstnode = p_firstnode->next;
	if (!p_firstnode)
		p_lastnode = NULL;
	return node;
}

static void FloodPortals(node_t *firstnode) {
	node_t *node;
	portal_t *p;
	int s;

	firstnode->occupied = 1;
	P_AddNodeToList(firstnode);

	for (node = P_NextNodeFromList(); node; node = P_NextNodeFromList()) {
		for (p = node->portals; p; p = p->next[s]) {
			s = (p->nodes[1] == node);
			// if the node at the other side of the portal is occupied already
			if (p->nodes[!s]->occupied)
				continue;
			// if it isn't possible to flood through this portal
			if (!Portal_EntityFlood(p, s))
				continue;

			p->nodes[!s]->occupied = node->occupied + 1;

			P_AddNodeToList(p->nodes[!s]);
		}
	}
}

int numrec;

static void FloodPortals_r(node_t *node, int dist) {
	portal_t *p;
	int s;

	Log_Print("\r%6d", ++numrec);

	if (node->occupied)
		Error("FloodPortals_r: node already occupied\n");
	if (!node) {
		Error("FloodPortals_r: NULL node\n");
	}
	node->occupied = dist;

	for (p = node->portals; p; p = p->next[s]) {
		s = (p->nodes[1] == node);
		// if the node at the other side of the portal is occupied already
		if (p->nodes[!s]->occupied)
			continue;
		// if it isn't possible to flood through this portal
		if (!Portal_EntityFlood(p, s))
			continue;
		// flood recursively through the current portal
		FloodPortals_r(p->nodes[!s], dist + 1);
	}
	Log_Print("\r%6d", --numrec);
}

static qboolean PlaceOccupant(node_t *headnode, vec3_t origin, entity_t *occupant) {
	node_t *node;
	vec_t d;
	plane_t *plane;

	// find the leaf to start in
	node = headnode;
	while (node->planenum != PLANENUM_LEAF) {
		if (node->planenum < 0 || node->planenum > nummapplanes) {
			Error("PlaceOccupant: invalid node->planenum\n");
		}
		plane = &mapplanes[node->planenum];
		d = DotProduct(origin, plane->normal) - plane->dist;
		if (d >= 0)
			node = node->children[0];
		else
			node = node->children[1];
		if (!node) {
			Error("PlaceOccupant: invalid child %d\n", d < 0);
		}
	}
	// don't start in solid
	//	if (node->contents == CONTENTS_SOLID)
	// ME: replaced because in LeafNode in brushbsp.c
	//    some nodes have contents solid with other contents
	if (node->contents & CONTENTS_SOLID)
		return qfalse;
	// if the node is already occupied
	if (node->occupied)
		return qfalse;

	// place the occupant in the first leaf
	node->occupant = occupant;

	numrec = 0;
	FloodPortals(node);

	return qtrue;
}

//===========================================================================
// Marks all nodes that can be reached by entites
//===========================================================================
qboolean FloodEntities(tree_t *tree) {
	int i;
	int x, y;
	vec3_t origin;
	const char *cl;
	qboolean inside;
	node_t *headnode;

	headnode = tree->headnode;
	Log_Print("------ FloodEntities -------\n");
	inside = qfalse;
	tree->outside_node.occupied = 0;

	// start at entity 1 not the world ( = 0)
	for (i = 1; i < num_entities; i++) {
		GetVectorForKey(&entities[i], "origin", origin);
		if (VectorCompare(origin, vec3_origin))
			continue;

		cl = ValueForKey(&entities[i], "classname");
		origin[2] += 1; // so objects on floor are ok

		//		Log_Print("flooding from entity %d: %s\n", i, cl);
		// nudge playerstart around if needed so clipping hulls allways
		// have a valid point
		if (!strcmp(cl, "info_player_start") || !strcmp(cl, "info_player_deathmatch") || !strcmp(cl, "team_redspawn") ||
			!strcmp(cl, "team_bluespawn") || !strcmp(cl, "team_redplayer") || !strcmp(cl, "team_blueplayer")) {
			for (x = -16; x <= 16; x += 16) {
				for (y = -16; y <= 16; y += 16) {
					origin[0] += x;
					origin[1] += y;
					if (PlaceOccupant(headnode, origin, &entities[i])) {
						inside = qtrue;
						x = 999; // stop for this info_player_start
						break;
					}
					origin[0] -= x;
					origin[1] -= y;
				}
			}
		} else {
			if (PlaceOccupant(headnode, origin, &entities[i])) {
				inside = qtrue;
			}
		}
	}

	if (!inside) {
		Log_Print("WARNING: no entities inside\n");
	} else if (tree->outside_node.occupied) {
		Log_Print("WARNING: entity reached from outside\n");
	}

	return (qboolean)(inside && !tree->outside_node.occupied);
}

/*
=========================================================

FILL OUTSIDE

=========================================================
*/

static int c_outside;
static int c_inside;
static int c_solid;

static void FillOutside_r(node_t *node) {
	if (node->planenum != PLANENUM_LEAF) {
		FillOutside_r(node->children[0]);
		FillOutside_r(node->children[1]);
		return;
	}
	// anything not reachable by an entity
	// can be filled away (by setting solid contents)
	if (!node->occupied) {
		if (!(node->contents & CONTENTS_SOLID)) {
			c_outside++;
			node->contents |= CONTENTS_SOLID;
		} else {
			c_solid++;
		}
	} else {
		c_inside++;
	}
}

//===========================================================================
// Fill all nodes that can't be reached by entities
//===========================================================================
void FillOutside(node_t *headnode) {
	c_outside = 0;
	c_inside = 0;
	c_solid = 0;
	Log_Print("------- FillOutside --------\n");
	FillOutside_r(headnode);
	Log_Print("%5i solid leaves\n", c_solid);
	Log_Print("%5i leaves filled\n", c_outside);
	Log_Print("%5i inside leaves\n", c_inside);
}
