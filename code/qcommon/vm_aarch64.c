/*
===========================================================================
Copyright (C) 2009 David S. Miller <davem@davemloft.net>
Copyright (C) 2013,2014 SUSE Linux Products GmbH
Copyright (C) 2026 World of Padman

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

AArch64 VM JIT compiler - ported from vm_armv7l.c
*/

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <stddef.h>

#include "vm_local.h"

/* exit() won't be called but use it because it is marked with noreturn */
#define DIE(reason, args...) \
	do { \
		Com_Error(ERR_DROP, "vm_aarch64 compiler error: " reason, ##args); \
		exit(1); \
	} while (0)

/*
 * AArch64 register assignments:
 *
 * X0-X7:   scratch / arguments
 * X8:      scratch (indirect result)
 * X9-X15:  scratch
 * X16-X17: intra-procedure-call scratch
 * X18:     platform register (do not use)
 * X19-X28: callee-saved
 * X29:     frame pointer
 * X30:     link register
 * SP:      stack pointer
 *
 * We use callee-saved registers for VM state:
 */
#define rOPSTACK     19
#define rOPSTACKBASE 20
#define rCODEBASE    21
#define rPSTACK      22
#define rDATABASE    23
#define rDATAMASK    24
#define rINSTPOINTERS 25
#define rPSTACKPTR   26

/*
 * opcode information table
 */
#define opImm0 0x0000
#define opImm1 0x0001
#define opImm4 0x0002

#define opRet0 0x0000
#define opRetI 0x0004
#define opRetF 0x0008
#define opRetIF (opRetI | opRetF)

#define opArg0 0x0000
#define opArgI 0x0010
#define opArgF 0x0020
#define opArgIF (opArgI | opArgF)

#define opArg2I 0x0040
#define opArg2F 0x0080
#define opArg2IF (opArg2I | opArg2F)

static const unsigned char vm_opInfo[256] = {
	[OP_UNDEF]  = opImm0,
	[OP_IGNORE] = opImm0,
	[OP_BREAK]  = opImm0,
	[OP_ENTER]  = opImm4,
	[OP_LEAVE]  = opImm4 | opRet0 | opArgIF,
	[OP_CALL]   = opImm0 | opRetI | opArgI,
	[OP_PUSH]   = opImm0 | opRetIF,
	[OP_POP]    = opImm0 | opRet0 | opArgIF,
	[OP_CONST]  = opImm4 | opRetIF,
	[OP_LOCAL]  = opImm4 | opRetI,
	[OP_JUMP]   = opImm0 | opRet0 | opArgI,

	[OP_EQ]  = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_NE]  = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_LTI] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_LEI] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_GTI] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_GEI] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_LTU] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_LEU] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_GTU] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_GEU] = opImm4 | opRet0 | opArgI | opArg2I,
	[OP_EQF] = opImm4 | opRet0 | opArgF | opArg2F,
	[OP_NEF] = opImm4 | opRet0 | opArgF | opArg2F,
	[OP_LTF] = opImm4 | opRet0 | opArgF | opArg2F,
	[OP_LEF] = opImm4 | opRet0 | opArgF | opArg2F,
	[OP_GTF] = opImm4 | opRet0 | opArgF | opArg2F,
	[OP_GEF] = opImm4 | opRet0 | opArgF | opArg2F,

	[OP_LOAD1]  = opImm0 | opRetI | opArgI,
	[OP_LOAD2]  = opImm0 | opRetI | opArgI,
	[OP_LOAD4]  = opImm0 | opRetIF | opArgI,
	[OP_STORE1] = opImm0 | opRet0 | opArgI | opArg2I,
	[OP_STORE2] = opImm0 | opRet0 | opArgI | opArg2I,
	[OP_STORE4] = opImm0 | opRet0 | opArgIF | opArg2I,
	[OP_ARG]    = opImm1 | opRet0 | opArgIF,
	[OP_BLOCK_COPY] = opImm4 | opRet0 | opArgI | opArg2I,

	[OP_SEX8]  = opImm0 | opRetI | opArgI,
	[OP_SEX16] = opImm0 | opRetI | opArgI,
	[OP_NEGI]  = opImm0 | opRetI | opArgI,
	[OP_ADD]   = opImm0 | opRetI | opArgI | opArg2I,
	[OP_SUB]   = opImm0 | opRetI | opArgI | opArg2I,
	[OP_DIVI]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_DIVU]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_MODI]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_MODU]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_MULI]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_MULU]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_BAND]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_BOR]   = opImm0 | opRetI | opArgI | opArg2I,
	[OP_BXOR]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_BCOM]  = opImm0 | opRetI | opArgI,
	[OP_LSH]   = opImm0 | opRetI | opArgI | opArg2I,
	[OP_RSHI]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_RSHU]  = opImm0 | opRetI | opArgI | opArg2I,
	[OP_NEGF]  = opImm0 | opRetF | opArgF,
	[OP_ADDF]  = opImm0 | opRetF | opArgF | opArg2F,
	[OP_SUBF]  = opImm0 | opRetF | opArgF | opArg2F,
	[OP_DIVF]  = opImm0 | opRetF | opArgF | opArg2F,
	[OP_MULF]  = opImm0 | opRetF | opArgF | opArg2F,
	[OP_CVIF]  = opImm0 | opRetF | opArgI,
	[OP_CVFI]  = opImm0 | opRetI | opArgF,
};

