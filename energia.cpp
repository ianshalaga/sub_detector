#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// Calcula la energia de una imagen

double energia(const Mat &imagen) {
	
	int filas = imagen.rows;
	int columnas = imagen.cols;
	
	double energia = norm(imagen,NORM_L2); // Norma euclideana de la imagen
	energia = sqrt(energia)/(filas*columnas); // Energia de la imagen
	
	return energia;
}
