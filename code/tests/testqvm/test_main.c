/* QVM module for VM unit testing.
 * vmMain dispatches opcode-heavy command handlers so the host-side test can
 * validate both interpreted and compiled execution paths against known results.
 */

#include "test_commands.h"
#include "test_syscalls.h"

typedef struct {
	char bytes[7];
} testBlob_t;

static int gIntValue;
static signed char gSignedByte;
static unsigned char gUnsignedByte;
static short gSignedShort;
static short gUnsignedShortBits;
static float gFloatValue;
static testBlob_t gBlob;

int vmEntry(int command, int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7,
			int arg8, int arg9, int arg10, int arg11);
static int helperSum3(int a, int b, int c) {
	return a + b + c;
}

static int helperCountdown(int start) {
	int total = 0;

	while (start > 0) {
		total += start;
		start--;
	}

	return total;
}

static void helperReport(int value) {
	test_report(value);
}

static int runBasicCase(int a, int b) {
	int result = helperSum3(a, b, 0);
	helperReport(result);
	return result;
}

static int runIntegerCase(int a, int b, int c, int d, int e, int f) {
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
	result += helperCountdown(3);

	helperSum3(result, a, b);
	helperReport(result);
	return result;
}

static int runFloatCase(int a, int b) {
	float x = (float)a / 4.0f;
	float y = (float)b / 5.0f;
	float z = -x;
	int result;

	z += x + y;
	z += x - y;
	z += x * y;
	z += x / y;

	gFloatValue = z;
	result = (int)gFloatValue;

	if (x == (float)a / 4.0f) result += 101;
	if (x != y) result += 103;
	if (x < y) result += 107;
	if (x <= y) result += 109;
	if (x > y) result += 113;
	if (x >= y) result += 127;

	helperReport(result);
	return result;
}

static int runMemoryCase(int a, int b, int c, int d, int e) {
	testBlob_t localBlob;
	testBlob_t copiedBlob;
	int result;

	gSignedByte = (signed char)a;
	gUnsignedByte = (unsigned char)b;
	gSignedShort = (short)c;
	gUnsignedShortBits = (short)d;
	gIntValue = e;

	localBlob.bytes[0] = (char)1;
	localBlob.bytes[1] = (char)2;
	localBlob.bytes[2] = (char)3;
	localBlob.bytes[3] = (char)4;
	localBlob.bytes[4] = (char)5;
	localBlob.bytes[5] = (char)6;
	localBlob.bytes[6] = (char)7;

	copiedBlob = localBlob;
	gBlob = copiedBlob;

	result = (int)gSignedByte;
	result += (int)gUnsignedByte;
	result += (int)gSignedShort;
	result += (int)(*(unsigned short *)&gUnsignedShortBits);
	result += gIntValue;
	result += (int)gBlob.bytes[6];
	result += helperSum3(gBlob.bytes[0], gBlob.bytes[1], gBlob.bytes[2]);

	helperReport(result);
	return result;
}

int vmEntry(int command, int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7,
			int arg8, int arg9, int arg10, int arg11) {
	unsigned ua;
	unsigned ub;
	unsigned uc;
	unsigned ud;
	float x;
	float y;
	float z;
	testBlob_t localBlob;
	testBlob_t copiedBlob;
	int result;

	switch (command) {
	case TEST_VM_CMD_BASIC:
		return runBasicCase(arg0, arg1);
	case TEST_VM_CMD_INT_OPS:
		ua = (unsigned)arg0;
		ub = (unsigned)arg1;
		uc = (unsigned)arg2;
		ud = (unsigned)arg3;
		result = 0;

		if (arg0 == arg1) result += 11;
		if (arg0 != arg1) result += 13;
		if (arg0 < arg1) result += 17;
		if (arg0 <= arg1) result += 19;
		if (arg0 > arg1) result += 23;
		if (arg0 >= arg1) result += 29;

		if (ua < ub) result += 31;
		if (ua <= ub) result += 37;
		if (ua > ub) result += 41;
		if (ua >= ub) result += 43;

		result += -arg0;
		result += arg0 + arg1;
		result += arg0 - arg1;
		result += arg0 * arg1;
		result += arg2 / arg3;
		result += arg2 % arg3;
		result += (int)(uc / ud);
		result += (int)(uc % ud);
		result += arg0 & arg1;
		result += arg2 | arg3;
		result += arg4 ^ arg5;
		result += ~arg0;
		result += arg0 << 2;
		result += arg1 >> 1;
		result += (int)(uc >> 3);
		result += helperCountdown(3);

		helperSum3(result, arg0, arg1);
		test_report(result);
		return result;
	case TEST_VM_CMD_FLOAT_OPS:
		x = (float)arg0 / 4.0f;
		y = (float)arg1 / 5.0f;
		z = -x;

		z += x + y;
		z += x - y;
		z += x * y;
		z += x / y;

		gFloatValue = z;
		result = (int)gFloatValue;

		if (x == (float)arg0 / 4.0f) result += 101;
		if (x != y) result += 103;
		if (x < y) result += 107;
		if (x <= y) result += 109;
		if (x > y) result += 113;
		if (x >= y) result += 127;

		test_report(result);
		return result;
	case TEST_VM_CMD_MEMORY_OPS:
		gSignedByte = (signed char)arg0;
		gUnsignedByte = (unsigned char)arg1;
		gSignedShort = (short)arg2;
		gUnsignedShortBits = (short)arg3;
		gIntValue = arg4;

		localBlob.bytes[0] = (char)1;
		localBlob.bytes[1] = (char)2;
		localBlob.bytes[2] = (char)3;
		localBlob.bytes[3] = (char)4;
		localBlob.bytes[4] = (char)5;
		localBlob.bytes[5] = (char)6;
		localBlob.bytes[6] = (char)7;

		copiedBlob = localBlob;
		gBlob = copiedBlob;

		result = (int)gSignedByte;
		result += (int)gUnsignedByte;
		result += (int)gSignedShort;
		result += (int)(*(unsigned short *)&gUnsignedShortBits);
		result += gIntValue;
		result += (int)gBlob.bytes[6];
		result += helperSum3(gBlob.bytes[0], gBlob.bytes[1], gBlob.bytes[2]);

		test_report(result);
		return result;
	default:
		test_report(-9999);
		return -9999;
	}
}