#ifdef DEBUG_VM
#define NOTIMPL(x) \
	do { Com_Error(ERR_DROP, "instruction not implemented: %x", x); } while (0)
#else
#define NOTIMPL(x) \
	do { \
		Com_Printf(S_COLOR_RED "instruction not implemented: %x\n", x); \
		vm->compiled = qfalse; \
		return; \
	} while (0)
#endif

static void VM_Destroy_Compiled(vm_t *vm) {
	if (vm->codeBase) {
		if (munmap(vm->codeBase, vm->codeLength))
			Com_Printf(S_COLOR_RED "Memory unmap failed, possible memory leak\n");
	}
	vm->codeBase = NULL;
}

static void __attribute__((__noreturn__)) ErrJump(unsigned num) {
	Com_Error(ERR_DROP, "program tried to execute code outside VM (%x)", num);
}

static int asmcall(int call, int pstack) {
	vm_t *savedVM = currentVM;
	int i, ret;

	currentVM->programStack = pstack - 4;

	if (sizeof(intptr_t) == sizeof(int)) {
		intptr_t *argPosition = (intptr_t *)((byte *)currentVM->dataBase + pstack + 4);
		argPosition[0] = -1 - call;
		ret = currentVM->systemCall(argPosition);
	} else {
		intptr_t args[MAX_VMSYSCALL_ARGS];
		args[0] = -1 - call;
		int *argPosition = (int *)((byte *)currentVM->dataBase + pstack + 4);
		for (i = 1; i < ARRAY_LEN(args); i++)
			args[i] = argPosition[i];
		ret = currentVM->systemCall(args);
	}

	currentVM = savedVM;
	return ret;
}

/*
 * Instruction encoding helpers for AArch64
 */

static void _emit(vm_t *vm, unsigned int isn, int pass) {
	if (pass)
		memcpy(vm->codeBase + vm->codeLength, &isn, 4);
	vm->codeLength += 4;
}

#define emit(isn) _emit(vm, isn, pass)

/* Move wide immediate into Wd (MOVZ) */
#define A64_MOVZ_W(rd, imm16, shift) \
	(0x52800000 | ((shift/16) << 21) | ((imm16) << 5) | (rd))

/* Move wide with keep into Wd (MOVK) */
#define A64_MOVK_W(rd, imm16, shift) \
	(0x72800000 | ((shift/16) << 21) | ((imm16) << 5) | (rd))

/* Move wide immediate into Xd (MOVZ 64-bit) */
#define A64_MOVZ_X(rd, imm16, shift) \
	(0xD2800000 | ((shift/16) << 21) | ((imm16) << 5) | (rd))

/* Move wide with keep into Xd (MOVK 64-bit) */
#define A64_MOVK_X(rd, imm16, shift) \
	(0xF2800000 | ((shift/16) << 21) | ((imm16) << 5) | (rd))

