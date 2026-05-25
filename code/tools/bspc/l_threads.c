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
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "l_cmd.h"
#include "l_threads.h"
#include "l_log.h"
#include "l_mem.h"

#define MAX_THREADS 64

// #define THREAD_DEBUG

int dispatch;
int workcount;
int oldf;
qboolean pacifier;
qboolean threaded;
void (*workfunction)(int);

int GetThreadWork(void) {
	int r;
	int f;

	ThreadLock();

	if (dispatch == workcount) {
		ThreadUnlock();
		return -1;
	}

	f = 10 * dispatch / workcount;
	if (f != oldf) {
		oldf = f;
		if (pacifier)
			printf("%i...", f);
	}

	r = dispatch;
	dispatch++;
	ThreadUnlock();

	return r;
}

static void ThreadWorkerFunction(int threadnum) {
	int work;

	while (1) {
		work = GetThreadWork();
		if (work == -1)
			break;
		workfunction(work);
	}
}

void RunThreadsOnIndividual(int workcnt, qboolean showpacifier, void (*func)(int)) {
	if (numthreads == -1)
		ThreadSetDefault();
	workfunction = func;
	RunThreadsOn(workcnt, showpacifier, ThreadWorkerFunction);
}

//===================================================================
//
// WIN32
//
//===================================================================

#if defined(WIN32) || defined(_WIN32)

#define USED

#include <windows.h>

typedef struct thread_s {
	HANDLE handle;
	int threadid;
	int id;
	struct thread_s *next;
} thread_t;

thread_t *firstthread;
thread_t *lastthread;
int currentnumthreads;
int currentthreadid;

int numthreads = 1;
CRITICAL_SECTION crit;
HANDLE semaphore;
static int enter;
static int numwaitingthreads = 0;

void ThreadSetDefault(void) {
	SYSTEM_INFO info;

	if (numthreads == -1) // not set manually
	{
		GetSystemInfo(&info);
		numthreads = info.dwNumberOfProcessors;
		if (numthreads < 1 || numthreads > 32)
			numthreads = 1;
	}
	qprintf("%i threads\n", numthreads);
}

void ThreadLock(void) {
	if (!threaded) {
		Error("ThreadLock: !threaded");
		return;
	}
	EnterCriticalSection(&crit);
	if (enter)
		Error("Recursive ThreadLock\n");
	enter = 1;
}

void ThreadUnlock(void) {
	if (!threaded) {
		Error("ThreadUnlock: !threaded");
		return;
	}
	if (!enter)
		Error("ThreadUnlock without lock\n");
	enter = 0;
	LeaveCriticalSection(&crit);
}

void ThreadSetupLock(void) {
	Log_Print("Win32 multi-threading\n");
	InitializeCriticalSection(&crit);
	threaded = qtrue;
	currentnumthreads = 0;
	currentthreadid = 0;
}

void ThreadShutdownLock(void) {
	DeleteCriticalSection(&crit);
	threaded = qfalse;
}

void ThreadSetupSemaphore(void) {
	semaphore = CreateSemaphore(NULL, 0, 99999999, "bspc");
}

void ThreadShutdownSemaphore(void) {
}

void ThreadSemaphoreWait(void) {
	WaitForSingleObject(semaphore, INFINITE);
}

void ThreadSemaphoreIncrease(int count) {
	ReleaseSemaphore(semaphore, count, NULL);
}

void RunThreadsOn(int workcnt, qboolean showpacifier, void (*func)(int)) {
	int threadid[MAX_THREADS];
	HANDLE threadhandle[MAX_THREADS];
	int i;
	int start, end;

	Log_Print("Win32 multi-threading\n");
	start = I_FloatTime();
	dispatch = 0;
	workcount = workcnt;
	oldf = -1;
	pacifier = showpacifier;
	threaded = qtrue;

	if (numthreads == -1)
		ThreadSetDefault();

	if (numthreads < 1 || numthreads > MAX_THREADS)
		numthreads = 1;
	//
	// run threads in parallel
	//
	InitializeCriticalSection(&crit);

	numwaitingthreads = 0;

	if (numthreads == 1) { // use same thread
		func(0);
	} else {
		for (i = 0; i < numthreads; i++) {
			threadhandle[i] = CreateThread(NULL,						 // LPSECURITY_ATTRIBUTES lpsa,
										   0,							 // DWORD cbStack,
										   (LPTHREAD_START_ROUTINE)func, // LPTHREAD_START_ROUTINE lpStartAddr,
										   (LPVOID)i,					 // LPVOID lpvThreadParm,
										   0,							 //   DWORD fdwCreate,
										   &threadid[i]);
		}

		for (i = 0; i < numthreads; i++)
			WaitForSingleObject(threadhandle[i], INFINITE);
	}
	DeleteCriticalSection(&crit);

	threaded = qfalse;
	end = I_FloatTime();
	if (pacifier)
		printf(" (%i)\n", end - start);
}

