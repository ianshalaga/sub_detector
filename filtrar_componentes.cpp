#include "opencv2/opencv.hpp"          // OpenCV
#include "opencv2/highgui/highgui.hpp" // OpenCV
#include "iostream"   				   // std::cout
#include "string"  					   // std::string, std::to_string
#include "pdi_functions.h"             // Funciones de Walter
#include "djr_functions.h"             // Mis funciones

using namespace pdi;
using namespace std;
using namespace cv;

// Un paso mas en la eliminacion de elementos indeseados

Mat filtrar_componentes(const Mat &mascara,const int &ee_dim) {
	
	Mat componentes, // Componentes conectadas etiquetadas
		estadisticas, // Valor mas a la izquierda, valor mas arriba, ancho del bounding box, alto del bounding box, area de cada componentes
		centroides; // Posicion de los centroides de las componentes etiquetadas
	connectedComponentsWithStats(mascara,componentes,estadisticas,centroides); // Las salidas son del tipo CV_32S
	componentes.convertTo(componentes,CV_8UC(1)); // Conversion a uchar (0 - 255)
	
	Mat sin_espurios = componentes.clone(); // Sin las componentes chicas
	Mat el_mayor = componentes.clone(); // La mayor de las componentes
	
	estadisticas = estadisticas(Rect(4,1,1,estadisticas.rows-1)); // Solo interesa la columna con las areas sin el area del fondo
	
	int filas = componentes.rows; // Filas de la matriz de componentes (para los for)
	int columnas = componentes.cols; // Columnas de la matriz de componentes (para los for)
	
	/*Eliminacion de pixeles espurios*/
	for(int k=0;k<estadisticas.rows;k++) { // Recorre las areas de las componentes
		if (estadisticas.at<int>(k,0) > ee_dim*ee_dim) { // Area mayor a la cantidad de pixeles que conforman el elemento estructurante utilizado para la erosion
			for(int i=0;i<filas;i++) { 
				for(int j=0;j<columnas;j++) { 
					if (componentes.at<uchar>(i,j) == k+1) // k+1 porque falta el fondo
						sin_espurios.at<uchar>(i,j) = 255;
				}
			}
		} else {
			for(int i=0;i<filas;i++) { 
				for(int j=0;j<columnas;j++) { 
					if (componentes.at<uchar>(i,j) == k+1) // k+1 porque falta el fondo
						sin_espurios.at<uchar>(i,j) = 0;
				}
			}
		}
	} // Fin del for de eliminacion de pixeles espurios
	
	/*Determinacion del area maxima*/
	double min,max;
	minMaxLoc(estadisticas,&min,&max);
	
	/*Solo permanece la mayor de las areas*/
	for(int k=0;k<estadisticas.rows;k++) { // Recorre las areas de las componentes
		if (estadisticas.at<int>(k,0) == max) { // Es el maximo?
			for(int i=0;i<filas;i++) { 
				for(int j=0;j<columnas;j++) { 
					if (componentes.at<uchar>(i,j) == k+1) // k+1 porque falta el fondo
						el_mayor.at<uchar>(i,j) = 255;
					else
						el_mayor.at<uchar>(i,j) = 0;
				}
			}
			break; // Una vez encontrada el area maxima no necesita continuar
		}
	} // Fin del for del area mas grande
	
	Mat componentes_filtradas; // Imagen resultado
	bitwise_and(sin_espurios,el_mayor,componentes_filtradas); // And entre ambas mascaras obtenidas
	
	return componentes_filtradas;
}
