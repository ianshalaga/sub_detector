#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include "pdi_functions.h"

using namespace std;
using namespace pdi;
using namespace cv;

Mat concatenar_imagenes(vector<Mat> imagenes,const bool &color,const bool &vertical) {
	
	if (color == true) {
		for(int i=0;i<imagenes.size();i++) { 
			if (imagenes[i].channels() == 1)
				cvtColor(imagenes[i],imagenes[i],CV_GRAY2BGR);
		}
	} else {
		for(int i=0;i<imagenes.size();i++) { 
			if (imagenes[i].channels() == 3)
				cvtColor(imagenes[i],imagenes[i],CV_BGR2GRAY);
		}
	}

	Mat mosaico = imagenes[0];
	
	if (vertical == true) {
		for(int i=1;i<imagenes.size();i++) { 
			int filas = imagenes[i].rows;
			int columnas = imagenes[i].cols;
			resize(imagenes[i],imagenes[i],{mosaico.cols,mosaico.cols*filas/columnas},0,0,CV_INTER_LINEAR);
		}
		for(int i=1;i<imagenes.size();i++) { 
			mosaico = mosaic(mosaico,imagenes[i],true);
		}
	} else {
		for(int i=1;i<imagenes.size();i++) {
			int filas = imagenes[i].rows;
			int columnas = imagenes[i].cols;
			resize(imagenes[i],imagenes[i],{mosaico.rows*columnas/filas,mosaico.rows},0,0,CV_INTER_LINEAR);
		}
		for(int i=1;i<imagenes.size();i++) { 
			mosaico = mosaic(mosaico,imagenes[i],false);
		}
	}
	
	return mosaico;
}
