#ifndef DATA_CL_PARAMS_HDR
#define DATA_CL_PARAMS_HDR

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	char * sink_file;
	long id;
	int verbose;
	bool append;
} Params;

void initCL(Params *params);
void parseCL(Params *params, int *argc, char **argv[]);
void parseFileCL(Params *params, char *fileName);
void dumpCL(FILE *fp, char prefix[], Params *params);
void finalizeCL(Params *params);
void printHelpCL(FILE *fp);

#endif