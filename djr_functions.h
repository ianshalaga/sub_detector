#ifndef DJR_FUNCTIONS_H
#define DJR_FUNCTIONS_H

// Devuelve el cuarto inferior de la imagen
cv::Mat region_inferior(const cv::Mat &imagen);

// Kernel pasa-altos de suma cero que prioriza bordes horizontales
cv::Mat kernelPA0();

// Dvuelve el o los histogramas de la imagen dependiendo de la cantidad de canales
std::vector<cv::Mat> histogramas(const cv::Mat &imagen);

// Promedio de intensidades de un canal
double promedio_intensidad(const cv::Mat &imagen);

// Concatena imagenes
cv::Mat concatenar_imagenes(std::vector<cv::Mat> imagenes,
							const bool &color,
							const bool &vertical);

// Calcula la energia de una imagen
double energia(const cv::Mat &imagen);

// Coloca el contorno de la imagen en cero
cv::Mat limpiar_contorno(cv::Mat imagen,int pixeles);

// Un paso mas en la eliminacion de elementos indeseados
cv::Mat filtrar_componentes(const cv::Mat &mascara,const int &ee_dim);

// Dibuja una linea horizontal donde los subtitulos fueron detectados
cv::Mat lineas_hough(const cv::Mat &bordes,const std::vector<cv::Vec2f> &rho_tita);

// Detecta la presencia de subtitulos en una imagen
void sub_detector(const std::string &nombre_video);

#endif
