#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include<cv.h>
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