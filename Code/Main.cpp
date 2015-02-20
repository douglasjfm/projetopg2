#include "Main.h"
#include "util.h"
#include <math.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "glfuncoes.h"


// OpenCV includes
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;


cv::VideoCapture cap;
Mat frame;

void initCV()
{
	cap.open("Resources\\InputData\\video2.mp4");

	if (!cap.isOpened())
	{
		cout << "error, could not open the capture" << endl;
		system("pause");
		exit(1);
	}

	namedWindow("video", WINDOW_AUTOSIZE);
}

void initialize()
{
	initCV();

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

void updateCV()
{
	cap >> frame;

	// loopzinho
	if (cap.get(CV_CAP_PROP_POS_FRAMES) == 500)
	{
		cap.set(CV_CAP_PROP_POS_FRAMES, 10);
	}

	imshow("video", frame);
}




void imgExtracao(Mat img, std::vector<KeyPoint> kps, Mat *desc)
{
	SurfDescriptorExtractor extractor;
	extractor.compute(img, kps, *desc);
}

void imgPontosChaves(Mat img, std::vector<KeyPoint> *kps, Mat *pontos)
{
	SurfFeatureDetector detector(400);

	detector.detect(img, *kps);
	drawKeypoints(img, *kps, *pontos/*,Scalar(127,127,127)*/, 4);
}

Mat cinzasSobel(Mat src_gray, Mat grad)
{
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	return grad;
}

void casar(Mat desc1, Mat desc2, vector<DMatch> *m)
{
	BFMatcher matcher(NORM_L2);
	matcher.match(desc1, desc2, *m);
}

void CVergonhice(){

	int fcount = 0;

	Mat padrao, modelo, kPadrao, kModelo, kCasada, exModelo, exPadrao;
	Mat padraoCinza, modeloCinza, padraoBlur, modeloBlur;
	Mat padraoSobel, modeloSobel;
	std::vector<KeyPoint> kpPadrao, kpModelo;
	vector<DMatch> pontosCasados;

	modelo = imread("Resources\\InputData\\obj.jpg");
	padrao = imread("Resources\\InputData\\objs.jpg");

	/// Filtro gaussiano
	GaussianBlur(padrao, padrao, Size(3, 3), 0, 0, BORDER_DEFAULT);
	GaussianBlur(modelo, modelo, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// RGB -> Escala de cinza
	cvtColor(padrao, padraoCinza, CV_RGB2GRAY);
	cvtColor(modelo, modeloCinza, CV_RGB2GRAY);

	/// Filtro de Sobel
	//padraoSobel = cinzasSobel(padraoCinza,padraoSobel);
	//modeloSobel = cinzasSobel(modeloCinza,modeloSobel);

	/// Calculo de pontos chave
	imgPontosChaves(modeloCinza, &kpModelo, &kModelo);///Caso usar o filtro de Sobel, usar 'modeloSobel';
	imgPontosChaves(padraoCinza, &kpPadrao, &kPadrao);///Caso usar o filtro de Sobel, usar 'padraoSobel';

	/// Extracao de features
	imgExtracao(modelo, kpModelo, &exModelo);
	imgExtracao(padrao, kpPadrao, &exPadrao);

	casar(exModelo, exPadrao, &pontosCasados);

	drawMatches(modelo, kpModelo, padrao, kpPadrao, pontosCasados, kCasada);

	imshow("PG - CIn/UFPE 2014.2", kCasada);

	imwrite("match.jpg", kCasada);
	imwrite("kModelo.jpg", kModelo);
	imwrite("kPadrao.jpg", kPadrao);
	printf("\n\t# de Frames: %d", fcount);

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(mydisplay);
	updateCV();
	glutReshapeFunc(myreshape);
	glutKeyboardUpFunc(handleKeyboardUp);
	glutKeyboardFunc(handleKeyboardPressed);
	glutIdleFunc(idleFunction);
	initialize();

	CVergonhice();

	glutMainLoop();
	return 0;
}