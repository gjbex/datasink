## Without datasink

Consider the following script that would perform parallel, indepedent computations on a compute node that has a total of 20 cores:
```
nr_cores=20
for id in `seq 0 $(( ${nr_cores} - 1 ));
do
    some_program  "some_input_${id}"  >  "some_output_${i}" &
done
wait

for id in `seq 0 $(( ${nr_cores} - 1 ));
do
    cat  "some_output_${i}"  >>  "all_output"
done
```
Each of the, in this case, 20 instances of the `some_program` application will perform writes on standard output, which is redirected to individual files.  When the computations have been performed in parallel, the resulting individual output files are aggregated into a single file.

Needless to say that such an approach will lead to many I/O operations, and meta data operations.  This particular case is pretty mild when compared to more massively parallelized computation using the worker framework or nitor (Adaptive Computing).

## With datasink

Datasink can be of help here, with a very minor change to the workflow, and none to the application `some_program`.  Consider:
```
nr_cores=20

create_sink  -nr_sinks ${nr_cores}  -sink_size 1mb  \
             -sink_file data.sink

for id in `seq 0 $(( ${nr_cores} - 1 ));
do
    some_program  "some_input_${id}"  |  data_sink  -id ${id}  \
                                                    -sink_file data.sink
done
wait

vacuum_sink  -sink_file data_sink  -out_file all_ouput
```
The `create_sink` command prepares a sink file `data.sink` that will be sued for output operations.  It contains meta data so that it can be handled by other datasink tools without the need to know how it was created exactly.  For details, see [Creating sink files](create_sink.md).

Rather than redirecting the standard output of processes to a file, it is piped through `data_sink` which will deal with the file I/O on behalf of the processes.  For details, see [Writing data to a sink file](data_sink.md).

Lastly, the sink file is cleaned up by running `vacuum_sink` that produces a file that is identical to the aggregated output in the original workflow.  For details, see [Compacting results](vacuum_sink.md).

Note that `data_sink` is agnostic about the nature of the processes' output, which may be text or binary.  However, for binary output, a simple concatenation of individual output will probably not result in a valid aggregated output file, so `split_sink` (see [Splitting results](split_sink.md) for details) and a subsequent specialized aggregation step may be more appropriate in that case.  For a description of the sink file format, see [Sink file description](sink_file.md).
