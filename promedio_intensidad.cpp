#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// Promedio de intensidades de un canal

double promedio_intensidad(const Mat &imagen) {
	
	int filas = imagen.rows;
	int columnas = imagen.cols;
	
	double suma = sum(imagen)[0];
	
	double promedio = suma/(filas*columnas);
	
	return promedio;
}
