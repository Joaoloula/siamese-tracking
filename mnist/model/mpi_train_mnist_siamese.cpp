#include <caffe/caffe.hpp>

// Set to TRAIN Phase
caffe::Caffe::set_phase(caffe::Caffe::TRAIN);
// set GPU
caffe::Caffe::set_mode(caffe::Caffe::CPU);
int deviceId = 0;
caffe::Caffe::SetDevice(deviceId);
LOG(INFO) << "Using CPU";
// Solver initialization and evaluation
caffe::SolverParameter solverParams;
caffe::ReadProtoFromTextFileOrDie("/home/loula/Programming/cpp/tracking/mnist/model/mpi_mnist_siamese_solver.prototxt", &solverParams);
caffe::Solver<float>* solver = new caffe::SGDSolver<float>(solverParams);
solver->Solve();
caffe::Net<float> mnist_siamese("/home/loula/Programming/cpp/tracking/mnist/model/mpi_mnist_siamese_train_test.prototxt");
