A sink file contains meta information, as well as, potentially a lot of, padding.  This can be removed by the `vacuum_sink` command that uses the sink files meta information to produce an aggregated output file that contains only the actual data produced by each of the individual processes.
```
vacuum_sink  -sink_file data.sink  -out_file data.txt
```
As mentioned, the data need not be text data, it can be binary data as well, but in that case `vacuum_sink` my not be the correct approach.

Optionally, `vacuum_sink` can produce feedback useful for tuning and/or debugging.  The `-verbose` flag allows to set the level of information printed to standard error.  For level 0 there is no feedback, level ` provides run time information, higher levels may be added in future.

Help on the avaible command line options for this command can be obtained using the `-?` flag.
