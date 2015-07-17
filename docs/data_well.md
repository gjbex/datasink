The `data_well` command also provides I/O redirection.  It reads from a sink, and writes to standard output, i.e., to a pipe.  Each `data_well` instance will read only from its designated location in the sink file, based on the ID it is passed as a command line argument.

Hence `data_well` is called with two arguments, the sink file name, and the sink ID, which is a number between `0` and `nr_sinks - 1` inclusive.  Its output will be provided through a pipe.
```
data_well  -id ${id}  -sink_file data.sink | some_other_program 
```

Optionally, `data_well` can produce feedback useful for tuning and/or debugging.  The `-verbose` flag allows to set the level of information printed to standard error.  For level 0 there is no feedback, level ` provides run time information, higher levels may be added in future.

