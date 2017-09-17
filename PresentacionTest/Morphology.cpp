#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include <stdio.h>

IplImage* src = 0;
IplImage* dst = 0;

IplConvKernel* element = 0;
int morph_operator = CV_MOP_OPEN;
int element_shape = CV_SHAPE_ELLIPSE;

//the address of variable which receives trackbar position update
int max_iters = 20;

int morph_pos = 0;
int operator_pos = 0;

// callback function for iterations trackbar
void Morph(int pos)
{
	int n = morph_pos;
	int an = n > 0 ? n : -n;
	element = cvCreateStructuringElementEx(an * 2 + 1, an * 2 + 1, an, an, element_shape, 0);
	cvMorphologyEx(src, dst, NULL, element, morph_operator);
	cvReleaseStructuringElement(&element);
	cvShowImage("Morphology", dst);
}

void setOperator(int pos)
{
	switch (pos)
	{
	case 0:
		morph_operator = CV_MOP_OPEN;
		break;
	case 1:
		morph_operator = CV_MOP_CLOSE;
		break;
	case 2:
		morph_operator = CV_MOP_ERODE;
		break;
	case 3:
		morph_operator = CV_MOP_DILATE;
		break;
	case 4:
		morph_operator = CV_MOP_GRADIENT;
		break;
	case 5:
		morph_operator = CV_MOP_TOPHAT;
		break;
	case 6:
		morph_operator = CV_MOP_BLACKHAT;
		break;
	}
	
	cvSetTrackbarPos("Iteraciones", "Morphology", 0);
	Morph(0);
}

void help()
{
	printf("This program demonstrated the use of the morphology operator, especially open, close, erode, dilate operations\n"
		"Morphology operators are built on max (close) and min (open) operators as measured by pixels covered by small structuring elements.\n"
		"These operators are very efficient.\n"
		"This program also allows you to play with elliptical, rectangluar and cross structure elements\n"
		"Call:\n"
		"./morphologyc [image_name -- Default baboon.jpg]\n"
		"\nHot keys: \n"
		"\tESC - quit the program\n"
		"\tr - use rectangle structuring element\n"
		"\te - use elliptic structuring element\n"
		"\tc - use cross-shaped structuring element\n"
		"\tSPACE - loop through all the options\n");
}

int main(int argc, char** argv)
{
	char* filename = argc == 2 ? argv[1] : (char*)"lena.jpg";
	if ((src = cvLoadImage(filename, 1)) == 0)
		return -1;

	help();

	dst = cvCloneImage(src);

	//create windows for output images
	cvNamedWindow("Morphology", 1);

	cvCreateTrackbar("Iteraciones", "Morphology", &morph_pos, max_iters, Morph);
	cvCreateTrackbar("Filtro", "Morphology", &operator_pos, 7, setOperator);

	for (;;)
	{
		int c;

		setOperator(operator_pos);
		Morph(morph_pos);

		c = cvWaitKey(0);

		if ((char)c == 27)
			break;
		if ((char)c == 'e')
			element_shape = CV_SHAPE_ELLIPSE;
		else if ((char)c == 'r')
			element_shape = CV_SHAPE_RECT;
		else if ((char)c == 'c')
			element_shape = CV_SHAPE_CROSS;
		else if ((char)c == ' ')
			element_shape = (element_shape + 1) % 3;
	}

	//release images
	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	//destroy windows
	cvDestroyWindow("Morphology");

	return 0;
}
