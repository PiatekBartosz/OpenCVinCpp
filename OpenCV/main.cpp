#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;

// variables need to be global, look CannyThreshold implementation
int lowThreshold = 0;
const int max_low_Threshold = 100;
const int ratio = 3;
const int kernel_size = 3;
Mat resized_img, blured_img, regular_edge_detected_img, edge_detected_img;
Mat equalized_histogram_img, gray_img;
std::string window_name = "Edge detection";

static void CannyThreshold(int, void*);

int main(void) {

	// img path
	std::string path = "photo.jpg";

	// load & resize img & prepare output matrixw
	Mat original_img = imread(path);

	if (original_img.empty()) {
		std::cout << "Could not open img";
		return -1;
	}
	
	// resize img 
	resize(original_img, resized_img, Size(), 0.25, 0.25);
	cvtColor(resized_img, gray_img, COLOR_BGR2GRAY);

	// invert colors (subtract every pixel value from 255)
	Mat inverted_img = Scalar::all(255) - resized_img;
	
	// blur img with 5x5 Gaussian kernel
	GaussianBlur(resized_img, blured_img, Size(5, 5), 0);

	// detect edges usual way
	Canny(blured_img, regular_edge_detected_img, 10, 50, 3);

	// equalized historgram 
	equalizeHist(gray_img, equalized_histogram_img);

	// detect edges using Canny Edge Detector, use blured img as input to filter noise
	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_low_Threshold, CannyThreshold);
	CannyThreshold(0, 0);

	// save results to a file
	imwrite("invertedImg.jpg", inverted_img);
	imwrite("bluredImg.jpg", blured_img);
	imwrite("edgeImg.jpg", regular_edge_detected_img);
	imwrite("equalizedImg.jpg", equalized_histogram_img);

	// display results
	/*imshow("Resized", resized_img);
	imshow("Inverted", inverted_img);
	imshow("Blured", blured_img);
	imshow("Regular edge detection", regular_edge_detected_img);
	imshow("Equilzed Histogram", equalized_histogram_img);*/

	waitKey(0);
	destroyAllWindows();
	return 0;
}

// trackball callback function
void CannyThreshold(int, void*)
{	
	blur(gray_img, edge_detected_img, Size(3, 3));
	Canny(edge_detected_img, edge_detected_img, lowThreshold, lowThreshold*ratio, kernel_size);
	imshow(window_name, edge_detected_img);
}
