#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sink_utils.h"
#include "create_cl_params.h"

#define CMD_LEN 32000

void check_params(const Params *params);

int main(int argc, char *argv[]) {
    long size, i, fill = -1;
    char cmd[CMD_LEN];
    int status;
    FILE *fp;
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose) {
        dumpCL(stderr, "# ", &params);
    }
    check_params(&params);
    size = params.meta_size + params.nr_sinks*params.sink_size;
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
    fwrite(&params.meta_size, sizeof(long), 1, fp);
    fwrite(&params.sink_size, sizeof(long), 1, fp);
    fwrite(&params.nr_sinks, sizeof(long), 1, fp);
    for (i = 0; i < params.nr_sinks; i++) {
        fwrite(&fill, sizeof(long), 1, fp);
    }
    fclose(fp);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

void check_params(const Params *params) {
    long req_size = (params->nr_sinks + NR_META_DATA_FIELDS)*sizeof(long);
    if (params->meta_size < req_size) {
        errx(EXIT_SIZE_ERR, "meta data size too small");
    }
}