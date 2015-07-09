Some HPC workloads are embarrassingly parallel in the sense that they consists of a large number of independent computations performed by coordinated, but non-interacting processes.  Each of these processes performs I/O operations, potentially on a shared, parallel file system such as lustre or GPFS.

Many (smallish) independent write operations will results in a high load for the meta data servers, which may degrade performance for all jobs on an HPC cluster.

Although it is possible to tune parallel file systems for such workloads, this implies that other I/O patterns may suffer a performance degradation.  Hence it would be useful to reduce the number of meta data IOPs as much as possible.

datasink aims to do this, without imposing a code modification on the applications.
