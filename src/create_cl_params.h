#ifndef CREATE_CL_PARAMS_HDR
#define CREATE_CL_PARAMS_HDR

#include <stdio.h>

typedef struct {
	long sink_size;
	long meta_size;
	long nr_sinks;
	char * sink_file;
	int verbose;
} Params;

void initCL(Params *params);
void parseCL(Params *params, int *argc, char **argv[]);
void parseFileCL(Params *params, char *fileName);
void dumpCL(FILE *fp, char prefix[], Params *params);
void finalizeCL(Params *params);
void printHelpCL(FILE *fp);

#endif