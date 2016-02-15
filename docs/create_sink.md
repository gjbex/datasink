The first step is the creation of a sink file, which is accomplished easily using the `create_sink` command.

Running `create_sink` will create a sink file for the specified number of processes (`nr_sink`), which each will write at most `sink_size` bytes of data, e.g.,
```
create_sink  -nr_sinks 40  -sink_size 2mb  -sink_file data.sink
```
will create file `data.sink` that wiil have a pre-allocated size of `(3 + nr_sinks)*8 + nr_sinks*sink_size` byte.

The sink size can be specified in byte (no unit), kb, mb or gb.

Optionally, the meta data size can be set explicitly, however, use cases for this are limited.  The `-meta_size` allows to specify the size of the meta data section in bytes, or the same units as accepted for the sink size.

For debugging purposes, the `-verbose` flag allows to set the level of information printed to standard error.  For level 0 there is no feedback, level ` provides run time information, higher levels may be added in future.

Help on the avaible command line options for this command can be obtained using the `-?` flag.