void AddThread(void (*func)(int)) {
	thread_t *thread;

	if (numthreads == 1) {
		if (currentnumthreads >= numthreads)
			return;
		currentnumthreads++;
		func(-1);
		currentnumthreads--;
	} else {
		ThreadLock();
		if (currentnumthreads >= numthreads) {
			ThreadUnlock();
			return;
		}
		thread = GetMemory(sizeof(thread_t));
		if (!thread)
			Error("can't allocate memory for thread\n");

		thread->threadid = currentthreadid;
		thread->handle = CreateThread(NULL,							// LPSECURITY_ATTRIBUTES lpsa,
									  0,							// DWORD cbStack,
									  (LPTHREAD_START_ROUTINE)func, // LPTHREAD_START_ROUTINE lpStartAddr,
									  (LPVOID)thread->threadid,		// LPVOID lpvThreadParm,
									  0,							// DWORD fdwCreate,
									  &thread->id);

		thread->next = NULL;
		if (lastthread)
			lastthread->next = thread;
		else
			firstthread = thread;
		lastthread = thread;
#ifdef THREAD_DEBUG
		qprintf("added thread with id %d\n", thread->threadid);
#endif
		currentnumthreads++;
		currentthreadid++;
		ThreadUnlock();
	}
}

void RemoveThread(int threadid) {
	thread_t *thread, *last;

	if (threadid == -1)
		return;

	ThreadLock();
	last = NULL;
	for (thread = firstthread; thread; thread = thread->next) {
		if (thread->threadid == threadid) {
			if (last)
				last->next = thread->next;
			else
				firstthread = thread->next;
			if (!thread->next)
				lastthread = last;
			FreeMemory(thread);
			currentnumthreads--;
#ifdef THREAD_DEBUG
			qprintf("removed thread with id %d\n", threadid);
#endif
			break;
		}
		last = thread;
	}
	if (!thread)
		Error("couldn't find thread with id %d", threadid);
	ThreadUnlock();
}

void WaitForAllThreadsFinished(void) {
	HANDLE handle;

	ThreadLock();
	while (firstthread) {
		handle = firstthread->handle;
		ThreadUnlock();

		WaitForSingleObject(handle, INFINITE);

		ThreadLock();
	}
	ThreadUnlock();
}

int GetNumThreads(void) {
	return currentnumthreads;
}

#endif // WIN32

//===================================================================
//
// POSIX (Linux, macOS)
//
//===================================================================

#if defined(__linux__) || defined(__APPLE__)

#define USED

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

#define pthread_startroutine_t void *(*)(void *)

typedef struct thread_s {
	pthread_t thread;
	int threadid;
	int id;
	struct thread_s *next;
} thread_t;

thread_t *firstthread;
thread_t *lastthread;
int currentnumthreads;
int currentthreadid;

int numthreads = 1;
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_attr_t attrib;
sem_t semaphore;
static int enter;

void ThreadSetDefault(void) {
	if (numthreads == -1) // not set manually
	{
		numthreads = 1;
	}
	qprintf("%i threads\n", numthreads);
}

void ThreadLock(void) {
	if (!threaded) {
		Error("ThreadLock: !threaded");
		return;
	}
	pthread_mutex_lock(&my_mutex);
	if (enter)
		Error("Recursive ThreadLock\n");
	enter = 1;
}

void ThreadUnlock(void) {
	if (!threaded) {
		Error("ThreadUnlock: !threaded");
		return;
	}
	if (!enter)
		Error("ThreadUnlock without lock\n");
	enter = 0;
	pthread_mutex_unlock(&my_mutex);
}

