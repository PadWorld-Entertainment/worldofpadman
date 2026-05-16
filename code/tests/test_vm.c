/*
 * Q3 VM unit test - validates interpreter and compiled VM paths
 * for the current architecture by loading a real QVM built with q3lcc/q3asm.
 *
 * Uses fopen to load the QVM file directly, bypassing the engine filesystem.
 */
#include "test_shared.h"
#include "vm_local.h"
#include <stdio.h>

TESTS_GLOBALS();

static intptr_t lastReportedValue;

static intptr_t testSyscall(intptr_t *parms) {
	lastReportedValue = parms[1];
	return 0;
}

#ifndef TEST_QVM_PATH
#define TEST_QVM_PATH "testqvm.qvm"
#endif

static byte *loadQVMFile(const char *path, int *outSize) {
	FILE *f = fopen(path, "rb");
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);
	byte *data = (byte *)malloc(size);
	if (!data) { fclose(f); return NULL; }
	if ((long)fread(data, 1, size, f) != size) { free(data); fclose(f); return NULL; }
	fclose(f);
	*outSize = (int)size;
	return data;
}

static vm_t *setupVM(const char *path, vmInterpret_t interpret) {
	static vm_t vm;
	int fileSize, i;
	byte *fileData = loadQVMFile(path, &fileSize);
	if (!fileData) return NULL;

	vmHeader_t *header = (vmHeader_t *)fileData;

	/* Byte swap header */
	if (LittleLong(header->vmMagic) == VM_MAGIC_VER2) {
		for (i = 0; i < (int)sizeof(vmHeader_t) / 4; i++)
			((int *)header)[i] = LittleLong(((int *)header)[i]);
	} else if (LittleLong(header->vmMagic) == VM_MAGIC) {
		for (i = 0; i < (int)(sizeof(vmHeader_t) - sizeof(int)) / 4; i++)
			((int *)header)[i] = LittleLong(((int *)header)[i]);
	} else {
		free(fileData); return NULL;
	}

	memset(&vm, 0, sizeof(vm));
	Q_strncpyz(vm.name, "testqvm", sizeof(vm.name));
	vm.systemCall = testSyscall;

	/* Data region (round up to power of 2) */
	int dataLength = header->dataLength + header->litLength + header->bssLength;
	for (i = 0; dataLength > (1 << i); i++) {}
	dataLength = 1 << i;

	vm.dataAlloc = dataLength + 4;
	vm.dataBase = (byte *)Hunk_Alloc(vm.dataAlloc, h_high);
	vm.dataMask = dataLength - 1;

	memcpy(vm.dataBase, fileData + header->dataOffset, header->dataLength + header->litLength);
	for (i = 0; i < header->dataLength; i += 4)
		*(int *)(vm.dataBase + i) = LittleLong(*(int *)(vm.dataBase + i));

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

static void testVMInterpreter(void) {
	vm_t *vm = setupVM(TEST_QVM_PATH, VMI_BYTECODE);
	ASSERT_EQ_INT(1, vm != NULL);

	lastReportedValue = 0;
	currentVM = vm;
	int args[MAX_VMMAIN_ARGS] = {0, 3, 4};
	int result = VM_CallInterpreted(vm, args);

	EXPECT_EQ_INT(7, result);
	EXPECT_EQ_INT(7, (int)lastReportedValue);
}

#ifndef NO_VM_COMPILED
static void testVMCompiled(void) {
	vm_t *vm = setupVM(TEST_QVM_PATH, VMI_COMPILED);
	ASSERT_EQ_INT(1, vm != NULL);

	if (!vm->compiled) return;

	lastReportedValue = 0;
	currentVM = vm;
	int args[MAX_VMMAIN_ARGS] = {0, 3, 4};
	int result = VM_CallCompiled(vm, args);

	EXPECT_EQ_INT(7, result);
	EXPECT_EQ_INT(7, (int)lastReportedValue);
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
