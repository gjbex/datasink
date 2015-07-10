#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "sink_utils.h"
#include "info_cl_params.h"

int main(int argc, char *argv[]) {
    Params params;
    Meta_data meta_data;
    FILE *fp;
    long id, data_size, nr_unused = 0;
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
    for (id = 0; id < meta_data.nr_sinks; id++) {
        data_size = read_data_size(fp, id);
        if (data_size >= 0) {
            printf("size id %ld = %ld byte\n", id, data_size);
        } else {
            printf("size id %ld unused\n", id);
            nr_unused++;
        }
    }
    if (nr_unused > 0) {
        fprintf(stderr, "### warning: %ld sinks were unused\n", nr_unused);
    }
    fclose(fp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