/* ADD Wd, Wn, Wm */
#define A64_ADD_W(rd, rn, rm) \
	(0x0B000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* ADD Xd, Xn, Xm */
#define A64_ADD_X(rd, rn, rm) \
	(0x8B000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* ADD Wd, Wn, #imm12 */
#define A64_ADD_W_IMM(rd, rn, imm12) \
	(0x11000000 | ((imm12) << 10) | ((rn) << 5) | (rd))

/* ADD Xd, Xn, #imm12 */
#define A64_ADD_X_IMM(rd, rn, imm12) \
	(0x91000000 | ((imm12) << 10) | ((rn) << 5) | (rd))

/* SUB Wd, Wn, Wm */
#define A64_SUB_W(rd, rn, rm) \
	(0x4B000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* SUB Xd, Xn, Xm */
#define A64_SUB_X(rd, rn, rm) \
	(0xCB000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* SUB Wd, Wn, #imm12 */
#define A64_SUB_W_IMM(rd, rn, imm12) \
	(0x51000000 | ((imm12) << 10) | ((rn) << 5) | (rd))

/* SUB Xd, Xn, #imm12 */
#define A64_SUB_X_IMM(rd, rn, imm12) \
	(0xD1000000 | ((imm12) << 10) | ((rn) << 5) | (rd))

/* SUBS (sets flags) Wd, Wn, Wm => CMP is SUBS with Rd=WZR */
#define A64_SUBS_W(rd, rn, rm) \
	(0x6B000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* CMP Wn, Wm */
#define A64_CMP_W(rn, rm) A64_SUBS_W(31, rn, rm)

/* AND Wd, Wn, Wm */
#define A64_AND_W(rd, rn, rm) \
	(0x0A000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* ORR Wd, Wn, Wm */
#define A64_ORR_W(rd, rn, rm) \
	(0x2A000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* EOR Wd, Wn, Wm */
#define A64_EOR_W(rd, rn, rm) \
	(0x4A000000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* MVN Wd, Wm (ORN Wd, WZR, Wm) */
#define A64_MVN_W(rd, rm) \
	(0x2A200000 | ((rm) << 16) | (31 << 5) | (rd))

/* NEG Wd, Wm (SUB Wd, WZR, Wm) */
#define A64_NEG_W(rd, rm) A64_SUB_W(rd, 31, rm)

/* LSL Wd, Wn, Wm (LSLV) */
#define A64_LSL_W(rd, rn, rm) \
	(0x1AC02000 | ((rm) << 16) | ((rn) << 5) | (rd))

/* LSR Wd, Wn, Wm (LSRV) */
#define A64_LSR_W(rd, rn, rm) \
	(0x1AC02400 | ((rm) << 16) | ((rn) << 5) | (rd))

/* ASR Wd, Wn, Wm (ASRV) */
#define A64_ASR_W(rd, rn, rm) \
	(0x1AC02800 | ((rm) << 16) | ((rn) << 5) | (rd))

/* MUL Wd, Wn, Wm (MADD Wd, Wn, Wm, WZR) */
#define A64_MUL_W(rd, rn, rm) \
	(0x1B007C00 | ((rm) << 16) | ((rn) << 5) | (rd))

/* SDIV Wd, Wn, Wm */
#define A64_SDIV_W(rd, rn, rm) \
	(0x1AC00C00 | ((rm) << 16) | ((rn) << 5) | (rd))

/* UDIV Wd, Wn, Wm */
#define A64_UDIV_W(rd, rn, rm) \
	(0x1AC00800 | ((rm) << 16) | ((rn) << 5) | (rd))

/* MSUB Wd, Wn, Wm, Wa  (Wa - Wn*Wm) */
#define A64_MSUB_W(rd, rn, rm, ra) \
	(0x1B008000 | ((rm) << 16) | ((ra) << 10) | ((rn) << 5) | (rd))

/* LDR Wt, [Xn, #imm] (unsigned offset, scaled by 4) */
#define A64_LDR_W_IMM(rt, rn, imm) \
	(0xB9400000 | (((imm)/4) << 10) | ((rn) << 5) | (rt))

/* STR Wt, [Xn, #imm] (unsigned offset, scaled by 4) */
#define A64_STR_W_IMM(rt, rn, imm) \
	(0xB9000000 | (((imm)/4) << 10) | ((rn) << 5) | (rt))

/* LDR Wt, [Xn, Xm] */
#define A64_LDR_W_REG(rt, rn, rm) \
	(0xB8606800 | ((rm) << 16) | ((rn) << 5) | (rt))

/* LDRB Wt, [Xn, Xm] */
#define A64_LDRB_W_REG(rt, rn, rm) \
	(0x38606800 | ((rm) << 16) | ((rn) << 5) | (rt))

/* LDRH Wt, [Xn, Xm] */
#define A64_LDRH_W_REG(rt, rn, rm) \
	(0x78606800 | ((rm) << 16) | ((rn) << 5) | (rt))

/* STR Wt, [Xn, Xm] */
#define A64_STR_W_REG(rt, rn, rm) \
	(0xB8206800 | ((rm) << 16) | ((rn) << 5) | (rt))

/* STRB Wt, [Xn, Xm] */
#define A64_STRB_W_REG(rt, rn, rm) \
	(0x38206800 | ((rm) << 16) | ((rn) << 5) | (rt))

/* STRH Wt, [Xn, Xm] */
#define A64_STRH_W_REG(rt, rn, rm) \
	(0x78206800 | ((rm) << 16) | ((rn) << 5) | (rt))

/* LDR Wt, [Xn], #imm (post-index) */
#define A64_LDR_W_POST(rt, rn, imm) \
	(0xB8400400 | (((imm) & 0x1FF) << 12) | ((rn) << 5) | (rt))

/* STR Wt, [Xn, #imm]! (pre-index) */
#define A64_STR_W_PRE(rt, rn, imm) \
	(0xB8000C00 | (((imm) & 0x1FF) << 12) | ((rn) << 5) | (rt))

/* LDR Xt, [Xn, #imm] (unsigned offset, scaled by 8) */
#define A64_LDR_X_IMM(rt, rn, imm) \
	(0xF9400000 | (((imm)/8) << 10) | ((rn) << 5) | (rt))

/* STP Xt1, Xt2, [Xn, #imm]! (pre-index, scaled by 8) */
#define A64_STP_X_PRE(rt1, rt2, rn, imm) \
	(0xA9800000 | ((((imm)/8) & 0x7F) << 15) | ((rt2) << 10) | ((rn) << 5) | (rt1))

/* LDP Xt1, Xt2, [Xn], #imm (post-index, scaled by 8) */
#define A64_LDP_X_POST(rt1, rt2, rn, imm) \
	(0xA8C00000 | ((((imm)/8) & 0x7F) << 15) | ((rt2) << 10) | ((rn) << 5) | (rt1))

/* STP Xt1, Xt2, [Xn, #imm] (signed offset, scaled by 8) */
#define A64_STP_X(rt1, rt2, rn, imm) \
	(0xA9000000 | ((((imm)/8) & 0x7F) << 15) | ((rt2) << 10) | ((rn) << 5) | (rt1))

/* LDP Xt1, Xt2, [Xn, #imm] (signed offset, scaled by 8) */
#define A64_LDP_X(rt1, rt2, rn, imm) \
	(0xA9400000 | ((((imm)/8) & 0x7F) << 15) | ((rt2) << 10) | ((rn) << 5) | (rt1))

/* MOV Xd, Xm (ORR Xd, XZR, Xm) */
#define A64_MOV_X(rd, rm) \
	(0xAA0003E0 | ((rm) << 16) | (rd))

/* MOV Wd, Wm (ORR Wd, WZR, Wm) */
#define A64_MOV_W(rd, rm) \
	(0x2A0003E0 | ((rm) << 16) | (rd))

/* BL offset (offset in instructions, signed 26-bit) */
#define A64_BL(off) \
	(0x94000000 | ((off) & 0x03FFFFFF))

/* BLR Xn */
#define A64_BLR(rn) \
	(0xD63F0000 | ((rn) << 5))

/* BR Xn */
#define A64_BR(rn) \
	(0xD61F0000 | ((rn) << 5))

/* RET (defaults to X30) */
#define A64_RET \
	(0xD65F03C0)

/* B offset (unconditional branch, offset in instructions) */
#define A64_B(off) \
	(0x14000000 | ((off) & 0x03FFFFFF))

/* B.cond offset (conditional branch, offset in instructions, 19-bit) */
#define A64_BCOND(cond, off) \
	(0x54000000 | ((((off)) & 0x7FFFF) << 5) | (cond))

/* Condition codes */
#define COND_EQ 0x0
#define COND_NE 0x1
#define COND_HS 0x2  /* unsigned >= */
#define COND_LO 0x3  /* unsigned < */
#define COND_MI 0x4
#define COND_PL 0x5
#define COND_HI 0x8  /* unsigned > */
#define COND_LS 0x9  /* unsigned <= */
#define COND_GE 0xA
#define COND_LT 0xB
#define COND_GT 0xC
#define COND_LE 0xD

/* BRK #imm16 */
#define A64_BRK(imm16) \
	(0xD4200000 | ((imm16) << 5))

/* NOP */
#define A64_NOP (0xD503201F)

/* SXTB Wd, Wn (sign extend byte) */
#define A64_SXTB_W(rd, rn) \
	(0x13001C00 | ((rn) << 5) | (rd))

/* SXTH Wd, Wn (sign extend halfword) */
#define A64_SXTH_W(rd, rn) \
	(0x13003C00 | ((rn) << 5) | (rd))

/* FMOV Sd, Wn */
#define A64_FMOV_S_W(sd, wn) \
	(0x1E270000 | ((wn) << 5) | (sd))

/* FMOV Wn, Sd */
#define A64_FMOV_W_S(wd, sn) \
	(0x1E260000 | ((sn) << 5) | (wd))

/* FCMP Sn, Sm */
#define A64_FCMP_S(sn, sm) \
	(0x1E202000 | ((sm) << 16) | ((sn) << 5))

/* FNEG Sd, Sn */
#define A64_FNEG_S(sd, sn) \
	(0x1E214000 | ((sn) << 5) | (sd))

/* FADD Sd, Sn, Sm */
#define A64_FADD_S(sd, sn, sm) \
	(0x1E202800 | ((sm) << 16) | ((sn) << 5) | (sd))

/* FSUB Sd, Sn, Sm */
#define A64_FSUB_S(sd, sn, sm) \
	(0x1E203800 | ((sm) << 16) | ((sn) << 5) | (sd))

/* FMUL Sd, Sn, Sm */
#define A64_FMUL_S(sd, sn, sm) \
	(0x1E200800 | ((sm) << 16) | ((sn) << 5) | (sd))

/* FDIV Sd, Sn, Sm */
#define A64_FDIV_S(sd, sn, sm) \
	(0x1E201800 | ((sm) << 16) | ((sn) << 5) | (sd))

/* SCVTF Sd, Wn (signed int to float) */
#define A64_SCVTF_S_W(sd, wn) \
	(0x1E220000 | ((wn) << 5) | (sd))

/* FCVTZS Wd, Sn (float to signed int, round toward zero) */
#define A64_FCVTZS_W_S(wd, sn) \
	(0x1E380000 | ((sn) << 5) | (wd))

/* LDR St, [Xn, #imm] (unsigned offset, scaled by 4) */
#define A64_LDR_S_IMM(st, rn, imm) \
	(0xBD400000 | (((imm)/4) << 10) | ((rn) << 5) | (st))

/* STR St, [Xn, #imm] (unsigned offset, scaled by 4) */
#define A64_STR_S_IMM(st, rn, imm) \
	(0xBD000000 | (((imm)/4) << 10) | ((rn) << 5) | (st))


/* Load a 32-bit immediate into Wn using MOVZ+MOVK */
static void emit_MOVWi(vm_t *vm, int pass, int reg, unsigned int val) {
	emit(A64_MOVZ_W(reg, val & 0xFFFF, 0));
	if (val > 0xFFFF)
		emit(A64_MOVK_W(reg, (val >> 16) & 0xFFFF, 16));
}

/* Load a 64-bit pointer into Xn using MOVZ+MOVK */
static void emit_MOVXi(vm_t *vm, int pass, int reg, uintptr_t val) {
	emit(A64_MOVZ_X(reg, val & 0xFFFF, 0));
	if (val > 0xFFFF)
		emit(A64_MOVK_X(reg, (val >> 16) & 0xFFFF, 16));
	if (val > 0xFFFFFFFF)
		emit(A64_MOVK_X(reg, (val >> 32) & 0xFFFF, 32));
	if (val > 0xFFFFFFFFFFFF)
		emit(A64_MOVK_X(reg, (val >> 48) & 0xFFFF, 48));
}

static int movxi_insn_count(uintptr_t val) {
	int count = 1;

	if (val > 0xFFFF)
		count++;
	if (val > 0xFFFFFFFF)
		count++;
	if (val > 0xFFFFFFFFFFFF)
		count++;

	return count;
}

/* Compute branch offset in instructions from current codeLength to target */
static int branch_offset(int from, int to) {
	return (to - from) / 4;
}


/*
 * CHECK_JUMP: verify jump target is within instruction range.
 * Uses W0 as the target, clobbers W1, X2.
 */
#define CHECK_JUMP \
	do { \
		emit_MOVWi(vm, pass, 1, (unsigned)vm->instructionCount); \
		emit(A64_CMP_W(0, 1)); \
		/* Skip over the full ErrJump pointer load and call on valid targets. */ \
		emit(A64_BCOND(COND_LO, movxi_insn_count((uintptr_t)ErrJump) + 2)); \
		emit_MOVXi(vm, pass, 2, (uintptr_t)ErrJump); \
		emit(A64_BLR(2)); \
	} while (0)

/*
 * Integer comparison + conditional branch
 */
#define IJ(condition) \
	do { \
		int _target; \
		emit_MOVWi(vm, pass, 0, arg.i); \
		CHECK_JUMP; \
		/* pop two values from opstack */ \
		emit(A64_LDR_W_IMM(1, rOPSTACK, 0));  /* W1 = *opstack (top) */ \
		emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4)); \
		emit(A64_LDR_W_IMM(2, rOPSTACK, 0));  /* W2 = *opstack (second) */ \
		emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4)); \
		emit(A64_CMP_W(2, 1)); \
		_target = pass ? branch_offset(vm->codeLength, vm->instructionPointers[arg.i]) : 0; \
		emit(A64_BCOND(condition, _target)); \
	} while (0)

/*
 * Float comparison + conditional branch
 */
#define FJ(condition) \
	do { \
		int _target; \
		emit_MOVWi(vm, pass, 0, arg.i); \
		CHECK_JUMP; \
		/* pop two floats from opstack */ \
		emit(A64_LDR_S_IMM(1, rOPSTACK, 0));  /* S1 = top */ \
		emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4)); \
		emit(A64_LDR_S_IMM(0, rOPSTACK, 0));  /* S0 = second */ \
		emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4)); \
		emit(A64_FCMP_S(0, 1)); \
		_target = pass ? branch_offset(vm->codeLength, vm->instructionPointers[arg.i]) : 0; \
		emit(A64_BCOND(condition, _target)); \
	} while (0)