void ThreadSetupLock(void) {
	Log_Print("pthread multi-threading\n");

	threaded = qtrue;
	currentnumthreads = 0;
	currentthreadid = 0;
}

void ThreadShutdownLock(void) {
	threaded = qfalse;
}

void ThreadSetupSemaphore(void) {
	sem_init(&semaphore, 0, 0);
}

void ThreadShutdownSemaphore(void) {
	sem_destroy(&semaphore);
}

void ThreadSemaphoreWait(void) {
	sem_wait(&semaphore);
}

void ThreadSemaphoreIncrease(int count) {
	int i;

	for (i = 0; i < count; i++) {
		sem_post(&semaphore);
	}
}

void RunThreadsOn(int workcnt, qboolean showpacifier, void (*func)(int)) {
	int i;
	pthread_t work_threads[MAX_THREADS];
	void *pthread_return;
	int start, end;

	Log_Print("pthread multi-threading\n");

	start = I_FloatTime();
	dispatch = 0;
	workcount = workcnt;
	oldf = -1;
	pacifier = showpacifier;
	threaded = qtrue;

	if (numthreads < 1 || numthreads > MAX_THREADS)
		numthreads = 1;

	if (pacifier)
		setbuf(stdout, NULL);

	for (i = 0; i < numthreads; i++) {
		if (pthread_create(&work_threads[i], NULL, (pthread_startroutine_t)func, (void *)(uintptr_t)i) == -1) {
			Error("pthread_create failed");
		}
	}

	for (i = 0; i < numthreads; i++) {
		if (pthread_join(work_threads[i], &pthread_return) == -1)
			Error("pthread_join failed");
	}

	threaded = qfalse;

	end = I_FloatTime();
	if (pacifier)
		printf(" (%i)\n", end - start);
}

void AddThread(void (*func)(int)) {
	thread_t *thread;

	if (numthreads == 1) {
		if (currentnumthreads >= numthreads)
			return;
		currentnumthreads++;
		func(-1);
		currentnumthreads--;
	} else {
		ThreadLock();
		if (currentnumthreads >= numthreads) {
			ThreadUnlock();
			return;
		}
		thread = GetMemory(sizeof(thread_t));
		if (!thread)
			Error("can't allocate memory for thread\n");

		thread->threadid = currentthreadid;

		if (pthread_create(&thread->thread, NULL, (pthread_startroutine_t)func, (void *)(uintptr_t)thread->threadid) ==
			-1) {
			Error("pthread_create failed");
		}

		thread->next = NULL;
		if (lastthread)
			lastthread->next = thread;
		else
			firstthread = thread;
		lastthread = thread;
#ifdef THREAD_DEBUG
		qprintf("added thread with id %d\n", thread->threadid);
#endif
		currentnumthreads++;
		currentthreadid++;
		ThreadUnlock();
	}
}

void RemoveThread(int threadid) {
	thread_t *thread, *last;

	if (threadid == -1)
		return;

	ThreadLock();
	last = NULL;
	for (thread = firstthread; thread; thread = thread->next) {
		if (thread->threadid == threadid) {
			if (last)
				last->next = thread->next;
			else
				firstthread = thread->next;
			if (!thread->next)
				lastthread = last;
			FreeMemory(thread);
			currentnumthreads--;
#ifdef THREAD_DEBUG
			qprintf("removed thread with id %d\n", threadid);
#endif
			break;
		}
		last = thread;
	}
	if (!thread)
		Error("couldn't find thread with id %d", threadid);
	ThreadUnlock();
}

void WaitForAllThreadsFinished(void) {
	pthread_t *thread;
	void *pthread_return;

	ThreadLock();
	while (firstthread) {
		thread = &firstthread->thread;
		ThreadUnlock();

		if (pthread_join(*thread, &pthread_return) == -1)
			Error("pthread_join failed");

		ThreadLock();
	}
	ThreadUnlock();
}

int GetNumThreads(void) {
	return currentnumthreads;
}

#endif // __linux__ || __APPLE__

//===================================================================
//
// SINGLE THREAD (fallback)
//
//===================================================================

#ifndef USED

#error "Unsupported platform for threading"

#endif // USED
