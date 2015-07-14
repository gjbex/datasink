#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sink_utils.h"
#include "split_cl_params.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    Meta_data meta_data;
    Params params;
    FILE *ifp, *ofp;
    long id;
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
    for (id = 0; id < meta_data.nr_sinks; id++) {
        char *file_name;
        int name_len = strlen(params.out_file_base) + 6;
        long data_size = read_data_size(ifp, id);
        size_t count;
        if (data_size < 0) {
            warnx("no data was written for id %ld\n", id);
            continue;
        }
        if (data_size > meta_data.sink_size) {
            warnx("%ld byte written to sink %ld, capacity exceede by %ld byte", data_size, id, data_size - meta_data.sink_size);
            data_size = meta_data.sink_size;
        }
        seek_data(ifp, &meta_data, id);
        if ((file_name = (char *) malloc(sizeof(char)*name_len)) == NULL) {
            errx(EXIT_MEM_ERR, "can allocate file name");
        }
        sprintf(file_name, "%s.%04ld", params.out_file_base, id);
        if ((ofp = fopen(file_name, "wb")) == NULL) {
            err(EXIT_OPEN_ERR, "can not open '%s'", file_name);
        }
        if (params.verbose) {
            fprintf(stderr, "writing %ld byte to '%s' for id %ld\n",
                    data_size, file_name, id);
        }
        for (;;) {
            count = fread(buffer, sizeof(char), BUFFER_SIZE, ifp);
            if (count == 0 && data_size > 0) {
                err(EXIT_READ_ERR, "read problem for id %ld", id);
            }
            if (data_size > BUFFER_SIZE) {
                fwrite(buffer, sizeof(char), BUFFER_SIZE, ofp);
                data_size -= BUFFER_SIZE;
            } else {
                fwrite(buffer, sizeof(char), data_size, ofp);
                break;
            }
        }
        fclose(ofp);
        free(file_name);
    }
    fclose(ifp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
