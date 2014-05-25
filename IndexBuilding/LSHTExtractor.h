#pragma once

#ifndef DLLEXPORT
#define DLLEXPORT extern "C" __declspec(dllexport)
#endif

//include
#include "stdafx.h"
#include "../ImageFeature/Features.h"
#include "LSHTable.h"

class LSHTExtractor
{
private:
	int tableNum;
	
public:
	LSHTable *tableArray;
	LSHTExtractor(ifstream &fin);//cur
	LSHTExtractor(double **dataVecList,int dataNumber,int vecLength,int keyLength,int tabNum,double thresh);//test
	LSHTExtractor(LSHTable *tabArray,int tabNum);
	
	void printTables();
	
	vector<int> getCandIDset(double *dataVec,int maxEDist);
	vector<int> LSHTExtractor::getCandIDsetf(double *dataVec,int maxEDist);
	vector<int> getCandIDset(double *dataVec,int maxEDist,bool useThresh);
	vector<int> getCandIDs(double *dataVec,int maxEDist);
	vector<int> getCandIDsf(double *dataVec,int maxEDist);
	void getTabSize(int* tSize);//only for builder
	void ajustTowardsAvg(double **dataVecList,int dataNumber,int vecLength,int keyLength,double thresh,double lthresh,double uthresh,int maxIter);
	vector<int> resultFilter(vector<int>* candSetList,double uth);
};


DLLEXPORT vector<int> Extractor(int fid,ImageFeature &imgfeat);
typedef vector<int> (*PExtractor)(int fid,ImageFeature &imgfeat);