/*
 * Q3 VM unit test - validates interpreter and compiled VM paths
 * for the current architecture.
 *
 * Constructs a minimal QVM bytecode program in memory that performs
 * arithmetic (adds two constants) and returns the result.
 */
#include "test_shared.h"
#include "vm_local.h"

TESTS_GLOBALS();

static intptr_t lastSyscallArg;

static intptr_t testSyscall(intptr_t *parms) {
	lastSyscallArg = parms[1];
	return 0;
}

/*
 * Build a minimal QVM bytecode and manually prepare a vm_t for the interpreter.
 * The program: vmMain(command) { syscall(-1, 3 + 4); return 42; }
 *
 * We bypass VM_PrepareInterpreter/VM_Compile to avoid Hunk_Alloc dependencies
 * and instead expand the bytecode to int-aligned codeBase directly.
 */
static qboolean setupTestVM(vm_t *vm, vmInterpret_t interpret) {
	int dataLength, i;

	/* Raw bytecode stream (opcode + optional operand bytes) */
	byte code[64];
	int codeLen = 0;
	int instructionCount = 0;

#define EMIT_OP(op) (code[codeLen++] = (byte)(op), instructionCount++)
#define EMIT_OP4(op, val) do { \
	code[codeLen++] = (byte)(op); \
	code[codeLen++] = (byte)((val) & 0xFF); \
	code[codeLen++] = (byte)(((val) >> 8) & 0xFF); \
	code[codeLen++] = (byte)(((val) >> 16) & 0xFF); \
	code[codeLen++] = (byte)(((val) >> 24) & 0xFF); \
	instructionCount++; \
} while (0)
#define EMIT_OP1(op, val) do { \
	code[codeLen++] = (byte)(op); \
	code[codeLen++] = (byte)(val); \
	instructionCount++; \
} while (0)

	EMIT_OP4(OP_ENTER, 68);
	EMIT_OP4(OP_CONST, 3);
	EMIT_OP4(OP_CONST, 4);
	EMIT_OP(OP_ADD);
	EMIT_OP1(OP_ARG, 8);
	EMIT_OP4(OP_CONST, -1);
	EMIT_OP(OP_CALL);
	EMIT_OP(OP_POP);
	EMIT_OP4(OP_CONST, 42);
	EMIT_OP4(OP_LEAVE, 68);

#undef EMIT_OP
#undef EMIT_OP4
#undef EMIT_OP1

	memset(vm, 0, sizeof(*vm));
	Q_strncpyz(vm->name, "test", sizeof(vm->name));
	vm->systemCall = testSyscall;

	/* Data region (power of 2, minimum for stack) */
	dataLength = PROGRAM_STACK_SIZE;
	vm->dataAlloc = dataLength + 4;
	vm->dataBase = calloc(1, vm->dataAlloc);
	if (!vm->dataBase) return qfalse;
	vm->dataMask = dataLength - 1;

	vm->instructionCount = instructionCount;
	vm->instructionPointers = calloc(instructionCount, sizeof(*vm->instructionPointers));
	if (!vm->instructionPointers) { free(vm->dataBase); return qfalse; }

	vm->codeLength = codeLen;
	vm->compiled = qfalse;

	if (interpret == VMI_COMPILED) {
#ifndef NO_VM_COMPILED
		/* For compiled path, build a vmHeader_t and use VM_Compile which uses Hunk_Alloc */
		int totalSize = sizeof(vmHeader_t) + codeLen;
		vmHeader_t *header = calloc(1, totalSize);
		header->vmMagic = VM_MAGIC;
		header->instructionCount = instructionCount;
		header->codeOffset = sizeof(vmHeader_t);
		header->codeLength = codeLen;
		header->dataOffset = sizeof(vmHeader_t) + codeLen;
		header->dataLength = 0;
		header->litLength = 0;
		header->bssLength = 0;
		memcpy((byte *)header + header->codeOffset, code, codeLen);
		VM_Compile(vm, header);
		free(header);
		if (!vm->compiled) {
			free(vm->dataBase);
			free(vm->instructionPointers);
			return qfalse;
		}
#else
		free(vm->dataBase);
		free(vm->instructionPointers);
		return qfalse;
#endif
	} else {
		/* Interpreter path: expand bytecode to int-aligned codeBase manually */
		int *codeBase = calloc(codeLen * 2, sizeof(int)); /* worst case: each byte becomes an int */
		if (!codeBase) { free(vm->dataBase); free(vm->instructionPointers); return qfalse; }
		vm->codeBase = (byte *)codeBase;

		int byte_pc = 0, int_pc = 0, instr = 0;
		while (instr < instructionCount) {
			vm->instructionPointers[instr] = int_pc;
			instr++;
			int op = code[byte_pc++];
			codeBase[int_pc++] = op;
			switch (op) {
			case OP_ENTER: case OP_CONST: case OP_LOCAL: case OP_LEAVE:
			case OP_EQ: case OP_NE: case OP_LTI: case OP_LEI:
			case OP_GTI: case OP_GEI: case OP_LTU: case OP_LEU:
			case OP_GTU: case OP_GEU: case OP_EQF: case OP_NEF:
			case OP_LTF: case OP_LEF: case OP_GTF: case OP_GEF:
			case OP_BLOCK_COPY:
				codeBase[int_pc] = code[byte_pc] | (code[byte_pc+1]<<8) |
					(code[byte_pc+2]<<16) | (code[byte_pc+3]<<24);
				byte_pc += 4; int_pc++;
				break;
			case OP_ARG:
				codeBase[int_pc] = code[byte_pc++];
				int_pc++;
				break;
			default:
				break;
			}
		}
		vm->codeLength = int_pc;
	}

	vm->programStack = vm->dataMask + 1;
	vm->stackBottom = vm->programStack - PROGRAM_STACK_SIZE;

	return qtrue;
}

static void freeTestVM(vm_t *vm) {
	if (vm->dataBase) free(vm->dataBase);
	if (vm->instructionPointers) free(vm->instructionPointers);
	if (!vm->compiled && vm->codeBase) free(vm->codeBase);
}

static void testVMInterpreter(void) {
	vm_t vm;
	ASSERT_EQ_INT(qtrue, setupTestVM(&vm, VMI_BYTECODE));

	lastSyscallArg = 0;
	currentVM = &vm;
	int args[MAX_VMMAIN_ARGS] = {0};
	int result = VM_CallInterpreted(&vm, args);

	EXPECT_EQ_INT(42, result);
	EXPECT_EQ_INT(7, (int)lastSyscallArg);

	freeTestVM(&vm);
}

#ifndef NO_VM_COMPILED
static void testVMCompiled(void) {
	vm_t vm;
	if (!setupTestVM(&vm, VMI_COMPILED)) {
		return; /* arch doesn't support compiled VM */
	}

	lastSyscallArg = 0;
	currentVM = &vm;
	int args[MAX_VMMAIN_ARGS] = {0};
	int result = VM_CallCompiled(&vm, args);

	EXPECT_EQ_INT(42, result);
	EXPECT_EQ_INT(7, (int)lastSyscallArg);

	freeTestVM(&vm);
}
#endif

int main(int argc, char *argv[]) {
	TESTS_INIT();

	ADD_TEST(testVMInterpreter);
#ifndef NO_VM_COMPILED
	ADD_TEST(testVMCompiled);
#endif

	Com_Shutdown();

	TESTS_SHUTDOWN();
}
