#include <cstdio>
#include "pch.h"

int main(int argc,char* argv[])
{
	RUN_EXAMPLE(example_cmdline_option);
	RUN_EXAMPLE(example_cmdline_value);
	RUN_EXAMPLE_A(example_ini,"/home/irokez/projects/cpp-class-lib/samples/config.sample");
	RUN_EXAMPLE(example_threadpool);
    return 0;
}