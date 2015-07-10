A sink file has a size that is fixed at its time of creation, and that is pre-allocated on the file system.  It consists of two parts, meta data and actual payload.

The meta data ensures that subsequent calls to `datasink` tools can deal with the file, without the user having to specify meta information, hence at least eliminating one possible source of errors and problems.

The meta data consists of the meta data size, expressed in bytes, and stored as an 8 byte integer, the sink size, i.e., the maximum number of bytes that each of the independent processes may write, again stored as an 8 byte integer.  Further, the maximum number of independent of processes that will write to the sink is store as well, again as an 8 byte integer.  Subsequent 8 byte integers will hold the number of bytes written to te sink for each process, those values as set to `-1` upon creation to distinguish between a sink that was ignored, versus one that was not written to.

Hence, if `nr_sinks` is the number of processes that will write to the sink, the meta data size should be at least `meta_size = 8*(3 + nr_sink)` byte.

The remaining part of the file consists of `nr_sink` sections of `sink_size` byte that will hold the output produced by each of the processes.

Hence the total file size of `file_size = meta_size + sink_size`.

The sink file is agnostic of the data type written to the sink section, so that may be binary or text.

Information about the sink file can be obtained by running the `info_sink` command, see [Sink file information](info_sink.md) for more information.
