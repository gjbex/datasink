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
#define EXIT_MEM_ERR 9
#define EXIT_INVALID_ID 10

typedef struct {
    long meta_size;
    long sink_size;
    long nr_sinks;
} Meta_data;

int read_meta_data(FILE *fp, Meta_data *meta_data);
int show_meta_data(const Meta_data *meta_data);
long read_data_size(FILE *fp, long id);
int seek_data(FILE *fp, const Meta_data *meta_data, long id);
int seek_meta_sink(FILE *fp, long id);
long compute_size(char *str);
int pre_allocate(const Meta_data *meta_data, const char *file_name,
                 int verbose);

#define NR_META_DATA_FIELDS 3
#define CMD_LEN 32000


#endif
