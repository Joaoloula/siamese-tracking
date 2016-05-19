#!/usr/bin/env sh

TOOLS=../../../mpi-caffe-master/build/tools

# mpirun -np 2 $TOOLS/caffe train --solver=mpi_mnist_siamese_solver.prototxt
mpirun -np 2 $TOOLS/caffe time -model mpi_mnist_siamese_train_test.prototxt -iterations 10

