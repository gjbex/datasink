#ifndef SPLIT_CL_PARAMS_HDR
#define SPLIT_CL_PARAMS_HDR

#include <stdio.h>

typedef struct {
	char * sink_file;
	char * out_file_base;
	int verbose;
} Params;

void initCL(Params *params);
void parseCL(Params *params, int *argc, char **argv[]);
void parseFileCL(Params *params, char *fileName);
void dumpCL(FILE *fp, char prefix[], Params *params);
void finalizeCL(Params *params);
void printHelpCL(FILE *fp);

#endif