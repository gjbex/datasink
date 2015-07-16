#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "sink_utils.h"
#include "vacuum_cl_params.h"

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    Params params;
    Meta_data meta_data;
    FILE *ifp, *ofp;
    long id, data_size, count, total_size;
    char buffer[BUFFER_SIZE];
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "", &params);
    }
    if ((ifp = fopen(params.sink_file, "rb")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.sink_file);
    }
    read_meta_data(ifp, &meta_data);
    if (params.verbose) {
        show_meta_data(&meta_data);
    }
    total_size = compute_total_data_size(ifp);
    if (params.verbose) {
        fprintf(stderr, "total data size = %ld byte\n", total_size);
    }
    pre_allocate(params.out_file, total_size, params.verbose);
    if ((ofp = fopen(params.out_file, "rb+")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.out_file);
    }
    for (id = 0; id < meta_data.nr_sinks; id++) {
        data_size = read_data_size(ifp, id);
        if (params.verbose) {
            fprintf(stderr, "%ld has %ld bytes\n", id, data_size);
        }
        if (!check_data_size(id, &data_size, &meta_data)) {
            continue;
        }
        seek_data(ifp, &meta_data, id);
        while (data_size > 0) {
            if (data_size >= BUFFER_SIZE) {
                count = BUFFER_SIZE;
                data_size -= BUFFER_SIZE;
            } else {
                count = data_size;
                data_size = 0;
            }
            if (fread(buffer, sizeof(char), BUFFER_SIZE, ifp) == 0)
                err(EXIT_READ_ERR, "can not read data for %ld", id);
            fwrite(buffer, sizeof(char), count, ofp);
        }
    }
    fclose(ofp);
    fclose(ifp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