/*
=================
VM_Compile
=================
*/
void VM_Compile(vm_t *vm, vmHeader_t *header) {
	unsigned char *code;
	int i_count, pc = 0;
	int pass;
	int codeOffset;

	vm->compiled = qfalse;
	vm->codeBase = NULL;
	vm->codeLength = 0;

	for (pass = 0; pass < 2; ++pass) {

		if (pass) {
			vm->codeBase = mmap(NULL, vm->codeLength, PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
			if (vm->codeBase == MAP_FAILED) {
				Com_Error(ERR_FATAL, "VM_CompileAArch64: can't mmap memory");
			}
			vm->codeLength = 0;
		}

		/*
		 * Entry point: called as entry(vm, &programStack, opStack)
		 *   X0 = vm_t *vm
		 *   X1 = int *programStack
		 *   X2 = int *opStack
		 *
		 * Save callee-saved registers and set up VM state registers.
		 */

		/* Save X19-X25, X29, X30 (LR) */
		emit(A64_STP_X_PRE(29, 30, 31, -96));  /* STP X29,X30,[SP,#-96]! */
		emit(A64_STP_X(19, 20, 31, 16));
		emit(A64_STP_X(21, 22, 31, 32));
		emit(A64_STP_X(23, 24, 31, 48));
		emit(A64_STP_X(25, 26, 31, 64));

		/* Set up VM state from arguments */
		/* rCODEBASE = vm->codeBase */
		emit(A64_LDR_X_IMM(rCODEBASE, 0, offsetof(vm_t, codeBase)));
		/* rDATABASE = vm->dataBase */
		emit(A64_LDR_X_IMM(rDATABASE, 0, offsetof(vm_t, dataBase)));
		/* rDATAMASK (W24) = vm->dataMask */
		emit(A64_LDR_W_IMM(rDATAMASK, 0, offsetof(vm_t, dataMask)));
		/* rINSTPOINTERS = vm->instructionPointers */
		emit(A64_LDR_X_IMM(rINSTPOINTERS, 0, offsetof(vm_t, instructionPointers)));
		/* rPSTACK (W22) = *X1 (programStack value) */
		emit(A64_LDR_W_IMM(rPSTACK, 1, 0));
		/* Preserve the host-side programStack pointer across VM execution. */
		emit(A64_MOV_X(rPSTACKPTR, 1));
		/* rOPSTACK = X2 */
		emit(A64_MOV_X(rOPSTACK, 2));
		/* rOPSTACKBASE = X2 */
		emit(A64_MOV_X(rOPSTACKBASE, 2));

		/* Branch to code start (BL so LR points to return sequence) */
		codeOffset = vm->codeLength;
		emit(A64_BL(0)); /* placeholder, patched in pass 1 */

		/* Return sequence: pop return value from opstack into W0 */
		/* (we branch back here after vmMain returns) */
		int returnOffset = vm->codeLength;
		emit(A64_LDR_W_IMM(0, rOPSTACK, 0));  /* W0 = *opstack */
		emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));

		/* Store programStack back */
		emit(A64_STR_W_IMM(rPSTACK, rPSTACKPTR, 0));

		/* Restore callee-saved and return */
		emit(A64_LDP_X(25, 26, 31, 64));
		emit(A64_LDP_X(23, 24, 31, 48));
		emit(A64_LDP_X(21, 22, 31, 32));
		emit(A64_LDP_X(19, 20, 31, 16));
		emit(A64_LDP_X_POST(29, 30, 31, 96));
		emit(A64_RET);

		/* Patch the branch to code start */
		if (pass) {
			int off = branch_offset(codeOffset, vm->codeLength);
			unsigned int *patch = (unsigned int *)(vm->codeBase + codeOffset);
			*patch = A64_BL(off);
		}


		/* Main instruction compilation loop */
		code = (unsigned char *)header + header->codeOffset;
		pc = 0;

		for (i_count = 0; i_count < header->instructionCount; i_count++) {
			union {
				unsigned char b[4];
				unsigned int i;
			} arg;
			unsigned char op = code[pc++];

			vm->instructionPointers[i_count] = vm->codeLength;

			if (vm_opInfo[op] & opImm4) {
				memcpy(arg.b, &code[pc], 4);
				pc += 4;
			} else if (vm_opInfo[op] & opImm1) {
				arg.b[0] = code[pc];
				arg.b[1] = arg.b[2] = arg.b[3] = 0;
				++pc;
			} else {
				arg.i = 0;
			}

			switch (op) {
			case OP_UNDEF:
			case OP_IGNORE:
				break;

			case OP_BREAK:
				emit(A64_BRK(0));
				break;

			case OP_ENTER:
				/* Save LR, subtract arg from programStack */
				emit(A64_STP_X_PRE(29, 30, 31, -16));
				if (arg.i > 0 && arg.i < 4096) {
					emit(A64_SUB_W_IMM(rPSTACK, rPSTACK, arg.i));
				} else if (arg.i >= 4096) {
					emit_MOVWi(vm, pass, 0, arg.i);
					emit(A64_SUB_W(rPSTACK, rPSTACK, 0));
				}
				break;

			case OP_LEAVE:
				/* Add arg to programStack, return */
				if (arg.i > 0 && arg.i < 4096) {
					emit(A64_ADD_W_IMM(rPSTACK, rPSTACK, arg.i));
				} else if (arg.i >= 4096) {
					emit_MOVWi(vm, pass, 0, arg.i);
					emit(A64_ADD_W(rPSTACK, rPSTACK, 0));
				}
				emit(A64_LDP_X_POST(29, 30, 31, 16));
				emit(A64_RET);
				break;

			case OP_CALL: {
				/*
				 * W0 = *opstack (call target); opstack -= 4
				 * if W0 < 0: syscall
				 * else: jump to instructionPointers[W0]
				 */
				int syscallOffset;
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				/* Check if negative (syscall) */
				emit(0x37F80000 | (0 & 0x1F)); /* TBNZ W0, #31, +N (patched) */
				syscallOffset = vm->codeLength - 4;

				/* Positive: VM call */
				CHECK_JUMP;
				/* X3 = instructionPointers[W0] (offset) */
				/* Use W0 as index, load 32-bit offset, but instructionPointers is intptr_t* */
				/* X3 = rINSTPOINTERS + W0*8 (since intptr_t is 8 bytes on 64-bit) */
				emit(0x8B000000 | (0 << 16) | (rINSTPOINTERS << 5) | 3); /* placeholder - need UXTW+LSL */
				/* Actually: LDR X3, [rINSTPOINTERS, W0, UXTW #3] */
				emit(0xF8605800 | (0 << 16) | (rINSTPOINTERS << 5) | 3); /* LDR X3, [X25, W0, UXTW #3] */
				/* X3 = codeBase + offset */
				emit(A64_ADD_X(3, rCODEBASE, 3));
				emit(A64_BLR(3));
				/* Branch past syscall handling */
				{
					int skipSyscall = vm->codeLength;
					emit(A64_B(0)); /* placeholder */

					/* Patch TBNZ to jump here */
					if (pass) {
						int off = branch_offset(syscallOffset, vm->codeLength);
						unsigned int *p = (unsigned int *)(vm->codeBase + syscallOffset);
						*p = 0x37F80000 | ((off & 0x3FFF) << 5) | (0 & 0x1F);
					}

					/* Syscall: call asmcall(W0, W_pstack) */
					emit(A64_MOV_W(1, rPSTACK));  /* W1 = programStack */
					emit_MOVXi(vm, pass, 2, (uintptr_t)asmcall);
					emit(A64_BLR(2));
					/* Store return value on opstack */
					emit(A64_ADD_X_IMM(rOPSTACK, rOPSTACK, 4));
					emit(A64_STR_W_IMM(0, rOPSTACK, 0));

					/* Patch skip branch */
					if (pass) {
						int off = branch_offset(skipSyscall, vm->codeLength);
						unsigned int *p = (unsigned int *)(vm->codeBase + skipSyscall);
						*p = A64_B(off);
					}
				}
				break;
			}

			case OP_PUSH:
				emit(A64_ADD_X_IMM(rOPSTACK, rOPSTACK, 4));
				break;

			case OP_POP:
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				break;

			case OP_CONST:
				emit_MOVWi(vm, pass, 0, arg.i);
				emit(A64_ADD_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_LOCAL:
				if (arg.i < 4096) {
					emit(A64_ADD_W_IMM(0, rPSTACK, arg.i));
				} else {
					emit_MOVWi(vm, pass, 0, arg.i);
					emit(A64_ADD_W(0, rPSTACK, 0));
				}
				emit(A64_ADD_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_JUMP: {
				/* W0 = *opstack; opstack -= 4; jump to instructionPointers[W0] */
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				CHECK_JUMP;
				emit(0xF8605800 | (0 << 16) | (rINSTPOINTERS << 5) | 3); /* LDR X3, [X25, W0, UXTW #3] */
				emit(A64_ADD_X(3, rCODEBASE, 3));
				emit(A64_BR(3));
				break;
			}

			case OP_EQ:  IJ(COND_EQ); break;
			case OP_NE:  IJ(COND_NE); break;
			case OP_LTI: IJ(COND_LT); break;
			case OP_LEI: IJ(COND_LE); break;
			case OP_GTI: IJ(COND_GT); break;
			case OP_GEI: IJ(COND_GE); break;
			case OP_LTU: IJ(COND_LO); break;
			case OP_LEU: IJ(COND_LS); break;
			case OP_GTU: IJ(COND_HI); break;
			case OP_GEU: IJ(COND_HS); break;

			case OP_EQF: FJ(COND_EQ); break;
			case OP_NEF: FJ(COND_NE); break;
			case OP_LTF: FJ(COND_MI); break;
			case OP_LEF: FJ(COND_LS); break;
			case OP_GTF: FJ(COND_GT); break;
			case OP_GEF: FJ(COND_GE); break;


			case OP_LOAD1:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_AND_W(0, rDATAMASK, 0));
				emit(A64_LDRB_W_REG(0, rDATABASE, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_LOAD2:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_AND_W(0, rDATAMASK, 0));
				emit(A64_LDRH_W_REG(0, rDATABASE, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_LOAD4:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_AND_W(0, rDATAMASK, 0));
				emit(A64_LDR_W_REG(0, rDATABASE, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_STORE1:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));  /* value */
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));  /* address */
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_AND_W(1, rDATAMASK, 1));
				emit(A64_STRB_W_REG(0, rDATABASE, 1));
				break;

			case OP_STORE2:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_AND_W(1, rDATAMASK, 1));
				emit(A64_STRH_W_REG(0, rDATABASE, 1));
				break;

			case OP_STORE4:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_AND_W(1, rDATAMASK, 1));
				emit(A64_STR_W_REG(0, rDATABASE, 1));
				break;

			case OP_ARG:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_ADD_W_IMM(1, rPSTACK, arg.b[0]));
				emit(A64_AND_W(1, rDATAMASK, 1));
				emit(A64_STR_W_REG(0, rDATABASE, 1));
				break;

			case OP_BLOCK_COPY:
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));  /* src */
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));  /* dst */
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit_MOVWi(vm, pass, 2, arg.i);        /* count */
				emit_MOVXi(vm, pass, 3, (uintptr_t)VM_BlockCopy);
				emit(A64_BLR(3));
				break;

			case OP_SEX8:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SXTB_W(0, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_SEX16:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SXTH_W(0, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_NEGI:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_NEG_W(0, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_ADD:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_ADD_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_SUB:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_SUB_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_DIVI:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_SDIV_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_DIVU:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_UDIV_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_MODI:
				/* W1 / W0, remainder = W1 - (W1/W0)*W0 */
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_SDIV_W(2, 1, 0));
				emit(A64_MSUB_W(0, 2, 0, 1));  /* W0 = W1 - W2*W0 */
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_MODU:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_UDIV_W(2, 1, 0));
				emit(A64_MSUB_W(0, 2, 0, 1));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_MULI:
			case OP_MULU:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_MUL_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_BAND:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_AND_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_BOR:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_ORR_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_BXOR:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_EOR_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_BCOM:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_MVN_W(0, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_LSH:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_LSL_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_RSHI:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_ASR_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			case OP_RSHU:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_W_IMM(1, rOPSTACK, 0));
				emit(A64_LSR_W(0, 1, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;


			case OP_NEGF:
				emit(A64_LDR_S_IMM(0, rOPSTACK, 0));
				emit(A64_FNEG_S(0, 0));
				emit(A64_STR_S_IMM(0, rOPSTACK, 0));
				break;

			case OP_ADDF:
				emit(A64_LDR_S_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_S_IMM(1, rOPSTACK, 0));
				emit(A64_FADD_S(0, 1, 0));
				emit(A64_STR_S_IMM(0, rOPSTACK, 0));
				break;

			case OP_SUBF:
				emit(A64_LDR_S_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_S_IMM(1, rOPSTACK, 0));
				emit(A64_FSUB_S(0, 1, 0));
				emit(A64_STR_S_IMM(0, rOPSTACK, 0));
				break;

			case OP_DIVF:
				emit(A64_LDR_S_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_S_IMM(1, rOPSTACK, 0));
				emit(A64_FDIV_S(0, 1, 0));
				emit(A64_STR_S_IMM(0, rOPSTACK, 0));
				break;

			case OP_MULF:
				emit(A64_LDR_S_IMM(0, rOPSTACK, 0));
				emit(A64_SUB_X_IMM(rOPSTACK, rOPSTACK, 4));
				emit(A64_LDR_S_IMM(1, rOPSTACK, 0));
				emit(A64_FMUL_S(0, 1, 0));
				emit(A64_STR_S_IMM(0, rOPSTACK, 0));
				break;

			case OP_CVIF:
				emit(A64_LDR_W_IMM(0, rOPSTACK, 0));
				emit(A64_SCVTF_S_W(0, 0));
				emit(A64_STR_S_IMM(0, rOPSTACK, 0));
				break;

			case OP_CVFI:
				emit(A64_LDR_S_IMM(0, rOPSTACK, 0));
				emit(A64_FCVTZS_W_S(0, 0));
				emit(A64_STR_W_IMM(0, rOPSTACK, 0));
				break;

			default:
				NOTIMPL(op);
				break;
			}
		}

		/* Emit return-to-caller branch (branch back to returnOffset) */
		{
			int off = pass ? branch_offset(vm->codeLength, returnOffset) : 0;
			emit(A64_B(off));
		}

	} /* end pass loop */

	if (mprotect(vm->codeBase, vm->codeLength, PROT_READ | PROT_EXEC)) {
		VM_Destroy_Compiled(vm);
		DIE("mprotect failed");
	}

	/* Clear instruction cache */
	__builtin___clear_cache(vm->codeBase, vm->codeBase + vm->codeLength);

	vm->destroy = VM_Destroy_Compiled;
	vm->compiled = qtrue;
}

