#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "H5Cpp.h"

using namespace std;
using namespace cv;
using namespace H5;

// declare maximum number of pairs per image and
// size of images and number of transformations
const int max_pairs = 50000;
const int img_size = 28;
const int trans_number = 1946;
const int skip_lines = 1029; // lines to skip

// define image path
string img_path = "/home/loula/Programming/cpp/tracking/mnist/new_dataset/";

int main(){
	// read lines from listfile
	ifstream file("listfile");
	string one_line;
	vector<string> lines;
	while(getline(file, one_line)){
		lines.push_back(one_line);
	}

	// loop through all images
	for (int k=1; k<100; k++){
		string list_vector [trans_number][5];
		int X [max_pairs][2][img_size][img_size];
		int y [max_pairs][4];
		
		// loop through all transformations of an image
		for (int i=0; i<trans_number; i++){
			// store filename and labels
			string current_line = lines[skip_lines+(k*trans_number)+i];
			string filename = current_line.substr(0, current_line.find(" "));
			string label = current_line.substr(1, current_line.find(" "));
			list_vector[i][0] = filename;
			list_vector[i][1] = label[2];
			list_vector[i][2] = label[3]+label[4];
			list_vector[i][3] = label[5];
			list_vector[i][4] = label[6];
		}
		// create transformation pairs
		for (int i=0; i<max_pairs; i++){
			// choose a random pair, retrieve the corresponding
			// images and calculate the relative transformation
			int index1 = rand() % trans_number;
			int index2 = rand() % trans_number;
			Mat img1 = imread(img_path+to_string(k)+"/"+
					  list_vector[index1][0]);
			Mat img2 = imread(img_path+to_string(k)+"/"+
					  list_vector[index2][0]);
			int label = stoi(list_vector[index1][1]);
			int dif_rot = (stoi(list_vector[index2][2])-
				       stoi(list_vector[index1][1]));
			int dif_x = (stoi(list_vector[index2][3])-
				     stoi(list_vector[index1][3]));
			int dif_y = (stoi(list_vector[index2][4])-
				     stoi(list_vector[index1][4]));
			
			// fill X and y data 
			for (int m=0; m<img_size; m++){
				for (int n=0; n<img_size; n++){
					X[i][0][m][n] = img1.at<int>(m, n); 
					X[i][1][m][n] = img2.at<int>(m, n);
				}
			}
			
			y[i][0] = label; 
			y[i][1] = dif_rot; 
			y[i][2] = dif_x; 
			y[i][3] = dif_y;
		}
		// create h5 file
	        H5File h5_file(to_string(k)+"_mnist_pairs.h5", H5F_ACC_TRUNC);
	        
		// define size of array and create dataspace for X
	        hsize_t X_dims[4];
	        X_dims[0] = max_pairs;
	        X_dims[1] = 2;
		X_dims[2] = img_size;
		X_dims[3] = img_size;
	        DataSpace X_dataspace(4, X_dims);
	        // define datatype for X
	        IntType X_datatype(PredType::NATIVE_INT);
		X_datatype.setOrder(H5T_ORDER_LE);
	      	// create dataset for X
		DataSet X_dataset = h5_file.createDataSet("X", X_datatype, 
                                                          X_dataspace);
	      	// write X dataset to file
		X_dataset.write(X, PredType::NATIVE_INT);

		// define size of array and create dataspace for y
	        hsize_t y_dims[2];
	        y_dims[0] = max_pairs;
	        y_dims[1] = 4;
	        DataSpace y_dataspace(2, y_dims);
	        // define datatype for y
	        IntType y_datatype(PredType::NATIVE_INT);
		y_datatype.setOrder(H5T_ORDER_LE);
	      	// create dataset for y
		DataSet y_dataset = h5_file.createDataSet("y", y_datatype, 
                                                          y_dataspace);
		// write y dataset to file
		y_dataset.write(y, PredType::NATIVE_INT);		
		
		// print progress
		cout << "Conversion " << to_string(k) << "% completed\n";
	}
}
