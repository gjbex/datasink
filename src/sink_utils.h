#ifndef SINK_UTILS_HDR
#define SINK_UTILS_HDR

#include <stdio.h>

/* exit codes */
#define EXIT_NO_ARG_ERR 2
#define EXIT_ARG_ERR 3
#define EXIT_OPEN_ERR 4
#define EXIT_SEEK_ERR 5
#define EXIT_READ_ERR 6
#define EXIT_SIZE_ERR 7
#define EXIT_DD_ERR 8

typedef struct {
    long meta_size;
    long sink_size;
    long nr_sinks;
} Meta_data;

int read_meta_data(FILE *fp, Meta_data *meta_data);
int show_meta_data(const Meta_data *meta_data);
long read_data_size(FILE *fp, long id);
int seek_data(FILE *fp, const Meta_data *meta_data, long id);
long compute_size(char *str);

#define NR_META_DATA_FIELDS 3

#endif
