To obtain the meta information stored in a sink file, the `info_sink` command can be used.  It will list the meta data size, the sink size, the number of sinks, and the number of bytes written to each sink.  It will issue a warning if sinks are unused.
```
info_sink  -sink_file data.sink
```

For debugging purposes, the `-verbose` flag allows to set the level of information printed to standard error.  For level 0 there is no feedback, level ` provides run time information, higher levels may be added in future.
