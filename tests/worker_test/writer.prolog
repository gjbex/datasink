#!/bin/bash -l

module load datasink/1.0

create_sink  -sink_size $(( 1024*20 ))  -sink_file data.sink  -nr_sinks 100
