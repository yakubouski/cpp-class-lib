#pragma once

#define RUN_EXAMPLE_A(name,...) {printf("\n\n" #name "\n─────────────────────────────────\n"); extern void name(int argc, char* argv[]); const char* a_args[]{ __VA_ARGS__}; name(sizeof(a_args), (char**)a_args); }
#define RUN_EXAMPLE(name) {printf("\n\n" #name "\n─────────────────────────────────\n"); extern void name(int argc, char* argv[]); name(argc, argv); }
