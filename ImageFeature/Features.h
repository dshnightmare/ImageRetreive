#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include <cv.h>
#include <opencv2/nonfree/features2d.hpp>
#include "../Tools/LoadImageLib.h"
using namespace cv;
using namespace std;

#define GLCM 1
#define EH 2
#define HU 3
#define HSV 4
#define SIFT 5
#define WAVELET 6
#define MAX_FEAT_ID 6
#define GLCM_LEVEL 8


class calculateFeature;
class ImageFeature
{
public:
	int id;
	double* GrayLevelCoocurrenceMatrix;
	double* EdgeHist;
	double* Sift;
	double* Hu;
	double* HSVFeat;
	double* WaveFeat;

	int GLCM_length;
	int EH_length;
	int SIFT_length;
	int HU_length;
	int HSV_length;
	int WAVE_length;

	ImageFeature();
	~ImageFeature();
	double* getFeat(int FeatID);
	int getlength(int FeatID);
	double Distance(ImageFeature a, int FeatID);
	void genFeat(Mat img, calculateFeature calc);
	//Mat ReadImage(int _id);
};

class calculateFeature
{
private:
	Mat localVocabulary;
public:
	void calcGLCM(Mat img, int offset1, int offset2, double* a);
	//double* calcCLCM(Mat img, int offset1, int offset2);
	void calcEH(Mat img, double* edgehist);
	void calcSIFT(Mat img, double* sift);
	//遍历所有图像,建立bow词汇表,并写入文件中保存
	void siftBowPreprocess(MyMat *imgs, int num);
	void calcHU(Mat img, double* hu);
	void calcHSV(Mat img, double* hsv);
	void calcWaveFeat(Mat img, double* wave);
};

	/*
#define WIDTH 32
#define HEIGHT 32
#define CHANNEL 3
#define IMAGESIZE 3073 // 32*32*3+1
	*/


DLLEXPORT ImageFeature* CalFeatureForImages(MyMat* imgs, int num, BOOL isFromLib);
typedef ImageFeature* (*PCalFeatureForImages)(MyMat* imgs, int num, BOOL isFromLib);

DLLEXPORT double CalFeatureDistance(ImageFeature &ele1, ImageFeature &ele2, int FeatIDs[], int weight[], int num);
typedef double (*PCalFeatureDistance)(ImageFeature &ele1, ImageFeature &ele2, int FeatID[], int weight[], int num);

DLLEXPORT ImageFeature* Create(int num);
typedef ImageFeature* (*PCreate)(int num);
