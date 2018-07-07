#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// Devuelve el cuarto inferior de la imagen

Mat region_inferior(const Mat &imagen) {
	
	int filas = imagen.rows;
	int columnas = imagen.cols;
	
	Mat region_interes;
	Rect ROI(0,filas/4*3,columnas,filas/4);
	region_interes = imagen(ROI);
	
	return region_interes;
}
