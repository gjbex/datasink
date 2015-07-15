#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "sink_utils.h"
#include "data_cl_params.h"

#define BUFF_SIZE 1024
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]) {
    Params params;
    Meta_data meta_data;
    char buffer[BUFF_SIZE];
    int is_overrun = FALSE;
    long offset, size = 0;
    FILE *fp;
    struct timeval start_time, end_time;

    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "", &params);
    }
    if (params.id < 0) {
        errx(EXIT_INVALID_ID, "sink ID %ld invalid", params.id);
    }
    if ((fp = fopen(params.sink_file, "rb+")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.sink_file);
    }
    if (params.verbose) {
        gettimeofday(&start_time, NULL);
        fprintf(stderr, "%ld opened '%s' at %d.%d\n", params.id,
                params.sink_file, (int) start_time.
                tv_sec, (int) start_time.tv_usec);
    }
    read_meta_data(fp, &meta_data);
    if (params.verbose) {
        show_meta_data(&meta_data);
    }
    if (params.id >= meta_data.nr_sinks) {
        fclose(fp);
        errx(EXIT_INVALID_ID, "sink ID %ld > %ld",
                params.id, meta_data.nr_sinks - 1);
    }
    seek_data(fp, &meta_data, params.id);
    for (;;) {
        int count = fread(buffer, sizeof(char), BUFF_SIZE, stdin);
        if (count == 0) {
            break;
        }
        size += count;
        if (size <= meta_data.sink_size) {
            fwrite(buffer, sizeof(char), count, fp);
        } else if (!is_overrun) {
            is_overrun = TRUE;
            fwrite(buffer, sizeof(char), size - meta_data.sink_size, fp);
            warnx("data written to sink %ld exceeds sink size, data loss\n",
                    params.id); 
        }
    }
    fseek(fp, (3 + params.id)*sizeof(long), SEEK_SET);
    fwrite(&size, sizeof(long), 1, fp);
    fseek(fp, 512, SEEK_SET);
    if (params.verbose) {
        gettimeofday(&start_time, NULL);
        fprintf(stderr, "%ld closed '%s' at %d.%d\n", params.id,
                params.sink_file, (int) start_time.tv_sec,
                (int) start_time.tv_usec);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
