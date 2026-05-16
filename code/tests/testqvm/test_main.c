/* Minimal QVM module for VM unit testing.
 * vmMain returns (arg0 + arg1) and calls syscall 0 with the result. */

#include "test_syscalls.h"

int vmMain(int command, int arg0, int arg1, int arg2, int arg3,
           int arg4, int arg5, int arg6, int arg7,
           int arg8, int arg9, int arg10, int arg11) {
	int result = arg0 + arg1;
	test_report(result);
	return result;
}
