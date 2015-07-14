#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "sink_utils.h"
#include "info_cl_params.h"

int compute_field_width(long nr_sinks);

int main(int argc, char *argv[]) {
    Params params;
    Meta_data meta_data;
    FILE *fp;
    long id, data_size, nr_unused = 0, nr_overrun = 0;
    int id_field_width, byte_field_width;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "", &params);
    }
    if ((fp = fopen(params.sink_file, "rb")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.sink_file);
    }
    read_meta_data(fp, &meta_data);
    printf("meta size = %ld byte\n", meta_data.meta_size);
    printf("sink size = %ld byte\n", meta_data.sink_size);
    printf("number of sinks = %ld\n", meta_data.nr_sinks);
    id_field_width = compute_field_width(meta_data.nr_sinks);
    byte_field_width = compute_field_width(meta_data.sink_size);
    for (id = 0; id < meta_data.nr_sinks; id++) {
        data_size = read_data_size(fp, id);
        if (data_size >= 0) {
            printf("size id %*ld = %*ld byte", id_field_width, id,
                    byte_field_width, data_size);
            if (data_size > meta_data.sink_size) {
                nr_overrun++;
                printf(" > %ld, %ld byte lost", meta_data.sink_size,
                       data_size - meta_data.sink_size);
            }
            printf("\n");
        } else {
            printf("size id %*ld unused\n", id_field_width, id);
            nr_unused++;
        }
    }
    if (nr_unused > 0) {
        fprintf(stderr, "### warning: %ld sinks were unused\n", nr_unused);
    }
    if (nr_overrun > 0) {
        fprintf(stderr, "### error: %ld sinks lost data,\n", nr_overrun);
        fprintf(stderr, "#          snik ize too small\n");
    }
    fclose(fp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

int compute_field_width(long nr_sinks) {
    return (int) ceil(log(nr_sinks)/log(10));
}
