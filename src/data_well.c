#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "well_cl_params.h"
#include "sink_utils.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    Params params;
    Meta_data meta_data;
    FILE *fp;
    long data_size;
    char buffer[BUFFER_SIZE];
    size_t count;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "", &params);
    }
    if ((fp = fopen(params.sink_file, "rb")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.sink_file);
    }
    read_meta_data(fp, &meta_data);
    check_sink_id(params.id, &meta_data, fp);
    data_size = read_data_size(fp, params.id);
    if (!check_data_size(params.id, &data_size, &meta_data)) {
        fclose(fp);
        errx(EXIT_SIZE_ERR, "sink %ld has no data", params.id);
    }
    if (params.verbose) {
        fprintf(stderr, "reading %ld byte from id %ld\n", data_size,
                params.id);
    }
    seek_data(fp, &meta_data, params.id);
    for (;;) {
        count = fread(buffer, sizeof(char), BUFFER_SIZE, fp);
        if (count == 0 && data_size > 0) {
            err(EXIT_READ_ERR, "read problem for id %ld", params.id);
        }
        if (data_size > BUFFER_SIZE) {
            fwrite(buffer, sizeof(char), BUFFER_SIZE, stdout);
            data_size -= BUFFER_SIZE;
        } else {
            fwrite(buffer, sizeof(char), data_size, stdout);
            break;
        }
    }
    fclose(fp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
