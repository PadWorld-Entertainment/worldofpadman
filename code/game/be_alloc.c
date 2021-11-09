/*
===========================================================================
Copyright (C) 2010, 2011, 2012 the-brain

This program is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.	If not, see <http://gnu.org/licenses/>.
===========================================================================
*/

#include "g_local.h"

/* Any unlikely to be used value */
#define FREEMEMCOOKIE ((int)0xDEADBE3F)

typedef struct freeMemNode_s {
	/* Size of ROUNDBITS */
	/* Size includes node (obviously) */
	int cookie, size;
	struct freeMemNode_s *prev, *next;
} freeMemNode_t;

/* 512K ought to be enough for anybody */
#define POOL_SIZE (2048 * 1024)
static char memory_pool[POOL_SIZE];
#undef POOL_SIZE
static freeMemNode_t *freeHead;
static int freeMem;

/*
	Find a free block and allocate.
	Does two passes, attempts to fill same-sized free slot first.
*/

/* Round to 32 bytes */
#define ROUNDBITS 31u

void *BE_Alloc(unsigned int size) {
	freeMemNode_t *fmn, *prev, *next, *smallest;
	unsigned int allocSize, smallestSize;
	char *endptr;
	int *ptr;

	/* Round to 32-byte boundary */
	allocSize = ((size + sizeof(int) + ROUNDBITS) & ~ROUNDBITS);
	ptr = NULL;

	smallest = NULL;
	/* Guaranteed not to miss any slots :) */
	smallestSize = (sizeof(memory_pool) + 1);
	for (fmn = freeHead; fmn; fmn = fmn->next) {
		if (fmn->cookie != FREEMEMCOOKIE) {
			G_Error("BE_Alloc: Memory corruption detected!\n");
		}

		if (fmn->size >= allocSize) {
			/* We've got a block */
			if (fmn->size == allocSize) {
				/* Same size, just remove */
				prev = fmn->prev;
				next = fmn->next;
				if (prev) {
					prev->next = next;
				}
				if (next) {
					next->prev = prev;
				}
				if (fmn == freeHead) {
					freeHead = next;
				}
				ptr = (int *)fmn;
				/* Stop the loop, this is fine */
				break;
			} else {
				/* Keep track of the smallest free slot */
				if (fmn->size < smallestSize) {
					smallest = fmn;
					smallestSize = fmn->size;
				}
			}
		}
	}

	if (!ptr && smallest) {
		/* We found a slot big enough */
		smallest->size -= allocSize;
		endptr = ((char *)smallest + smallest->size);
		ptr = (int *)endptr;
	}

	if (ptr) {
		freeMem -= allocSize;
		/* TODO: Move this into BE_CAlloc()? */
		memset(ptr, 0, allocSize);
		/* Store a copy of size for deallocation */
		*ptr++ = allocSize;
		return (void *)ptr;
	}

	G_Error("BE_Alloc: failed on allocation of %i bytes\n", size);
	return NULL;
}

#undef ROUNDBITS

/*
	Release allocated memory, add it to the free list.
*/
void BE_Free(void *ptr) {
	freeMemNode_t *fmn;
	char *freeend;
	int *freeptr;

	G_assert(ptr != NULL);
	G_assert(freeHead != NULL);

	freeptr = ptr;
	freeptr--;

	freeMem += *freeptr;

	for (fmn = freeHead; fmn; fmn = fmn->next) {
		freeend = (((char *)fmn) + fmn->size);
		if (freeend == (char *)freeptr) {
			/* Released block can be merged to an existing node */
			/* Add size of node */
			fmn->size += *freeptr;
			return;
		}
	}

	/* No merging, add to head of list */
	fmn = (freeMemNode_t *)freeptr;
	/* Set this first to avoid corrupting *freeptr */
	fmn->size = *freeptr;
	fmn->cookie = FREEMEMCOOKIE;
	fmn->prev = NULL;
	fmn->next = freeHead;
	freeHead->prev = fmn;
	freeHead = fmn;
}

