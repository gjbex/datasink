#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "vacuum_cl_params.h"
#include "vacuum_cl_params_aux.h"

#define MAX_CL_OPT_LEN 128
#define MAX_CL_LINE_LEN 1024

void initCL(Params *params) {
	int len;
	len = strlen("data.sink");
	if (!(params->sink_file = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate sink_file field");
	strncpy(params->sink_file, "data.sink", len + 1);
	len = strlen("out.dat");
	if (!(params->out_file = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate out_file field");
	strncpy(params->out_file, "out.dat", len + 1);
	params->verbose = 0;
}

void parseCL(Params *params, int *argc, char **argv[]) {
	char *argv_str;
	int i = 1;
	while (i < *argc) {
		 if (!strncmp((*argv)[i], "-?", 3)) {
			printHelpCL(stderr);
			finalizeCL(params);
			exit(EXIT_SUCCESS);
		}
		if (!strncmp((*argv)[i], "-sink_file", 11)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-sink_file' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->sink_file);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->sink_file = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-out_file", 10)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-out_file' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->out_file);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->out_file = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-verbose", 9)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-verbose' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->verbose = atoi(argv_str);
			i++;
			continue;
		}
		break;
	}
	if (i > 1) {
		(*argv)[i - 1] = (*argv)[0];
		*argv = &((*argv)[i - 1]);
		*argc -= (i - 1);
	}
}

void parseFileCL(Params *params, char *fileName) {
	char line_str[MAX_CL_LINE_LEN];
	char argv_str[MAX_CL_LINE_LEN];
	FILE *fp;
	if (!(fp = fopen(fileName, "r"))) {
		fprintf(stderr, "### error: can not open file '%s'\n", fileName);
		exit(EXIT_CL_FILE_OPEN_FAIL);
	}
	while (fgets(line_str, MAX_CL_LINE_LEN, fp)) {
		if (isCommentCL(line_str)) continue;
		if (isEmptyLineCL(line_str)) continue;
		if (sscanf(line_str, "sink_file = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-sink_file' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->sink_file);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->sink_file = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->sink_file);
			continue;
		}
		if (sscanf(line_str, "out_file = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-out_file' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->out_file);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->out_file = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->out_file);
			continue;
		}
		if (sscanf(line_str, "verbose = %[^\n]", argv_str) == 1) {
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-verbose' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->verbose = atoi(argv_str);
			continue;
		}
		fprintf(stderr, "### warning, line can not be parsed: '%s'\n", line_str);
	}
	fclose(fp);
}

void dumpCL(FILE *fp, char prefix[], Params *params) {
	fprintf(fp, "%ssink_file = '%s'\n", prefix, params->sink_file);
	fprintf(fp, "%sout_file = '%s'\n", prefix, params->out_file);
	fprintf(fp, "%sverbose = %d\n", prefix, params->verbose);
}

void finalizeCL(Params *params) {
	free(params->sink_file);
	free(params->out_file);
}

void printHelpCL(FILE *fp) {
	fprintf(fp, "  -sink_file <string>: sink file to use (default: 'data.sink')\n  -out_file <string>: output file to produce (default: 'out.dat')\n  -verbose <integer>: write debug information to standard error\n                      (default: 0)\n  -?: print this message");
}