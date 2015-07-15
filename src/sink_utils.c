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

long compute_size(char *str) {
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
