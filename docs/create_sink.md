The first step is the creation of a sink file, which is accomplished easily using the `create_sink` command.

Running `create_sink` will create a sink file for the specified number of processes (`nr_sink`), which each will write at most `sink_size` bytes of data, e.g.,
```
create_sink  -nr_sinks 40  -sink_size  -sink_file data.sink
```
will create file `data.sink` that wiil have a pre-allocated size of `(3 + nr_sinks)*8 + nr_sinks*sink_size` byte.
