#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "create_cl_params.h"
#include "create_cl_params_aux.h"

#define MAX_CL_OPT_LEN 128
#define MAX_CL_LINE_LEN 1024

void initCL(Params *params) {
	int len;
	len = strlen("1024");
	if (!(params->sink_size = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate sink_size field");
	strncpy(params->sink_size, "1024", len + 1);
	len = strlen("-1");
	if (!(params->meta_size = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate meta_size field");
	strncpy(params->meta_size, "-1", len + 1);
	params->nr_sinks = 20;
	len = strlen("data.sink");
	if (!(params->sink_file = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate sink_file field");
	strncpy(params->sink_file, "data.sink", len + 1);
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
		if (!strncmp((*argv)[i], "-sink_size", 11)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-sink_size' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->sink_size);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->sink_size = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-meta_size", 11)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-meta_size' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->meta_size);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->meta_size = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-nr_sinks", 10)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isLongCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-nr_sinks' of type long\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->nr_sinks = atol(argv_str);
			i++;
			continue;
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
		if (sscanf(line_str, "sink_size = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-sink_size' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->sink_size);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->sink_size = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->sink_size);
			continue;
		}
		if (sscanf(line_str, "meta_size = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-meta_size' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->meta_size);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->meta_size = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->meta_size);
			continue;
		}
		if (sscanf(line_str, "nr_sinks = %[^\n]", argv_str) == 1) {
			if (!isLongCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-nr_sinks' of type long\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->nr_sinks = atol(argv_str);
			continue;
		}
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
	fprintf(fp, "%ssink_size = '%s'\n", prefix, params->sink_size);
	fprintf(fp, "%smeta_size = '%s'\n", prefix, params->meta_size);
	fprintf(fp, "%snr_sinks = %ld\n", prefix, params->nr_sinks);
	fprintf(fp, "%ssink_file = '%s'\n", prefix, params->sink_file);
	fprintf(fp, "%sverbose = %d\n", prefix, params->verbose);
}

void finalizeCL(Params *params) {
	free(params->sink_size);
	free(params->meta_size);
	free(params->sink_file);
}

void printHelpCL(FILE *fp) {
	fprintf(fp, "  -sink_size <string>: size of the data for sinks, units b, kb, mb\n                       (default: '1024')\n  -meta_size <string>: size for the meta data, advanced option\n                       (default: '-1')\n  -nr_sinks <long integer>: number of sinks in the sink file (default:\n                            20)\n  -sink_file <string>: sink file name (default: 'data.sink')\n  -verbose <integer>: write debug information to standard error\n                      (default: 0)\n  -?: print this message");
}