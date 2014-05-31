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
	MyMat():Mat(), type(0), id(-1){}
	~MyMat(){~Mat();}
	MyMat(MyMat &m):Mat(m.clone()), type(m.type), id(m.id){}
};

DLLEXPORT  MyMat* LoadFromCIFAR10(string path);
typedef MyMat* (*PLoadFromCIFAR10)(string path);

DLLEXPORT  MyMat* LoadFromCIFAR10Cal(string path);
typedef MyMat* (*PLoadFromCIFAR10Cal)(string path);

DLLEXPORT MyMat* LoadFromCIFAR10Test(string path);
typedef MyMat* (*PLoadFromCIFAR10Test)(string path);

DLLEXPORT  MyMat* LoadFromCorel1000(string path);
typedef MyMat* (*PLoadFromCorel1000)(string path);

// 10000的还没分类
//DLLEXPORT  MyMat* LoadFromCorel10000(string path);
//typedef MyMat* (*PLoadFromCorel10000)(string path);