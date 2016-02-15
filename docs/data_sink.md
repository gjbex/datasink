The `data_sink` command provides the actual I/O redirection.  It reads from standard input, i.e., from a pipe, and writes data to the sink file.  Each `data_sink` instance will write only to its designated location in the sink file, based on the ID it is passed as a command line argument.

Hence `data_sink` is called with two arguments, the sink file name, and the sink ID, which is a number between `0` and `nr_sinks - 1` inclusive.  Its input will be provided through a pipe.
```
some_program | data_sink  -id ${id}  -sink_file data.sink
```

Subsequent calls to data_sink with the same ID will overwrite that sink's
data.  However, if the `-append` option is specified, the data will be
appended, e.g.,
```
some_program | data_sink  -id ${id}  -append  -sink_file data.sink
```

Optionally, `data_sink` can produce feedback useful for tuning and/or debugging.  The `-verbose` flag allows to set the level of information printed to standard error.  For level 0 there is no feedback, level ` provides run time information, higher levels may be added in future.

Help on the avaible command line options for this command can be obtained using the `-?` flag.
