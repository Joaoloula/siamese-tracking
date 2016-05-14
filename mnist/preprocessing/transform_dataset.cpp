#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace cv;
using namespace std;

// boost namespace, file path and iterator
namespace fs = boost::filesystem;
fs::path targetDir("/home/loula/Programming/c++/tracking/mnist/dataset"); 
fs::directory_iterator it(targetDir), eod;

// source and destination paths
string src_path = "/home/loula/Programming/c++/tracking/mnist/dataset/csv/mnist_train.csv"; 
string dst_path = "/home/loula/Programming/c++/tracking/mnist/transformed_dataset/";

int main(){
	// compression parameters (png)
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0);
	
	// define center of rotation and size of images
	Point center = Point(14,14);
	Size dsize = Size(28, 28);
	
	// create array from which the translation matrix will be sourced
	// and declare the variables for source, destination and transformations
	float trans_array[6] = {0, 0, 0, 0, 0, 0};
	Mat src, rot_mat, trans_mat, trans_rot_mat;
        Mat dst = Mat::zeros(28, 28, CV_64F);

	// declare variables to store the image, its label and its id
	ifstream file(src_path);
	string image;
	string label;
	int id = 0;

	// loop through images (lines in csv file) 
	while(getline(file, image)){
		// create image stream
		stringstream image_stream(image);
		
		// read image label
		getline(image_stream, label, ',');

		// loop through elements in image
		// adding them to a vector
		vector<int> img_matrix;
		string pixel;
	        while(getline(image_stream, pixel, ',')){
			img_matrix.push_back(stoi(pixel));
	 	}

       		// transform vector into mat		
		//src = Mat(28, 28, CV_8UC1, &img_mat
		Mat src = Mat::zeros(28, 28, CV_8UC1);
		for(int rows = 0; rows < 28; rows++){
   			for(int cols= 0; cols< 28; cols++){
      				src.at<uchar>(rows,cols) = img_matrix[(28*rows)+cols];
   			}
		}

		// loop through possible rotations
		for (int rot=-30; rot<=30; rot=rot+3){
			// create rotation matrix
			rot_mat = getRotationMatrix2D(center, rot, 1);
			for (int x_trans=-3; x_trans<=3; x_trans++){		
				// add x translation
				trans_array[2] = x_trans;
				for (int y_trans=-3; y_trans<=3; y_trans++){	
					// add y translation 
					trans_array[5] = y_trans;
					// create translation matrix
					trans_mat = Mat(2, 3, CV_8UC1, &trans_array);
					//combine translation and rotation matrices
					add(trans_mat, rot_mat, trans_rot_mat, 
					    Mat(), CV_32FC1);
					
					// transform the image
					warpAffine(src, dst, rot_mat, dsize);

					// inverse image
					bitwise_not (dst, dst);

					// write resulting image
					imwrite(dst_path +
						"img=" + to_string(id) +
						"label=" + label +
						"-rot=" + to_string(rot) +
						"-x_trans=" + to_string(x_trans) +
						"-y_trans=" + to_string(y_trans) +
						".png",
						dst,
						compression_params
						);
				}
			}
		}
		id ++;
	}
	return 0;
}
