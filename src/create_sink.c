#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sink_utils.h"
#include "create_cl_params.h"

Meta_data convert_params(Params *params);

int main(int argc, char *argv[]) {
    long size, i, fill = -1;
    FILE *fp;
    Params params;
    Meta_data meta_data;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "", &params);
    }
    meta_data = convert_params(&params);
    if (params.verbose) {
        fprintf(stderr, "meta_size = %ld\n", meta_data.meta_size);
        fprintf(stderr, "sink_size = %ld\n", meta_data.sink_size);
        fprintf(stderr, "nr_sinks  = %ld\n", meta_data.nr_sinks);
    }
    size = compute_sink_file_size(&meta_data);
    pre_allocate(params.sink_file, size, params.verbose);
    if ((fp = fopen(params.sink_file, "rb+")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open '%s'", params.sink_file);
    }
    fwrite(&meta_data.meta_size, sizeof(long), 1, fp);
    fwrite(&meta_data.sink_size, sizeof(long), 1, fp);
    fwrite(&meta_data.nr_sinks, sizeof(long), 1, fp);
    for (i = 0; i < meta_data.nr_sinks; i++) {
        fwrite(&fill, sizeof(long), 1, fp);
    }
    fclose(fp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

Meta_data convert_params(Params *params) {
    Meta_data meta_data;
    long req_size = (NR_META_DATA_FIELDS + params->nr_sinks)*sizeof(long);
    if (params->verbose) {
        dumpCL(stderr, "", params);
        fprintf(stderr, "number of meta fields = %d\n",
                NR_META_DATA_FIELDS);
        fprintf(stderr, "sizeof(long) = %d\n", sizeof(long));
    }
    if (params->verbose) {
        fprintf(stderr, "required meta size = %ld byte\n",
                req_size);
    }
    meta_data.nr_sinks = params->nr_sinks;
    meta_data.sink_size = convert_size_units(params->sink_size);
    meta_data.meta_size = convert_size_units(params->meta_size);
    if (meta_data.meta_size < 0) {
        meta_data.meta_size = req_size;
    } else {
        if (meta_data.meta_size < req_size) {
            errx(EXIT_SIZE_ERR,
                    "meta size %ld byte less than required %ld byte",
                    meta_data.meta_size, req_size);
        }
    }
    return meta_data;
}
