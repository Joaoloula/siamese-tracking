#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace cv;
using namespace std;

// boost namespace and file path
namespace fs = boost::filesystem;
fs::path targetDir("/home/loula/Programming/cpp/tracking/mnist/dataset");

// source, destination and listfile paths
string src_path = "/home/loula/Programming/cpp/tracking/mnist/dataset/csv/mnist_train.csv"; 
string dst_path = "/home/loula/Programming/cpp/tracking/mnist/new_new_dataset/";

int main(){
	// compression parameters (png)
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0);
	
	// define image parameters
	Point center = Point(14,14);
	int cols_size = 28;
	int rows_size = 28;
	Size dsize = Size(cols_size, rows_size);
	int max_rot = 30;
	int rot_step = 3;
	int max_trans = 3;
	int trans_step = 1;
	int max_images = 100;

	// create array from which the translation matrix will be sourced
	// and declare the variables for source, destination and transformations
	float trans_array[6] = {0, 0, 0, 0, 0, 0};
	Mat src, rot_mat, trans_mat, trans_rot_mat;
        Mat dst = Mat::zeros(dsize, CV_64F);

	// declare variables to store the image, its filename, its label and its id
	ifstream file(src_path);
	string filename;
	string image;
	string label;
	int id = 0;

	// declare variables for the leveldb listfile
	string leveldb_label;
	string id_str;
	string rot_str;
	string x_trans_str;
	string y_trans_str;

	// output to listfile
	// ofstream outfile;
	// outfile.open("listfile", ios_base::app);

	// loop through images (lines in csv file) 
	while(getline(file, image) && id<100){
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
		Mat src = Mat::zeros(dsize, CV_8UC1);
		for(int rows = 0; rows < rows_size; rows++){
   			for(int cols= 0; cols< cols_size; cols++){
      				src.at<uchar>(rows,cols) = img_matrix[(rows_size*rows)+cols];
   			}
		}

		// loop through possible rotations
		for (int rot=-1*max_rot; rot<=max_rot; rot=rot+rot_step){
			// create rotation matrix
			rot_mat = getRotationMatrix2D(center, rot, 1);
			for (int x_trans=-1*max_trans; x_trans<=max_trans; 
			     x_trans=x_trans+trans_step){		
				for (int y_trans=-1*max_trans; y_trans<=max_trans; 
				     y_trans=y_trans+trans_step){	
					// create translation matrix
					trans_mat = Mat::zeros(2, 3, CV_64FC1);
					trans_mat.at<double>(0,0) = 1; trans_mat.at<double>(1,1) = 1;
					trans_mat.at<double>(0,2) = x_trans ; trans_mat.at<double>(1,2) = y_trans;
					
					// transform the image by applying rotation
					// and translation matrices
					warpAffine(src, dst, rot_mat, dsize);
					warpAffine(dst, dst, trans_mat, dsize);
					// inverse image
					bitwise_not (dst, dst);

					// create label strings with 0 padding for
					// rotation
					id_str = to_string(id);

					if (rot+max_rot<10){
					       	rot_str = "0" + to_string(rot+max_rot);
					}
					else rot_str = to_string(rot+max_rot);

					x_trans_str = to_string(x_trans+max_trans);
					y_trans_str = to_string(y_trans+max_trans);			
					
					// write resulting image
					filename = ("label=" + label +
						    "-rot=" + rot_str +
						    "-x_trans=" + x_trans_str +
						    "-y_trans=" + y_trans_str +
						    ".png");

					imwrite(dst_path + id_str + 
						"/" + filename,
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
