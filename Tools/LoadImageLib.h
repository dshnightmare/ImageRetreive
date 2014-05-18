#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include<cv.h>
#include <fstream>
using namespace cv;
using namespace std;

class MyMat: public Mat
{
public:
	int id;
	char type;
	MyMat():Mat(), type(0), id(-1){};
	MyMat(MyMat &m):Mat(m.clone()), type(m.type), id(m.id){};
};

DLLEXPORT  MyMat* LoadFromCIFAR10(string path);
typedef MyMat* (*PLoadFromCIFAR10)(string path);

DLLEXPORT MyMat* LoadFromCIFAR10Test(string path);
typedef MyMat* (*PLoadFromCIFAR10Test)(string path);