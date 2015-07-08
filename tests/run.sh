#!/bin/bash

export PATH="../bin:${PATH}"

export DATA_SINK_SIZE=128
export DATA_SINK_META_SIZE=128
export DATA_SINK_FILE='data.sink'

nr_sinks=3
text_file="data.txt"

rm -f ${DATA_SINK_FILE}
create_sink -sink_size  ${DATA_SINK_SIZE} \
            -meta_size  ${DATA_SINK_META_SIZE} \
            -nr_sinks   ${nr_sinks} \
            -sink_file  ${DATA_SINK_FILE}

for id in `seq 0 $(( ${nr_sinks} - 1 ))`;
do
    DATA_SINK_ID=$id data_sink -id ${id} \
        -sink_file ${DATA_SINK_FILE} < "in_data_${id}.txt"
done

rm -f ${text_file}
vacuum_sink -sink_file ${DATA_SINK_FILE} \
            -out_file  ${text_file}
