#include "opencv2/opencv.hpp"          // OpenCV
#include "opencv2/highgui/highgui.hpp" // OpenCV
#include "iostream"   				   // std::cout
#include "string"  					   // std::string, std::to_string
#include "pdi_functions.h"             // Funciones de Walter
#include "djr_functions.h"             // Mis funciones

using namespace pdi;
using namespace std;
using namespace cv;

// Detecta la presencia de subtitulos en una imagen

void sub_detector(const string &nombre_video) {
	
	VideoCapture video(nombre_video); // Carga del video
	int total_frames = video.get(CV_CAP_PROP_FRAME_COUNT); // Cantidad total de fotogramas del video
	
	Mat kernel = kernelPA0(); // Kernel para la deteccion de bordes
	
	int k = 0;
	video.set(CV_CAP_PROP_POS_FRAMES,k);
	
	for(int i=k;i<total_frames;i++) { // Recorre todos los fotogramas del video
		
		cout << "fotograma" << i-k << endl; // Muestra el progreso del procesamiento
		
		/*Region de interes*/
		Mat fotograma; video >> fotograma; // Extrae el fotograma i del video
		if (fotograma.empty()) break; // En caso de fotogramas vacios		
		Mat fotograma_inferior = region_inferior(fotograma); // Extrae el cuarto inferior del fotograma i
		
		/*Reduccion de ruido*/
		Mat fotograma_gauss; // Imagen desenfocada
		GaussianBlur(fotograma_inferior,fotograma_gauss,{3,3},0,0,BORDER_DEFAULT); // Desenfoque gaussiano
		
		Mat fotograma_mediana; // Imagen con filtrado de mediana
		medianBlur(fotograma_gauss,fotograma_mediana,3); // Filtrado de mediana
		
		/*Separacion en canales*/
		vector<Mat> fotograma_bgr_vec, fotograma_hsv_vec; // Vectores de canales
		split(fotograma_mediana,fotograma_bgr_vec); // Separacion en canales BGR
		Mat fotograma_hsv; // Imagen para la conversion a HSV
		cvtColor(fotograma_mediana,fotograma_hsv,CV_BGR2HSV); // Conversion al modelo de color HSV
		split(fotograma_hsv,fotograma_hsv_vec); // Separacion en canales HSV
		Mat fotograma_gris; // Imagen para la conversion a escala de grises
		cvtColor(fotograma_mediana,fotograma_gris,CV_BGR2GRAY); // Conversion a escala de grises
		
		/*Conversion de tipo para el promediado de canales*/
		Mat canalB_f, canalG_f, canalR_f, /*canalH_f,*/ canalS_f, canalV_f, canalGS_f; // Canales pasados a flotante
		fotograma_bgr_vec[0].convertTo(canalB_f,CV_64FC(1),1.0/255); // Conversion del canal B a flotante
		fotograma_bgr_vec[1].convertTo(canalG_f,CV_64FC(1),1.0/255); // Conversion del canal G a flotante
		fotograma_bgr_vec[2].convertTo(canalR_f,CV_64FC(1),1.0/255); // Conversion del canal R a flotante
//		fotograma_hsv_vec[0].convertTo(canalH_f,CV_64FC(1),1.0/255); // Conversion del canal H a flotante
		fotograma_hsv_vec[1].convertTo(canalS_f,CV_64FC(1),1.0/255); // Conversion del canal S a flotante
		fotograma_hsv_vec[2].convertTo(canalV_f,CV_64FC(1),1.0/255); // Conversion del canal V a flotante
		fotograma_gris.convertTo(canalGS_f,CV_64FC(1),1.0/255); // Conversion de la imagen en escala de grises a flotante
		
		/*Promediado de canales*/
		Mat suma_canales = canalB_f + canalG_f + canalR_f + /*canalH_f +*/ canalS_f + canalV_f + canalGS_f; // Suma de canales.
		Mat promedio_canales = suma_canales/(3+2+1); // Promedio de canales. 3(BGR), 2(SV), 1(escala de grises)
		promedio_canales.convertTo(promedio_canales,CV_8UC(1),255); // Conversion a uchar
		
		/*Deteccion de bordes del promedio de canales*/
		Mat promedio_canales_bordes = convolve(promedio_canales,kernel); // Bordes del promedio de canales
		promedio_canales_bordes = limpiar_contorno(promedio_canales_bordes,1); // Elimina el contorno que se genera tras la convolucion
		
		/*Binarizacion de los bordes*/
		Mat promedio_canales_bordes_u; // Imagen para la binarizacion de los bordes
		threshold(promedio_canales_bordes,promedio_canales_bordes_u,0,255,THRESH_BINARY|THRESH_OTSU); // Binarizacion de los bordes
		
		/*Rellenado de bordes*/
		int ee_dim = 3*0.0104167*fotograma.rows; // El elemento estructurante se adapta a la resolucion vertical del video
		Mat ee(ee_dim,ee_dim,CV_8UC(1),Scalar(1)); // Elemento estructurante para el llenado de bordes
		Mat bordes_llenados; // Imagen para los bordes rellenados
		morphologyEx(promedio_canales_bordes_u,bordes_llenados,MORPH_CLOSE,ee,Point(-1,-1),1); // Rellenado de bordes
		
		/*Limpieza de pixeles espurios*/
		Mat bordes_mediana;
		medianBlur(bordes_llenados,bordes_mediana,5);
		
		/*Proceso de limpieza avanzado*/
		Mat componentes_filtradas = filtrar_componentes(bordes_mediana,ee_dim);
		
		/*Dilatacion horizontal*/
		Mat bordes_dilatados;
		Mat ee_dilatacion(1,fotograma.cols,CV_8UC(1),Scalar(1));
		dilate(componentes_filtradas,bordes_dilatados,ee_dilatacion,Point(-1,-1),1);
		
		/*Deteccion de lineas horizontales*/
		vector<Vec2f> lineas_horizontales;
		HoughLines(bordes_dilatados,lineas_horizontales,1,CV_PI/2,1);
		
		/*Linea de deteccion*/
		Mat lineas_sub = lineas_hough(fotograma_gris,lineas_horizontales);
		
		/*Presentacion de resultados*/
		cvtColor(promedio_canales,promedio_canales,CV_GRAY2BGR);
		cvtColor(promedio_canales_bordes,promedio_canales_bordes,CV_GRAY2BGR);
		cvtColor(promedio_canales_bordes_u,promedio_canales_bordes_u,CV_GRAY2BGR);
		cvtColor(bordes_llenados,bordes_llenados,CV_GRAY2BGR);
		cvtColor(bordes_mediana,bordes_mediana,CV_GRAY2BGR);
		cvtColor(componentes_filtradas,componentes_filtradas,CV_GRAY2BGR);
		cvtColor(bordes_dilatados,bordes_dilatados,CV_GRAY2BGR);
//		cvtColor(lineas_sub,lineas_sub,CV_GRAY2BGR);
		
		putText(promedio_canales,"Promedio de canales",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(promedio_canales_bordes,"Bordes del promedio",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(promedio_canales_bordes_u,"Bordes binarizados",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(bordes_llenados,"Bordes rellenados",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(bordes_mediana,"Bordes mediana",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(componentes_filtradas,"Limpieza avanzada",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(bordes_dilatados,"Dilatacion horizontal",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		putText(lineas_sub,"Linea de deteccion",Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0),1,CV_AA);
		
		vector<Mat> resultados = {promedio_canales,
								  promedio_canales_bordes,
								  promedio_canales_bordes_u,
								  bordes_llenados,
								  bordes_mediana,
								  componentes_filtradas,                                                                               
								  bordes_dilatados,
								  lineas_sub};
		
		Mat mosaico_res = concatenar_imagenes(resultados,true,true);
		
		/*Escritura en archivos*/
		stringstream s;
		s << "pruebas/" << "fotograma" << i-k << ".jpg";
		imwrite(s.str(),mosaico_res);
	} // Fin del fro que recorre todos los fotogramas del video
} // Fin de sub_detector
