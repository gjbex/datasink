#include <err.h>
#include <stdlib.h>

#include "sink_utils.h"

int read_meta_data(FILE *fp, Meta_data *meta_data) {
    long offset = 0;
    if (fseek(fp, offset, SEEK_SET) != 0)
        err(EXIT_SEEK_ERR, "can not seek to %ld", offset);
    if (fread(&meta_data->meta_size, sizeof(long), 1, fp) == 0)
        err(EXIT_READ_ERR, "can not meta size");
    if (fread(&meta_data->sink_size, sizeof(long), 1, fp) == 0)
        err(EXIT_READ_ERR, "can not sink size");
    if (fread(&meta_data->nr_sinks, sizeof(long), 1, fp) == 0)
        err(EXIT_READ_ERR, "can not number of sinks");
    return EXIT_SUCCESS;
}

int show_meta_data(const Meta_data *meta_data) {
    fprintf(stderr, "meta size = %ld\n", meta_data->meta_size);
    fprintf(stderr, "sink size = %ld\n", meta_data->sink_size);
    fprintf(stderr, "nr of sinks = %ld\n", meta_data->nr_sinks);
    return EXIT_SUCCESS;
}

long read_data_size(FILE *fp, long id) {
    long data_size;
    seek_meta_sink(fp, id);
    if (fread(&data_size, sizeof(long), 1, fp) == 0)
        err(EXIT_READ_ERR, "can not data size");
    return data_size;
}

int seek_data(FILE *fp, const Meta_data *meta_data, long id) {
    long offset = meta_data->meta_size + id*meta_data->sink_size;
    if (fseek(fp, offset, SEEK_SET) != 0)
        err(EXIT_SEEK_ERR, "can not seek to %ld", offset);
    return EXIT_SUCCESS;
}

int seek_meta_sink(FILE *fp, long id) {
    long offset = (NR_META_DATA_FIELDS + id)*sizeof(long);
    if (fseek(fp, offset, SEEK_SET) != 0)
        err(EXIT_SEEK_ERR, "can not seek to %ld", offset);
    return EXIT_SUCCESS;
}

long convert_size_units(char *str) {
    char *end_ptr = str;
    long size = strtol(str, &end_ptr, 10);
    if (*end_ptr != '\0') {
        if (*end_ptr == 'k' || *end_ptr == 'K') {
            size *= 1024;
        } else if (*end_ptr == 'm' || *end_ptr == 'M') {
            size *= 1024*1024;
        } else if (*end_ptr == 'g' || *end_ptr == 'G') {
            size *= 1024*1024*1024;
        } else if (!(*end_ptr == 'b' || *end_ptr == 'B')) {
            errx(EXIT_ARG_ERR, "invalid size unit: '%c'", *end_ptr);
        }
    } else if (end_ptr == str) {
        errx(EXIT_ARG_ERR, "invalid size specification: '%s'", str);
    }
    return size;
}

long compute_sink_file_size(const Meta_data *meta_data) {
    return meta_data->meta_size + meta_data->nr_sinks*meta_data->sink_size;
}

int pre_allocate(const char *file_name, long size, int verbose) {
    char cmd[CMD_LEN];
    int status;
    if (size <= 2147483647) {
        sprintf(cmd, "dd if=/dev/zero of=%s bs=1 count=%ld%s",
                file_name, size, verbose ? "" : " 2> /dev/null");
        if (verbose) {
            fprintf(stderr, "executing '%s'\n", cmd);
        }
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
    return EXIT_SUCCESS;
}

long compute_total_data_size(FILE *fp) {
    long total_size = 0, id, data_size;
    Meta_data meta_data;
    read_meta_data(fp, &meta_data);
    for (id = 0; id < meta_data.nr_sinks; id++) {
        data_size = read_data_size(fp, id);
        if (data_size > meta_data.sink_size) {
            total_size += meta_data.sink_size;
        } else if (data_size >= 0) {
            total_size += data_size;
        }
    }
    return total_size;
}
