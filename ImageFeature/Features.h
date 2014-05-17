#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include <fstream>
#include<cv.h>
#include <opencv2/nonfree/features2d.hpp>
using namespace cv;
using namespace std;

#define GLCM 1
#define EH 2
#define SIFT 3
#define GLCM_LEVEL 8

class ImageFeature
{
	int id;
	double* GrayLevelCoocurrenceMatrix;
	double* EdgeHist;
	double* Sift;
	int GLCM_length;
	int EH_length;
	int SIFT_length;
public:
	double* getGLCM();
	double* getEH();
	double* getSIFT();
	int getlength(int FeatID);
	double Distance(ImageFeature a, int FeatID);
	//Mat ReadImage(int _id);
};

class calculateFeature
{
private:
	Mat localVocabulary;
public:
	void calcGLCM(Mat img, int offset1, int offset2, double* a);
	double* calcCLCM(Mat img, int offset1, int offset2);
	void calcEH(Mat img, int* edgehist);
	double* calcSIFT(Mat img, int dictSize);
	//遍历所有图像,建立bow词汇表,并写入文件中保存
	void siftBowPreprocess();
};

	/*
#define WIDTH 32
#define HEIGHT 32
#define CHANNEL 3
#define IMAGESIZE 3073 // 32*32*3+1
	*/


DLLEXPORT void CalFeatureForImages(string path);
typedef void (*pCalFeatureForImages)(string path);