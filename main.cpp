#include "opencv2/opencv.hpp"          // OpenCV
#include "opencv2/highgui/highgui.hpp" // OpenCV
#include "iostream"   				   // std::cout
#include "string"  					   // std::string, std::to_string
#include "pdi_functions.h"             // Funciones de Walter
#include "djr_functions.h"             // Mis funciones

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	
	/*Carga del video*/
	stringstream s_v1,s_v2;
	s_v1 << "y480_r";
	s_v2 << "videos/" << s_v1.str() << ".mp4";
	
	/*Detectar subtitulos*/
	sub_detector(s_v2.str());
	
	return 0;
} 
