#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sink_utils.h"
#include "create_cl_params.h"

#define CMD_LEN 32000

Meta_data convert_params(Params *params);

int main(int argc, char *argv[]) {
    long size, i, fill = -1;
    char cmd[CMD_LEN];
    int status;
    FILE *fp;
    Params params;
    Meta_data meta_data;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "# ", &params);
    }
    meta_data = convert_params(&params);
    if (params.verbose) {
        fprintf(stderr, "meta_size = %ld\n", meta_data.meta_size);
        fprintf(stderr, "sink_size = %ld\n", meta_data.sink_size);
        fprintf(stderr, "nr_sinks  = %ld\n", meta_data.nr_sinks);
    }
    size = meta_data.meta_size + meta_data.nr_sinks*meta_data.sink_size;
    if (size <= 2147483647) {
        sprintf(cmd, "dd if=/dev/zero of=%s bs=1 count=%ld",
                params.sink_file, size);
        status = system(cmd);
        if (status == -1) {
            err(EXIT_DD_ERR, "could not fork dd");
        } else if (status != 0) {
            err(EXIT_DD_ERR, "dd exited with code %d\n",
                    WEXITSTATUS(status));
        }
    } else {
        errx(EXIT_SIZE_ERR, "file size too large");
    }
    if ((fp = fopen(params.sink_file, "rb+")) == NULL) {
        err(EXIT_OPEN_ERR, "can not open %s", params.sink_file);
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
    meta_data.sink_size = compute_size(params->sink_size);
    meta_data.meta_size = compute_size(params->meta_size);
    meta_data.nr_sinks = params->nr_sinks;
    if (meta_data.meta_size < 0) {
        meta_data.meta_size = req_size;
    } else if (meta_data.meta_size < req_size) {
        errx(EXIT_SIZE_ERR, "meta data size too small");
    }
    return meta_data;
}
