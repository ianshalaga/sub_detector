#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// Kernel pasa-altos de suma cero que prioriza bordes horizontales

Mat kernelPA0() {
	
	Mat kernel(3,3,CV_64F,Scalar(-1));
	
//	kernel.at<double>(1,1) = 10; // centro
	kernel.at<double>(1,1) = 7; // centro
	
//	kernel.at<double>(0,1) = -2; // centro superior
//	kernel.at<double>(2,1) = -2; // centro inferior
	
	return kernel;
}
