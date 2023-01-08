#include "c.h"

void init(int argc, char *argv[]) {
	{
		input_init(argc, argv);
	}
	{
		main_init(argc, argv);
	}
	{
		prof_init(argc, argv);
	}
	{
		trace_init(argc, argv);
	}
	{
		type_init(argc, argv);
	}
}
