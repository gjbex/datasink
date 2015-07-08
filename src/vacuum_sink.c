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
    long id, data_size, count;
    char buffer[BUFFER_SIZE];
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "# ", &params);
    }
    if ((ifp = fopen(params.sink_file, "rb")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.sink_file);
    }
    if ((ofp = fopen(params.out_file, "wb")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.out_file);
    }
    read_meta_data(ifp, &meta_data);
    if (params.verbose) {
        show_meta_data(&meta_data);
    }
    for (id = 0; id < meta_data.nr_sinks; id++) {
        data_size = read_data_size(ifp, id);
        if (params.verbose) {
            fprintf(stderr, "%ld has %ld bytes\n", id, data_size);
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
