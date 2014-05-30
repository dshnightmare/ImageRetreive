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
#define LBP 7


#define MAX_FEAT_ID 7
#define GLCM_LEVEL 8
#define SIFT_VOCA_SIZE 50

#define GLCM_length  5
#define EH_length  5
#define HU_length  7
#define HSV_length  9
#define SIFT_length  SIFT_VOCA_SIZE
#define WAVE_length  12
#define LBP_length  256


class calculateFeature;
class ImageFeature
{
public:
	//int id;
	double GrayLevelCoocurrenceMatrix[5];
	double EdgeHist[5];
	double Sift[SIFT_VOCA_SIZE];
	double Hu[7];
	double HSVFeat[9];
	double WaveFeat[12];
	double Lbp[256];

	ImageFeature(){};
	~ImageFeature(){};
	double* getFeat(int FeatID);
	int getlength(int FeatID);
	double Distance(ImageFeature &a, int FeatID);
	void genFeat(Mat img, int FeatID, calculateFeature calc);
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
	void calcLBP(Mat img, double* lbp);
	void calcHSV(Mat img, double* hsv);
	void calcWaveFeat(Mat img, double* wave);
	void loadVocFile();
};

	/*
#define WIDTH 32
#define HEIGHT 32
#define CHANNEL 3
#define IMAGESIZE 3073 // 32*32*3+1
	*/


DLLEXPORT ImageFeature* CalFeatureForImages(MyMat* imgs, int num, BOOL isFromLib, int FeatIDs[], int featCount);
typedef ImageFeature* (*PCalFeatureForImages)(MyMat* imgs, int num, BOOL isFromLib, int FeatIDs[], int featCount);

DLLEXPORT double CalFeatureDistance(ImageFeature &ele1, ImageFeature &ele2, int FeatIDs[], int weight[], int num);
typedef double (*PCalFeatureDistance)(ImageFeature &ele1, ImageFeature &ele2, int FeatID[], int weight[], int num);

DLLEXPORT ImageFeature* Create(int num);
typedef ImageFeature* (*PCreate)(int num);
