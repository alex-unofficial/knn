# knn
## compute the all-knn problem

This program was built as an assignment for my Parallel & Distributed Comp. Systems
class in University.

Given a set of input points it will find the K-Nearest Neighbours of each data point in the set,
in a set of distributed memory nodes using `OpenMPI`.

Compiling
---------
Clone the repository using
```bash
git clone https://github.com/alex-unofficial/knn
cd knn
```

### setup locally:
In order to compile the program you need to have
installed [mpi](https://www.open-mpi.org/), and [gsl](https://www.gnu.org/software/gsl/).
[pkgconf](http://pkgconf.org/) is also required.
You also need `make` to run the Makefile.

### setup on `AUTH IT Compute Cluster`
you can use the following line to load all the required modules
```
module load gcc/10.2.0 openmpi/4.0.5 gsl/2.5 pkgconf/1.7.3
```

To build the main executable run
```bash
make
```

To build and run a suite of tests run
```bash
make test
```

To build everything run
```bash
make all
```

All the binaries are located in the `bin/` directory.

Running the program
-------------------
To run the program for a matrix stored in `input.dat` to find the `k` nearest neighbours do
```bash
mpirun ./bin/mpiknn input.dat k
```

`input.dat` must be in the format
```dat
N d
a11 a12 a13 ... a1d
a21 a22 a23 ... a2d
 :   :   :  '.   :
 :   :   :    '. :
aN1 aN2 aN3 ... aNd
```
where the first line specifies the number of input points `N` and the number of dimensions `d`
followed by `N` rows of `d` space-seperated values. Each row represent an input point.

You may also specify some additional parameters.

the number of processes can be specified to `mpirun` using `-N`
```bash
mpirun -N p ./bin/mpiknn input.dat k
```

by default the output is redirected to `stdout`, you can specify an output device using `-o`
```bash
mpirun -N p ./bin/mpiknn -o output -- input.dat k
```

In addition to that, you can also specify the log file, where information about the execution
will be added using `-l`
```bash
mpirun -N p ./bin/mpiknn -o output -l logfile -- input.dat k
```

You can specify the input device as well using `-i`
```bash
mpirun -N p ./bin/mpiknn -i input -o output -l logfile -- k
```

You may specify k using `-k`
```bash
mpirun -N p ./bin/mpiknn -k k -i input -o output -l logfile
```

Finally, you may specify the memory each process is allowed to use using `-m`
```bash
mpirun -N p ./bin/mpiknn -k k -i input -o output -l logfile -m mem
```
you can use notation like `10G` or `512k` for the memory.


A summary of the options is below
```bash
./bin/mpiknn [-i input] [-o output] [-l logfile] [-m mem] [-k k] [--] [input.dat] [k]
```
where you may not specify the input file both using `-i` and after `--`, but you must specify at least 1.
the same is true for `k`


### running on `AUTH IT`
in order to run the program in the `AUTH IT` compute cluster you can use one of the available
batch scripts (or write your own)

for example
```
sbatch tools/mpiknn.sh input.dat k mem
```