/*
	Set up the initial node
*/
void BE_InitMemory(void) {
	freeHead = (freeMemNode_t *)memory_pool;
	freeHead->cookie = FREEMEMCOOKIE;
	freeHead->size = sizeof(memory_pool);
	freeHead->next = NULL;
	freeHead->prev = NULL;
	freeMem = sizeof(memory_pool);
}

/*
	If there's a frenzy of deallocation and we want to
	allocate something big, this is useful. Otherwise...
	not much use.
*/
void BE_DefragmentMemory(void) {
	freeMemNode_t *startfmn, *endfmn, *fmn;

	for (startfmn = freeHead; startfmn;) {
		endfmn = (freeMemNode_t *)(((char *)startfmn) + startfmn->size);
		for (fmn = freeHead; fmn;) {
			if (fmn->cookie != FREEMEMCOOKIE) {
				G_Error("BE_DefragmentMemory: Memory corruption detected!\n");
			}

			if (fmn == endfmn) {
				/* We can add fmn onto startfmn. */
				if (fmn->prev) {
					fmn->prev->next = fmn->next;
				}
				if (fmn->next) {
					if (!(fmn->next->prev = fmn->prev)) {
						/* We're removing the head node */
						freeHead = fmn->next;
					}
				}
				startfmn->size += fmn->size;
				/* A redundant call, really. */
				memset(fmn, 0, sizeof(freeMemNode_t));

				startfmn = freeHead;
				/* Break out of current loop */
				endfmn = fmn = NULL;
			} else {
				fmn = fmn->next;
			}
		}

		if (endfmn) {
			/* endfmn acts as a 'restart' flag here */
			startfmn = startfmn->next;
		}
	}
}

/*
	Give a breakdown of memory
*/
void BE_MemoryInfo(void) {
	freeMemNode_t *fmn = (freeMemNode_t *)memory_pool;
	int size, chunks;
	freeMemNode_t *end = (freeMemNode_t *)(memory_pool + sizeof(memory_pool));
	void *p;
	char sizeBuf[128];

	/* NOTE: QVM implementation of printf("%p") does not print "0x" prefix */

	BE_Printf("%p-%p: ", fmn, end);
	ReadableSize(sizeBuf, sizeof(sizeBuf), (sizeof(memory_pool) - freeMem));
	BE_Printf(S_COLOR_BOLD "%s" S_COLOR_DEFAULT " / ", sizeBuf);
	ReadableSize(sizeBuf, sizeof(sizeBuf), sizeof(memory_pool));
	BE_Printf(S_COLOR_BOLD "%s" S_COLOR_DEFAULT ".\n", sizeBuf);

	while (fmn < end) {
		size = chunks = 0;
		p = fmn;
		while ((fmn < end) && (FREEMEMCOOKIE == fmn->cookie)) {
			size += fmn->size;
			chunks++;
			fmn = (freeMemNode_t *)((char *)fmn + fmn->size);
		}
		if (size) {
			ReadableSize(sizeBuf, sizeof(sizeBuf), size);
			Com_Printf("	%p: " S_COLOR_BOLD "%s free" S_COLOR_DEFAULT " (%d chunks)\n", p, sizeBuf, chunks);
		}
		size = chunks = 0;
		p = fmn;
		while ((fmn < end) && (fmn->cookie != FREEMEMCOOKIE)) {
			size += *(int *)fmn;
			chunks++;
			fmn = (freeMemNode_t *)((size_t)fmn + *(int *)fmn);
		}
		if (size) {
			ReadableSize(sizeBuf, sizeof(sizeBuf), size);
			Com_Printf("	%p: " S_COLOR_BOLD "%s allocated" S_COLOR_DEFAULT " (%d chunks)\n", p, sizeBuf, chunks);
		}
	}
}
