#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

#include "stdafx.h"
#include "LSHTable.h"
#include "../ImageFeature/Features.h"

using namespace std;

class LSHTBuilder
{
private:
	LSHTable *tableArray;
	int tableNum;

public:
	LSHTBuilder();
	LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,double itcptRate);
	LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,double itcptRate,ofstream &fout);
	//build from double** and write to file//API2
	LSHTBuilder(ifstream &fin);//read builder from file//API1
	LSHTBuilder(LSHTable *tabArray,int tabNum);

	LSHTable *getTable(int& tabNum);
	void writeToFile(ofstream &fout);
	void getTabSize(int* tSize);
	void ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double lthresh,double thresh,double itcptRate,double uthresh,int maxIter);
};



DLLEXPORT LSHTBuilder* Builder(ImageFeature *imgFeatArray,int dataNum);
typedef LSHTBuilder* (*PBuilder)(ImageFeature *imgFeatArray,int dataNum);