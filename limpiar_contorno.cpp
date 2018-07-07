#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include "djr_functions.h"

using namespace pdi;
using namespace cv;

// Coloca el contorno de la imagen de un pixel de grosor en cero

Mat limpiar_contorno(Mat imagen,int pixeles) {
	
	int filas = imagen.rows;
	int columnas = imagen.cols;
	
	Mat mascara(filas,columnas,CV_8UC(1),Scalar(255));
	
	for(int i=0;i<filas;i++) { 
		for(int k=0;k<pixeles;k++) { 
			mascara.at<uchar>(i,k) = 0;
			mascara.at<uchar>(i,columnas-1-k) = 0;
		}
	}
	
	for(int j=0;j<columnas;j++) { 
		for(int k=0;k<pixeles;k++) { 
			mascara.at<uchar>(k,j) = 0;
			mascara.at<uchar>(filas-1-k,j) = 0;
		}
	}
	
	Mat sin_contorno;
	bitwise_and(imagen,mascara,sin_contorno);
	
	return sin_contorno;
}