/*
=================
VM_CallCompiled
=================
*/
int VM_CallCompiled(vm_t *vm, int *args) {
	byte stack[OPSTACK_SIZE + 15];
	int *opStack;
	int programStack = vm->programStack;
	int stackOnEntry = programStack;
	byte *image = vm->dataBase;
	int *argPointer;
	int retVal;

	currentVM = vm;
	vm->currentlyInterpreting = qtrue;

	programStack -= (8 + 4 * MAX_VMMAIN_ARGS);
	argPointer = (int *)&image[programStack + 8];
	memcpy(argPointer, args, 4 * MAX_VMMAIN_ARGS);
	argPointer[-1] = 0;
	argPointer[-2] = -1;

	opStack = PADP(stack, 16);
	*opStack = 0xDEADBEEF;

	/* Call generated code: entry(vm, &programStack, opStack) */
	{
		int (*entry)(vm_t *, int *, int *);
		entry = (void *)(vm->codeBase);
		retVal = entry(vm, &programStack, opStack);
	}

	if (*opStack != 0xDEADBEEF) {
		Com_Error(ERR_DROP, "opStack corrupted in compiled code");
	}

	if (programStack != stackOnEntry - (8 + 4 * MAX_VMMAIN_ARGS))
		Com_Error(ERR_DROP, "programStack corrupted in compiled code");

	vm->programStack = stackOnEntry;
	vm->currentlyInterpreting = qfalse;

	return retVal;
}
