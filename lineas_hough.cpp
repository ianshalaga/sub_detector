#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include "pdi_functions.h"
#include "opencv2/video/tracking.hpp"
#include "djr_functions.h"

using namespace std;
using namespace pdi;
using namespace cv;

// Dibuja una linea horizontal donde los subtitulos fueron detectados

Mat lineas_hough(const Mat &bordes,const vector<Vec2f> &rho_tita) {
	
	Mat bordes_color;
	cvtColor(bordes,bordes_color,CV_GRAY2BGR);
	
	if (rho_tita.size() != 0) {
		for(size_t i=0;i<1/*rho_tita.size()*/;i++) {
			float rho = rho_tita[i][0];
			float theta = rho_tita[i][1];
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			Point pt1(cvRound(x0 + 1000*(-b)),
					  cvRound(y0 + 1000*(a)));
			Point pt2(cvRound(x0 - 1000*(-b)),
					  cvRound(y0 - 1000*(a)));
			line(bordes_color,pt1,pt2,Scalar(0,0,255),3,8);
		}
	}
	
	return bordes_color;
}
