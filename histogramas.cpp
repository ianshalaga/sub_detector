#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include "pdi_functions.h"
#include "vector"
#include "djr_functions.h"

using namespace std;
using namespace pdi;
using namespace cv;

vector<Mat> histogramas(const Mat &imagen) {
	
	vector<Mat> histo;
	
	// Crear un histograma con 256 bin (numero de subdivisiones) uno por pixel [0..256]
	int numbins = 256;
	
	// Establecer rango para los canales (B, G, R)
	float range[] = { 0, 256 };
	const float* histRange = { range };
	
	if (imagen.channels() == 1) {
		Mat hist;
		
		// Calcular el histograma
		calcHist(&imagen,1,0,Mat(),hist,1,&numbins,&histRange);
		
		// Normalizar el histograma
		normalize(hist,hist,0,1,CV_MINMAX);
		
		int rows = hist.rows;
		
		Mat graph(rows,rows,CV_8UC(1),Scalar(0));
		
		for (size_t k=0;k<rows-1;k++){
			float value = hist.at<float>(k);
			Point p1(k,rows);
			Point p2(k,rows-value*rows);
			line(graph,p1,p2,Scalar(255));
		}
		
		histo = {graph};
	} else if (imagen.channels() == 3) {
		// Dividir los canales BRG
		vector<Mat> bgr;
		split(imagen,bgr);
		
		Mat b_hist,g_hist,r_hist;
		
		// Calcular el histograma para cada canal
		calcHist(&bgr[0],1,0,Mat(),b_hist,1,&numbins,&histRange);
		calcHist(&bgr[1],1,0,Mat(),g_hist,1,&numbins,&histRange);
		calcHist(&bgr[2],1,0,Mat(),r_hist,1,&numbins,&histRange);
		
		// Normalizar los histogramas 
		normalize(b_hist,b_hist,0,1,CV_MINMAX);
		normalize(g_hist,g_hist,0,1,CV_MINMAX);
		normalize(r_hist,r_hist,0,1,CV_MINMAX);
		
		int rows = b_hist.rows;
		
		Mat graphB(rows,rows,CV_8UC(3),Scalar(0,0,0));
		Mat graphG(rows,rows,CV_8UC(3),Scalar(0,0,0));
		Mat graphR(rows,rows,CV_8UC(3),Scalar(0,0,0));
		
		for (size_t k=0;k<rows-1;k++){
			float valueB = b_hist.at<float>(k);
			float valueG = g_hist.at<float>(k);
			float valueR = r_hist.at<float>(k);
			Point p1(k,rows);
			Point p2B(k,rows-valueB*rows);
			Point p2G(k,rows-valueG*rows);
			Point p2R(k,rows-valueR*rows);
			line(graphB,p1,p2B,Scalar(255,0,0));
			line(graphG,p1,p2G,Scalar(0,255,0));
			line(graphR,p1,p2R,Scalar(0,0,255));
		}
		
		vector<Mat> mosaico_vec = {graphB,graphG,graphR};
		Mat mosaico_hist = concatenar_imagenes(mosaico_vec,true,true);
		
		histo = {graphB,graphG,graphR,mosaico_hist};		
	}
	
	return histo;
}
