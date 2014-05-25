#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
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
	LSHTBuilder(ifstream &fin,int keyLen,int tabNum);
	LSHTBuilder(ifstream &fin,int keyLen,int tabNum,double thresh);//cur
	LSHTBuilder(ifstream &fin,int keyLen,int tabNum,double thresh,ofstream &fout);
	LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh);
	LSHTBuilder(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh,ofstream &fout);
	//LSHTBuilder(ifstream &fin);
	LSHTBuilder(LSHTable *tabArray,int tabNum);

	void writeToFile(ofstream &fout);
	void getTabSize(int* tSize);
	void ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double lthresh,double thresh,double uthresh,int maxIter);
};



DLLEXPORT void Builder(ImageFeature *imgFeatArray,int dataNum);
typedef void (*PBuilder)(ImageFeature *imgFeatArray,int dataNum);