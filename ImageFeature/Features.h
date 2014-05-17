#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include<cv.h>
#include "../Tools/LoadImageLib.h"
using namespace cv;
using namespace std;

#define GLCM 1
#define EH 2
#define SIFT 3
#define HU 4
#define GLCM_LEVEL 8

class ImageFeature
{
public:
	int id;
	double* GrayLevelCoocurrenceMatrix;
	double* EdgeHist;
	double* Sift;
	double* Hu;
	int GLCM_length;
	int EH_length;
	int SIFT_length;
	int HU_length;
	double* getFeat(int FeatID);
	int getlength(int FeatID);
	double Distance(ImageFeature a, int FeatID);
	void genFeat(Mat img);
	//Mat ReadImage(int _id);
};

class calculateFeature
{
public:
	void calcGLCM(Mat img, int offset1, int offset2, double* a);
	//double* calcCLCM(Mat img, int offset1, int offset2);
	void calcEH(Mat img, int* edgehist);
	void calcHU(Mat img, double* hu);
	double* calcSIFT(Mat img);
};

	/*
#define WIDTH 32
#define HEIGHT 32
#define CHANNEL 3
#define IMAGESIZE 3073 // 32*32*3+1
	*/


DLLEXPORT ImageFeature* CalFeatureForImages(MyMat* imgs, int num);
typedef ImageFeature* (*PCalFeatureForImages)(MyMat* imgs, int num);

DLLEXPORT double CalFeatureDistance(ImageFeature &ele1, ImageFeature &ele2, int FeatID);
typedef double (*PCalFeatureDistance)(ImageFeature &ele1, ImageFeature &ele2, int FeatID);