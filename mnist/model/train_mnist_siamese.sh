#!/usr/bin/env sh

TOOLS=../../../caffe-master/build/tools

mpirun -np 8 $TOOLS/caffe time -model mnist_siamese_train_test.prototxt -iterations 10
# $TOOLS/caffe train --solver=mnist_siamese_solver.prototxt | tee log_mnist.log
