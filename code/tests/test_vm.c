/*
 * Q3 VM unit test - validates interpreter and compiled VM paths
 * for the current architecture by loading a real QVM built with q3lcc/q3asm.
 *
 * Uses fopen to load the QVM file directly, bypassing the engine filesystem.
 */
#include "test_shared.h"
#include "testqvm2/test_commands.h"
#include "vm_local.h"
#include <stdio.h>

TESTS_GLOBALS();

#define TEST_OPCODE_COUNT (OP_CVFI + 1)

static intptr_t lastReportedValue;

static intptr_t testSyscall(intptr_t *parms) {
	lastReportedValue = parms[1];
	return 0;
}

#ifndef TEST_QVM_PATH
#define TEST_QVM_PATH "testqvm.qvm"
#endif

#ifndef TEST_QVM2_PATH
#define TEST_QVM2_PATH "testqvm2.qvm"
#endif

#ifndef TEST_QVM2_ASM_PATH
#define TEST_QVM2_ASM_PATH "test_main2.asm"
#endif

static byte *loadQVMFile(const char *path, int *outSize) {
	FILE *f = fopen(path, "rb");
	long size;
	byte *data;

	if (!f) {
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = (byte *)malloc(size);
	if (!data) {
		fclose(f);
		return NULL;
	}

	if ((long)fread(data, 1, size, f) != size) {
		free(data);
		fclose(f);
		return NULL;
	}

	fclose(f);
	*outSize = (int)size;
	return data;
}

static char *loadTextFile(const char *path) {
	FILE *f = fopen(path, "rb");
	long size;
	char *data;

	if (!f) {
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = (char *)malloc(size + 1);
	if (!data) {
		fclose(f);
		return NULL;
	}

	if ((long)fread(data, 1, size, f) != size) {
		free(data);
		fclose(f);
		return NULL;
	}

	data[size] = '\0';
	fclose(f);
	return data;
}

static qboolean swapQVMHeader(vmHeader_t *header) {
	int i;

	if (LittleLong(header->vmMagic) == VM_MAGIC_VER2) {
		for (i = 0; i < (int)sizeof(vmHeader_t) / 4; i++) {
			((int *)header)[i] = LittleLong(((int *)header)[i]);
		}
		return qtrue;
	}

	if (LittleLong(header->vmMagic) == VM_MAGIC) {
		for (i = 0; i < (int)(sizeof(vmHeader_t) - sizeof(int)) / 4; i++) {
			((int *)header)[i] = LittleLong(((int *)header)[i]);
		}
		return qtrue;
	}

	return qfalse;
}

static int opcodeImmediateSize(int opcode) {
	switch (opcode) {
	case OP_ENTER:
	case OP_LEAVE:
	case OP_CONST:
	case OP_LOCAL:
	case OP_EQ:
	case OP_NE:
	case OP_LTI:
	case OP_LEI:
	case OP_GTI:
	case OP_GEI:
	case OP_LTU:
	case OP_LEU:
	case OP_GTU:
	case OP_GEU:
	case OP_EQF:
	case OP_NEF:
	case OP_LTF:
	case OP_LEF:
	case OP_GTF:
	case OP_GEF:
	case OP_BLOCK_COPY:
		return 4;
	case OP_ARG:
		return 1;
	default:
		return 0;
	}
}

static void collectOpcodeCoverage(const char *path, qboolean *seen) {
	int fileSize;
	byte *fileData = loadQVMFile(path, &fileSize);
	vmHeader_t *header;
	const byte *code;
	int pc;

	ASSERT_EQ_INT(1, fileData != NULL);

	header = (vmHeader_t *)fileData;
	ASSERT_EQ_INT(1, swapQVMHeader(header));

	code = fileData + header->codeOffset;
	pc = 0;

	while (pc < header->codeLength) {
		int opcode = code[pc++];
		int immSize = opcodeImmediateSize(opcode);

		if (opcode >= 0 && opcode < TEST_OPCODE_COUNT) {
			seen[opcode] = qtrue;
		}
		pc += immSize;
	}

	EXPECT_EQ_INT(header->codeLength, pc);
	free(fileData);
}

static vm_t *setupVM(const char *path, vmInterpret_t interpret) {
	static vm_t vm;
	int fileSize, i;
	byte *fileData = loadQVMFile(path, &fileSize);
	vmHeader_t *header;
	int dataLength;

	if (!fileData) {
		return NULL;
	}

	header = (vmHeader_t *)fileData;
	if (!swapQVMHeader(header)) {
		free(fileData);
		return NULL;
	}

	memset(&vm, 0, sizeof(vm));
	Q_strncpyz(vm.name, "testqvm", sizeof(vm.name));
	vm.systemCall = testSyscall;

	dataLength = header->dataLength + header->litLength + header->bssLength;
	for (i = 0; dataLength > (1 << i); i++) {
	}
	dataLength = 1 << i;

	vm.dataAlloc = dataLength + 4;
	vm.dataBase = (byte *)Hunk_Alloc(vm.dataAlloc, h_high);
	vm.dataMask = dataLength - 1;

	memcpy(vm.dataBase, fileData + header->dataOffset, header->dataLength + header->litLength);
	for (i = 0; i < header->dataLength; i += 4) {
		*(int *)(vm.dataBase + i) = LittleLong(*(int *)(vm.dataBase + i));
	}

	if (header->vmMagic == VM_MAGIC_VER2) {
		header->jtrgLength &= ~0x03;
		vm.numJumpTableTargets = header->jtrgLength >> 2;
		vm.jumpTableTargets = (byte *)Hunk_Alloc(header->jtrgLength, h_high);
		memcpy(vm.jumpTableTargets,
			   fileData + header->dataOffset + header->dataLength + header->litLength,
			   header->jtrgLength);
		for (i = 0; i < header->jtrgLength; i += 4) {
			*(int *)(vm.jumpTableTargets + i) = LittleLong(*(int *)(vm.jumpTableTargets + i));
		}
	}

	vm.instructionCount = header->instructionCount;
	vm.instructionPointers = (intptr_t *)Hunk_Alloc(vm.instructionCount * sizeof(*vm.instructionPointers), h_high);
	vm.codeLength = header->codeLength;
	vm.compiled = qfalse;

#ifndef NO_VM_COMPILED
	if (interpret == VMI_COMPILED) {
		VM_Compile(&vm, header);
	}
#endif
	if (!vm.compiled) {
		VM_PrepareInterpreter(&vm, header);
	}

	vm.programStack = vm.dataMask + 1;
	vm.stackBottom = vm.programStack - PROGRAM_STACK_SIZE;

	free(fileData);
	return &vm;
}

static int runVMCommand(vm_t *vm, int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
	return (int)VM_Call(vm, command, arg0, arg1, arg2, arg3, arg4, arg5, 0, 0, 0, 0, 0);
}

static int expectedBasic(int a, int b) {
	return a + b;
}

static int expectedIntegerCase(int a, int b, int c, int d, int e, int f) {
	unsigned ua = (unsigned)a;
	unsigned ub = (unsigned)b;
	unsigned uc = (unsigned)c;
	unsigned ud = (unsigned)d;
	int result = 0;

	if (a == b) result += 11;
	if (a != b) result += 13;
	if (a < b) result += 17;
	if (a <= b) result += 19;
	if (a > b) result += 23;
	if (a >= b) result += 29;

	if (ua < ub) result += 31;
	if (ua <= ub) result += 37;
	if (ua > ub) result += 41;
	if (ua >= ub) result += 43;

	result += -a;
	result += a + b;
	result += a - b;
	result += a * b;
	result += (int)(ua * ub);
	result += c / d;
	result += c % d;
	result += (int)(uc / ud);
	result += (int)(uc % ud);
	result += a & b;
	result += c | d;
	result += e ^ f;
	result += ~a;
	result += a << 2;
	result += b >> 1;
	result += (int)(uc >> 3);
	result += 6;

	return result;
}

static int expectedFloatCase(int a, int b) {
	float x = (float)a / 4.0f;
	float y = (float)b / 5.0f;
	float z = -x;
	int result;

	z += x + y;
	z += x - y;
	z += x * y;
	z += x / y;

	result = (int)z;

	if (x == (float)a / 4.0f) result += 101;
	if (x != y) result += 103;
	if (x < y) result += 107;
	if (x <= y) result += 109;
	if (x > y) result += 113;
	if (x >= y) result += 127;

	return result;
}

static int expectedMemoryCase(int a, int b, int c, int d, int e) {
	signed char signedByte = (signed char)a;
	unsigned char unsignedByte = (unsigned char)b;
	short signedShort = (short)c;
	unsigned short unsignedShort = (unsigned short)d;
	int result = (int)signedByte;

	result += (int)unsignedByte;
	result += (int)signedShort;
	result += (int)unsignedShort;
	result += e;
	result += 7;
	result += 1 + 2 + 3;

	return result;
}

static void verifyVMExecution(vmInterpret_t interpret) {
	vm_t *vm = setupVM(TEST_QVM2_PATH, interpret);
	int result;
	int expected;

	ASSERT_EQ_INT(1, vm != NULL);

	lastReportedValue = 0;
	result = runVMCommand(vm, TEST_VM_CMD_BASIC, 3, 4, 0, 0, 0, 0);
	expected = expectedBasic(3, 4);
	EXPECT_EQ_INT(expected, result);
	EXPECT_EQ_INT(expected, (int)lastReportedValue);

	lastReportedValue = 0;
	result = runVMCommand(vm, TEST_VM_CMD_INT_OPS, 7, 3, 20, 6, 12, 5);
	expected = expectedIntegerCase(7, 3, 20, 6, 12, 5);
	EXPECT_EQ_INT(expected, result);
	EXPECT_EQ_INT(expected, (int)lastReportedValue);

	lastReportedValue = 0;
	result = runVMCommand(vm, TEST_VM_CMD_FLOAT_OPS, 8, 5, 0, 0, 0, 0);
	expected = expectedFloatCase(8, 5);
	EXPECT_EQ_INT(expected, result);
	EXPECT_EQ_INT(expected, (int)lastReportedValue);

	lastReportedValue = 0;
	result = runVMCommand(vm, TEST_VM_CMD_MEMORY_OPS, -5, 250, -1234, 43210, 99, 0);
	expected = expectedMemoryCase(-5, 250, -1234, 43210, 99);
	EXPECT_EQ_INT(expected, result);
	EXPECT_EQ_INT(expected, (int)lastReportedValue);
}

static void testVMOpcodeCoverage(void) {
	qboolean seen[TEST_OPCODE_COUNT];
	char *asmText;
	int opcode;

	memset(seen, 0, sizeof(seen));
	collectOpcodeCoverage(TEST_QVM2_PATH, seen);
	asmText = loadTextFile(TEST_QVM2_ASM_PATH);
	ASSERT_EQ_INT(1, asmText != NULL);

	for (opcode = 0; opcode < TEST_OPCODE_COUNT; ++opcode) {
		if (opcode == OP_UNDEF || opcode == OP_IGNORE) {
			continue;
		}
		EXPECT_EQ_INT(1, seen[opcode]);
	}

	EXPECT_EQ_INT(1, strstr(asmText, "INDIRB") != NULL);
	free(asmText);
}

static void verifySimpleVM(vmInterpret_t interpret) {
	vm_t *vm = setupVM(TEST_QVM_PATH, interpret);
	int result;

	ASSERT_EQ_INT(1, vm != NULL);

	lastReportedValue = 0;
	result = (int)VM_Call(vm, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	EXPECT_EQ_INT(7, result);
	EXPECT_EQ_INT(7, (int)lastReportedValue);
}

static void testVMSimpleInterpreter(void) {
	verifySimpleVM(VMI_BYTECODE);
}

#ifndef NO_VM_COMPILED
static void testVMSimpleCompiled(void) {
	verifySimpleVM(VMI_COMPILED);
}
#endif

static void testVMExtendedInterpreter(void) {
	verifyVMExecution(VMI_BYTECODE);
}

#ifndef NO_VM_COMPILED
static void testVMExtendedCompiled(void) {
	verifyVMExecution(VMI_COMPILED);
}
#endif

int main(int argc, char *argv[]) {
	TESTS_INIT();

	ADD_TEST(testVMSimpleInterpreter);
#ifndef NO_VM_COMPILED
	ADD_TEST(testVMSimpleCompiled);
#endif
	ADD_TEST(testVMOpcodeCoverage);
	ADD_TEST(testVMExtendedInterpreter);
#ifndef NO_VM_COMPILED
	ADD_TEST(testVMExtendedCompiled);
#endif

	Com_Shutdown();
	TESTS_SHUTDOWN();
}
