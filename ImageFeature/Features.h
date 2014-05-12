#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
using namespace cv;
using namespace std;

class ImageFeature
{
	int id;
	double *f1;
	double *f2;
	double *f3;
	double distance(const ImageFeature& a, int id);
};


DLLEXPORT void CalFeatureForImages(string path);
typedef void (*pCalFeatureForImages)(string path);